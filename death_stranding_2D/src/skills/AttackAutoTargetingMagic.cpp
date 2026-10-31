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
        auto center = host_->owner_->center();

        const float radius = atkInfo.attackRadius();
        auto aabbX = center.x - radius;
        auto aabbY = center.y - radius;

        return AABB(
            static_cast<int>(aabbX),
            static_cast<int>(aabbY),
            2 * radius,
            2 * radius);
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
    if (canvas.keyPressed(triggerKey_))
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

    auto &mainLayer = dynamic_cast<MainLayer &>(layer);
    auto &playStage = dynamic_cast<PlayStage &>(mainLayer.currentStage());
    auto &gridSystem = playStage.gridSystem();

    auto aabbCtn = impl_->getAttackContainer();
    GEB::Structures::Array<Entity *> candidateItemList;
    gridSystem.getCandidateListByAABB(owner_, aabbCtn, candidateItemList);

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
        head->data->OnAttacked(atkInfo);
        head = head->next;
    }
}

void AttackAutoTargetingMagic::render(World &world, Layer &layer) {}
