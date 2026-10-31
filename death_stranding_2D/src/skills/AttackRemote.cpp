#include "skills/AttackRemote.h"
#include "world/World.h"
#include "utils/Utils.h"
#include "stages/Play.h"
#include "layers/MainLayer.h"

void AttackRemote::tryBegin(World &world, Layer &layer)
{
    if (active_ || !owner_)
        return;

    const auto &canvas = world.canvas();
    if (canvas.keyPressed(triggerKey_))
    {
        Attack::begin();

        direction_ = attackInfo_.direction();
        startPos_ = attackInfo_.originPoint();
        desPos_ = attackInfo_.originPoint();
    }
}

void AttackRemote::end()
{
    if (!active_)
        return;
    Attack::end();
    direction_ = GEB::Direction::None;
    startPos_ = {0, 0};
    desPos_ = {0, 0};
}

void AttackRemote::tryHit(World &world, Layer &layer)
{
    if (!preTryHit(world, layer))
        return;

    startPos_ = desPos_; // update startPos_ for current frame.

    // out of viewport
    auto viewport = world.gameInfo().viewport();
    if (startPos_.x < viewport.x() || startPos_.x > viewport.x() + viewport.width() ||
        startPos_.y < viewport.y() || startPos_.y > viewport.y() + viewport.height())
    {
        end();
        return;
    }

    auto &mainLayer = dynamic_cast<MainLayer &>(layer);
    auto &playStage = dynamic_cast<PlayStage &>(mainLayer.currentStage());
    auto &gridSystem = playStage.gridSystem();

    AABB attackContainer = getAttackContainer(world, layer);
    // pointers in list do not need to be owned, just references to existing entities, like 'this'.
    GEB::Structures::Array<Entity *> candidateItemList;
    gridSystem.getCollisionCandidateListByAABB(owner_, attackContainer, candidateItemList);
    // add map tiles which may collide with the hero to the candidate list
    playStage.map().addCollisionTileToCandiListByAABB(owner_,
                                                      attackContainer,
                                                      candidateItemList);

    const auto &atkInfo = attackInfo();
    float speed = atkInfo.rangedAttackSpeed();
    float dt = world.gameInfo().dt();
    float distance = speed * dt;
    desPos_ = GEB::Utils::getDestination(startPos_, direction_, distance);

    if ((candidateItemList.size() == 0))
        return;

    // try hit
    GEB::Structures::Array<Entity *> hitEntities;
    for (int j = 0; j < candidateItemList.size(); ++j)
    {
        auto *entity = dynamic_cast<Entity *>(candidateItemList[j]);
        if (!entity)
            continue;

        GEB::Vec2 entityHitBox[4];
        GEB::Utils::getRectByEntity(entity, entityHitBox);

        if (GEB::Utils::SATLineBox(startPos_, desPos_, entityHitBox))
        {
            hitEntities.pushBack(entity);
        }
    }

    if (hitEntities.size() == 0)
        return;

    int targetIndex = -1;
    float squaredDist = std::numeric_limits<float>::max();
    for (int i = 0; i < hitEntities.size(); ++i)
    {
        const auto *entity = hitEntities[i];
        AABB aabb{
            entity->x(),
            entity->y(),
            entity->width(),
            entity->height()};

        GEB::Vec2 ips[2];
        int count = GEB::Utils::getIntersectionPoints(startPos_, desPos_ - startPos_, aabb, ips);
        if (count > 0)
        {
            /**
             * [bugfix]: consider two points
             *  It could fail if startPos was already inside the AABB, while only checking one point.
             *  So, select the nearest intersection point to the start position.
             */
            auto vector1 = (ips[0] - startPos_);
            float newSquaredDist = vector1.x * vector1.x + vector1.y * vector1.y;

            if (count == 2)
            {
                auto vector2 = (ips[1] - startPos_);
                float newSquaredDist2 = vector2.x * vector2.x + vector2.y * vector2.y;
                newSquaredDist = std::min(newSquaredDist, newSquaredDist2);
            }

            if (newSquaredDist < squaredDist)
            {
                targetIndex = i;
                squaredDist = newSquaredDist;
            }
        }
    }
    if (targetIndex != -1 && hitEntities[targetIndex] != nullptr)
    {
        // hit characters
        if (auto *itemEntity = dynamic_cast<ItemEntity *>(hitEntities[targetIndex]))
        {
            itemEntity->OnAttacked(atkInfo);
        }

        end();
    }
}

AABB AttackRemote::getAttackContainer(World &world, Layer &layer) const
{
    const auto &attackInfo = this->attackInfo();
    float speed = attackInfo.rangedAttackSpeed();
    float dt = world.gameInfo().dt();
    float distance = speed * dt;
    GEB::Vec2 desPos = GEB::Utils::getDestination(startPos_, direction_, distance);
    return AABB::createAABB(startPos_, desPos);
}

// todo: remove
void AttackRemote::render(World &world, Layer &layer)
{
    if (!isActive())
        return;

    const auto &info = attackInfo();
    if (elapsedTime_ < info.hitStartTime() || elapsedTime_ >= info.hitEndTime())
        return;

    GEB::Vec2 movement = desPos_ - startPos_;
    if (movement.x == 0.0f && movement.y == 0.0f)
        return;

    auto &canvas = world.canvas();
    const auto &viewport = world.gameInfo().viewport();
    const int visibleWidth = std::min(viewport.width(), static_cast<int>(canvas.getWidth()));
    const int visibleHeight = std::min(viewport.height(), static_cast<int>(canvas.getHeight()));

    GEB::Vec2 center = (startPos_ + desPos_) * 0.5f;
    center.x -= viewport.x();
    center.y -= viewport.y();
    GEB::Vec2 direction = GEB::Math::normalize(movement);
    GEB::Vec2 sideDirection = GEB::Math::perpendicular(direction);

    // Keep short per-frame segments visible, centered on the collision segment.
    const float halfLength = std::max(GEB::Math::dotProduct(movement, direction) * 0.5f, 6.0f);
    const float halfWidth = 4.0f;
    float extentX = GEB::Math::abs(direction.x) * halfLength + GEB::Math::abs(sideDirection.x) * halfWidth;
    float extentY = GEB::Math::abs(direction.y) * halfLength + GEB::Math::abs(sideDirection.y) * halfWidth;
    int left = std::max(0, GEB::Math::floorToInt(center.x - extentX));
    int right = std::min(visibleWidth, GEB::Math::ceilToInt(center.x + extentX));
    int top = std::max(0, GEB::Math::floorToInt(center.y - extentY));
    int bottom = std::min(visibleHeight, GEB::Math::ceilToInt(center.y + extentY));

    for (int y = top; y < bottom; ++y)
    {
        for (int x = left; x < right; ++x)
        {
            GEB::Vec2 offset{x + 0.5f - center.x, y + 0.5f - center.y};
            float forwardDistance = GEB::Math::dotProduct(offset, direction);
            float sideDistance = GEB::Math::dotProduct(offset, sideDirection);
            if (GEB::Math::abs(forwardDistance) > halfLength ||
                GEB::Math::abs(sideDistance) > halfWidth)
                continue;

            if (GEB::Math::abs(sideDistance) <= 1.0f)
                canvas.draw(x, y, 255, 255, 255);
            else
                canvas.draw(x, y, 255, 220, 0);
        }
    }
}
