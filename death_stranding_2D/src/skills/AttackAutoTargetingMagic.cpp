#include "skills/AttackAutoTargetingMagic.h"
#include "world/World.h"
#include "layers/MainLayer.h"
#include "entities/AABB.h"

class AttackAutoTargetingMagicImpl
{
public:
    explicit AttackAutoTargetingMagicImpl(AttackAutoTargetingMagic *host)
        : host_(host) {}

    AABB getAttackContainer()
    {
        const auto &atkInfo = host_->attackInfo();
        const float radius = atkInfo.attackRadius();

        auto center = host_->owner_->center();

        return AABB::createAABB(center, radius);
    }

    void addItemToItemsInorder(ItemEntity *item, float health)
    {
        if (!host_)
            return;
        auto *head = host_->itemsWithMaxHealth_.front();
        while (head)
        {
            const auto *currentItem = head->data;
            if (!currentItem)
                break;

            const auto &state = currentItem->state();
            if (state.health() > health)
                break;
            head = head->next;
        }
        const auto &atkInfo = host_->attackInfo();
        if (!head)
        {
            host_->itemsWithMaxHealth_.pushBack(item);
        }
        else
        {
            host_->itemsWithMaxHealth_.insertBefore(head, item);
        }

        while (host_->itemsWithMaxHealth_.size() > atkInfo.attackedCount())
        {
            host_->itemsWithMaxHealth_.removeFirst();
        }
    }

private:
    AttackAutoTargetingMagic *host_;
};

AttackAutoTargetingMagic::AttackAutoTargetingMagic(int key)
    : Attack(key),
      impl_(new AttackAutoTargetingMagicImpl(this)) {}

AttackAutoTargetingMagic::~AttackAutoTargetingMagic()
{
    if (impl_)
    {
        delete impl_;
        impl_ = nullptr;
    }
}

void AttackAutoTargetingMagic::tryBegin(World &world, Layer &layer)
{
    if (active_ || !owner_)
        return;

    const auto &canvas = world.canvas();
    if (triggerKey_ >= 0 && canvas.keyPressed(triggerKey_))
    {
        Attack::begin();
    }
}

void AttackAutoTargetingMagic::end()
{
    if (!active_)
        return;
    Attack::end();
    triggerOnce_ = false;
    itemsWithMaxHealth_.clear();
}

void AttackAutoTargetingMagic::tryHit(World &world, Layer &layer)
{
    auto &mainLayer = dynamic_cast<MainLayer &>(layer);
    auto &playStage = dynamic_cast<PlayStage &>(mainLayer.currentStage());
    auto *gridSystem = playStage.gridSystem();
    if (!gridSystem)
        return;

    const auto &atkInfo = attackInfo();
    if (atkInfo.requiredMana() > owner_->state().mana())
        return;

    if (!preTryHit(world, layer))
        return;

    if (triggerOnce_)
        return;
    triggerOnce_ = true;

    float oldMana = owner_->state().mana();
    owner_->state().setMana(oldMana - atkInfo.requiredMana());

    auto aabbCtn = impl_->getAttackContainer();
    GEB::Structures::Array<Entity *> candidateItemList;
    gridSystem->getCandidateListByAABB(owner_, aabbCtn, candidateItemList);

    if ((candidateItemList.size() == 0))
        return;

    for (int j = 0; j < candidateItemList.size(); ++j)
    {
        if (!candidateItemList[j])
            continue;

        auto *entity = dynamic_cast<ItemEntity *>(candidateItemList[j]);
        if (!entity || !(entity->active()))
            continue;
        std::uintptr_t entityPtr = reinterpret_cast<std::uintptr_t>(entity);
        if (hitTargets_.contains(entityPtr))
            continue;

        auto owerCenter = owner_->center();
        auto targetCenter = entity->center();

        auto offsetVec = targetCenter - owerCenter;

        auto distanceSquared = offsetVec.x * offsetVec.x + offsetVec.y * offsetVec.y;

        if (distanceSquared > attackInfo().attackRadius() * attackInfo().attackRadius())
            continue;

        impl_->addItemToItemsInorder(entity, entity->state().health());
    }

    auto *head = itemsWithMaxHealth_.front();
    while (head)
    {
        if (!(head->data))
        {
            head = head->next;
            continue;
        }
        hitTarget(*head->data, atkInfo);
        head = head->next;
    }
}

// todo: remove
void AttackAutoTargetingMagic::render(World &world, Layer &layer)
{
    if (!isActive() || !triggerOnce_)
        return;

    const auto &info = attackInfo();
    if (elapsedTime_ < info.hitStartTime() || elapsedTime_ >= info.hitEndTime())
        return;

    auto &canvas = world.canvas();
    const auto &viewport = world.gameInfo().viewport();
    const int visibleWidth = std::min(viewport.width(), static_cast<int>(canvas.getWidth()));
    const int visibleHeight = std::min(viewport.height(), static_cast<int>(canvas.getHeight()));
    const float borderWidth = 3.0f;

    auto *head = itemsWithMaxHealth_.front();
    while (head)
    {
        auto *target = head->data;
        head = head->next;
        if (!target || !target->active() || target->width() <= 0 || target->height() <= 0)
            continue;

        float targetX = target->x() - viewport.x();
        float targetY = target->y() - viewport.y();
        float targetRight = targetX + target->width();
        float targetBottom = targetY + target->height();
        int left = std::max(0, GEB::Math::floorToInt(targetX - borderWidth));
        int right = std::min(visibleWidth, GEB::Math::ceilToInt(targetRight + borderWidth));
        int top = std::max(0, GEB::Math::floorToInt(targetY - borderWidth));
        int bottom = std::min(visibleHeight, GEB::Math::ceilToInt(targetBottom + borderWidth));

        // Keep the orange border outside the body so later entity draws do not cover it.
        for (int y = top; y < bottom; ++y)
        {
            for (int x = left; x < right; ++x)
            {
                if (x >= targetX && x < targetRight && y >= targetY && y < targetBottom)
                    continue;
                canvas.draw(x, y, 255, 140, 0);
            }
        }
    }
}
