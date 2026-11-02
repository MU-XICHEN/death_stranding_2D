#include <string>

#include "entities/ItemEntity.h"
#include "GamesEngineeringBase.h"
#include "world/World.h"
#include "map/Map.h"
#include "layers/Layer.h"
#include "layers/MainLayer.h"
#include "stages/Play.h"
#include "entities/AABB.h"
#include "skills/Attack.h"
#include "structure/Array.h"
#include "entities/Potion.h"

ItemEntity::~ItemEntity()
{
    for (int i = 0; i < attacks_.size(); i++)
    {
        if (attacks_[i])
            delete attacks_[i];
        attacks_[i] = nullptr;
    }
    for (int i = 0; i < skills_.size(); i++)
    {
        if (skills_[i])
            delete skills_[i];
        skills_[i] = nullptr;
    }
}

void ItemEntity::coreUpdate(World &world, Layer &layer)
{
    if (deactivateIfNeeded(layer))
        return;

    processSkill(world, layer);   // update items' skill states of this frame
    handleMovement(world, layer); // handle movement of this frame
    processAttack(world, layer);  // update items' attack states of this frame

    // attack start this frame, so elapsedTime in Attack should be calculated next frame
    tryTriggerAttacks(world, layer); // trigger skill for next frame
    tryTriggerSkills(world, layer);  // trigger skills for next frame
}

void ItemEntity::render(World &world, Layer &layer)
{
    const Viewport &viewport = world.gameInfo().viewport();
    auto &canvas = world.canvas();
    const int visibleWidth = viewport.width();
    const int visibleHeight = viewport.height();

    float xRelativeToViewport = x_ - viewport.x();
    float yRelativeToViewport = y_ - viewport.y();

    if (xRelativeToViewport >= visibleWidth || yRelativeToViewport >= visibleHeight ||
        xRelativeToViewport + width_ <= 0 || yRelativeToViewport + height_ <= 0)
        return;

    for (int y = 0; y < height_; y++)
    {
        for (int x = 0; x < width_; x++)
        {
            float targetX = xRelativeToViewport + x;
            float targetY = yRelativeToViewport + y;

            if (targetX < 0 || targetY < 0 ||
                targetX >= visibleWidth || targetY >= visibleHeight)
                continue;

            canvas.draw(static_cast<int>(targetX),
                        static_cast<int>(targetY),
                        color_.r,
                        color_.g,
                        color_.b);
        }
    }

    // draw attacks effects
    for (int i = 0; i < attacks_.size(); i++)
    {
        if (!attacks_[i] || !(attacks_[i]->isActive()))
            continue;
        attacks_[i]->render(world, layer);
    }

    // draw state
    if (state_.health() >= 0)
    {
        auto &textRenderer = world.textRenderer();
        auto targetX = static_cast<int>(xRelativeToViewport);
        auto targetY = static_cast<int>(yRelativeToViewport);
        std::string healthText = std::to_string(static_cast<int>(state_.health()));
        textRenderer.drawString(
            world,
            healthText.c_str(),
            targetX,
            targetY,
            GEB::TextSizeType::TEXT_SIZE_SMALL);
    }
    if (state_.mana() >= 0)
    {
        auto &textRenderer = world.textRenderer();
        auto targetX = static_cast<int>(xRelativeToViewport);
        auto targetY = static_cast<int>(yRelativeToViewport) + GEB::TextSizeH::TEXT_SIZE_H_SMALL + ROLE_HP_TEXT_SEP;
        std::string manaText = std::to_string(static_cast<int>(state_.mana()));
        textRenderer.drawString(
            world,
            manaText.c_str(),
            targetX,
            targetY,
            GEB::TextSizeType::TEXT_SIZE_SMALL);
    }
}

// Movement functions
void ItemEntity::moveUp(float distance, float topBoundary)
{
    float resPos = y_ - distance;
    y_ = std::min(y_, std::max(resPos, topBoundary));
}
void ItemEntity::moveDown(float distance, float bottomBoundary)
{
    float resPos = y_ + distance;
    y_ = std::max(y_, std::min(resPos, bottomBoundary - height_));
}
void ItemEntity::moveLeft(float distance, float leftBoundary)
{
    float resPos = x_ - distance;
    x_ = std::min(x_, std::max(resPos, leftBoundary));
}
void ItemEntity::moveRight(float distance, float rightBoundary)
{
    float resPos = x_ + distance;
    x_ = std::max(x_, std::min(resPos, rightBoundary - width_));
}

void ItemEntity::calculateBoundaryTop(GEB::Boundary &boundary, const Entity *barrierItem)
{
    const float barrierRight = barrierItem->x() + barrierItem->width();
    const float barrierBottom = barrierItem->y() + barrierItem->height();
    // half-open intervals [min, max)
    if (barrierRight <= x_ || barrierItem->x() >= x_ + width_)
        return;

    if (barrierBottom <= y_)
        boundary.top = std::max(boundary.top, barrierBottom);
}

void ItemEntity::calculateBoundaryBottom(GEB::Boundary &boundary, const Entity *barrierItem)
{
    const float barrierRight = barrierItem->x() + barrierItem->width();

    if (barrierRight <= x_ || barrierItem->x() >= x_ + width_)
        return;

    if (barrierItem->y() >= y_ + height_)
        boundary.bottom = std::min(boundary.bottom, barrierItem->y());
}

void ItemEntity::calculateBoundaryLeft(GEB::Boundary &boundary, const Entity *barrierItem)
{
    const float barrierRight = barrierItem->x() + barrierItem->width();
    const float barrierBottom = barrierItem->y() + barrierItem->height();
    if (barrierBottom <= y_ || barrierItem->y() >= y_ + height_)
        return;

    if (barrierRight <= x_)
        boundary.left = std::max(boundary.left, barrierRight);
}

void ItemEntity::calculateBoundaryRight(GEB::Boundary &boundary, const Entity *barrierItem)
{
    const float barrierBottom = barrierItem->y() + barrierItem->height();
    if (barrierBottom <= y_ || barrierItem->y() >= y_ + height_)
        return;

    if (barrierItem->x() >= x_ + width_)
        boundary.right = std::min(boundary.right, barrierItem->x());
}

void ItemEntity::calculateBoundaryImpl(GEB::Boundary &boundary, const Entity *barrierItem, GEB::Direction direction)
{
    switch (direction)
    {
    case GEB::Direction::Up:
        calculateBoundaryTop(boundary, barrierItem);
        break;
    case GEB::Direction::Down:
        calculateBoundaryBottom(boundary, barrierItem);
        break;
    case GEB::Direction::Left:
        calculateBoundaryLeft(boundary, barrierItem);
        break;
    case GEB::Direction::Right:
        calculateBoundaryRight(boundary, barrierItem);
        break;
    case GEB::Direction::None:
        break;
    default:
        break;
    }
}

GEB::Boundary ItemEntity::calculateBoundary(const MapInfo &map, GEB::Direction diretion, Entity **candidateItemHead, int size)
{
    GEB::Boundary boundary{
        .top = map.topBoundary(),
        .bottom = map.bottomBoundary(),
        .left = map.leftBoundary(),
        .right = map.rightBoundary()};

    for (int i = 0; i < size; i++)
    {
        if (!(*(candidateItemHead + i)) || *(candidateItemHead + i) == this)
            continue;
        calculateBoundaryImpl(boundary, *(candidateItemHead + i), diretion);
    }

    return boundary;
}

void ItemEntity::moveWithCollision(
    GEB::Vec2 distanceVec,
    const MapInfo &map,
    Entity **candidateItemHead,
    int size)
{
    // calculate boundaries horizontally and vertically, respectively, in case of diagonal movement
    if (distanceVec.y < 0)
        moveUp(-distanceVec.y, calculateBoundary(map, GEB::Direction::Up, candidateItemHead, size).top);
    else if (distanceVec.y > 0)
        moveDown(distanceVec.y, calculateBoundary(map, GEB::Direction::Down, candidateItemHead, size).bottom);

    if (distanceVec.x < 0)
        moveLeft(-distanceVec.x, calculateBoundary(map, GEB::Direction::Left, candidateItemHead, size).left);
    else if (distanceVec.x > 0)
        moveRight(distanceVec.x, calculateBoundary(map, GEB::Direction::Right, candidateItemHead, size).right);
}

bool ItemEntity::deactivateIfNeeded(Layer &layer)
{
    if (!active_)
    {
        // inactive
        auto &mainLayer = dynamic_cast<MainLayer &>(layer);
        auto &playStage = dynamic_cast<PlayStage &>(mainLayer.currentStage());
        auto &gridSystem = playStage.gridSystem();

        gridSystem.addInactiveItem(this);

        return true;
    }
    return false;
}

void ItemEntity::processSkill(World &world, Layer &layer)
{
    auto dt = world.gameInfo().dt();

    for (int i = 0; i < skills_.size(); i++)
    {
        auto *skill = skills_[i];
        if (!skill)
            continue;

        if (!skill->active())
            continue;

        skill->onProcessed(dt, state_);
    }
}

void ItemEntity::processAttack(World &world, Layer &layer)
{
    for (int i = 0; i < attacks_.size(); i++)
    {
        auto *attack = attacks_[i];
        if (!attack)
            continue;
        if (!attack->isActive())
            continue;
        attack->tryHit(world, layer);
    }
}

void ItemEntity::OnAttacked(const AttackInfo &attackInfo)
{
    int realDamage = static_cast<int>(attackInfo.damage() - state_.defense());
    if (realDamage < 0)
        realDamage = 0;
    state_.setHealth(state_.health() - realDamage);

    if (state_.health() <= 0)
        setActive(false);
}

void ItemEntity::addAttack(Attack *attack)
{
    attack->setOwner(this);
    attacks_.pushBack(attack);
}

void ItemEntity::addSkill(Skill *skill)
{
    skill->setOwner(this);
    skills_.pushBack(skill);
}

void ItemEntity::initializeState(GEB::Pair<int, float> *newProp,
                                 int size)
{
    state_.initialize(newProp, size);
}

bool ItemEntity::onUsePotion(Potion *potion)
{
    // If entity consumed the potion, return true;
    return false;
}