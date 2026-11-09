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

    // Update active skills, movement, and attacks for this frame.
    processSkill(world, layer);
    handleMovement(world, layer);
    processAttack(world, layer);

    // Start new actions last. Their timers begin updating next frame.
    tryTriggerAttacks(world, layer);
    tryTriggerSkills(world, layer);
}

void ItemEntity::render(World &world, Layer &layer)
{
    if (!active())
        return;

    // Draw the entity using its assigned sprite, if any.
    auto asset = spriteAsset();
    if (asset != AssetId::None)
        renderSprite(world, asset);

    renderStateInfo(world);

    // Draw active attack effects after the entity.
    for (int i = 0; i < attacks_.size(); ++i)
    {
        auto *attack = attacks_[i];
        if (!attack)
            continue;
        if (!attack->isActive())
            continue;
        attack->render(world, layer);
    }
}

void ItemEntity::renderStateInfo(World &world)
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

    // draw state
    if (state_.health() >= 0)
    {
        auto &textRenderer = world.textRenderer();
        auto targetX = static_cast<int>(xRelativeToViewport);
        auto targetY = static_cast<int>(yRelativeToViewport);
        std::string healthText = "HP: " + std::to_string(static_cast<int>(state_.health()));
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
        std::string manaText = "MP: " + std::to_string(static_cast<int>(state_.mana()));
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
    y_ = std::max(resPos, topBoundary);
}
void ItemEntity::moveDown(float distance, float bottomBoundary)
{
    float resPos = y_ + distance;
    y_ = std::min(resPos, bottomBoundary - height_);
}
void ItemEntity::moveLeft(float distance, float leftBoundary)
{
    float resPos = x_ - distance;
    x_ = std::max(resPos, leftBoundary);
}
void ItemEntity::moveRight(float distance, float rightBoundary)
{
    float resPos = x_ + distance;
    x_ = std::min(resPos, rightBoundary - width_);
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
    // Start with map edges, then limit movement using nearby obstacles.
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
    GEB::Vec2 delta,
    const MapInfo &map,
    Entity **items,
    int size)
{
    // Move on each axis separately, so the entity can slide along walls.
    if (delta.y < 0)
        moveUp(-delta.y, calculateBoundary(map, GEB::Direction::Up, items, size).top);
    else if (delta.y > 0)
        moveDown(delta.y, calculateBoundary(map, GEB::Direction::Down, items, size).bottom);

    if (delta.x < 0)
        moveLeft(-delta.x, calculateBoundary(map, GEB::Direction::Left, items, size).left);
    else if (delta.x > 0)
        moveRight(delta.x, calculateBoundary(map, GEB::Direction::Right, items, size).right);
}

bool ItemEntity::deactivateIfNeeded(Layer &layer)
{
    if (!active_)
    {
        // Queue removal instead of changing the list during updates.
        auto &mainLayer = dynamic_cast<MainLayer &>(layer);
        auto &playStage = dynamic_cast<PlayStage &>(mainLayer.currentStage());
        auto *gridSystem = playStage.gridSystem();
        if (!gridSystem)
            return true;

        gridSystem->addInactiveItem(this);

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

void ItemEntity::tryTriggerAttacks(World &world, Layer &layer)
{
    for (int i = 0; i < attacks_.size(); ++i)
    {
        auto *attack = attacks_[i];
        if (!attack)
            continue;
        attack->tryBegin(world, layer);
    }
}

void ItemEntity::tryTriggerSkills(World &world, Layer &layer)
{
    auto &canvas = world.canvas();
    for (int i = 0; i < skills_.size(); ++i)
    {
        if (!skills_[i])
            continue;
        skills_[i]->tryActivate(world, layer, state_);
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

void ItemEntity::handleMovement(World &world, Layer &layer) {}

void ItemEntity::onAttacked(const Attack *atk, const AttackInfo &attackInfo)
{
    // Defense reduces damage, but damage cannot go below zero.
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

int spriteFrame(GEB::Direction direction)
{
    switch (direction)
    {
    case GEB::Direction::Up:
        return 0;
    case GEB::Direction::UpRight:
        return 1;
    case GEB::Direction::Right:
        return 2;
    case GEB::Direction::DownRight:
        return 3;
    case GEB::Direction::Down:
        return 4;
    case GEB::Direction::DownLeft:
        return 5;
    case GEB::Direction::Left:
        return 6;
    case GEB::Direction::UpLeft:
        return 7;
    default:
        return 4;
    }
}

void ItemEntity::renderSprite(World &world, AssetId asset)
{
    auto &canvas = world.canvas();
    auto *sheet = world.assetManager().getImage(asset);
    if (!sheet)
        return;

    auto *buffer = canvas.getBackBuffer();
    if (!buffer)
        return;

    // Validate the eight-direction sheet and target size.
    int frameSize = ROLE_SIZE;
    int sheetColumns = 4;
    int sheetRows = 2;
    if (sheet->width != frameSize * sheetColumns)
        return;
    if (sheet->height != frameSize * sheetRows)
        return;

    int width = width_;
    int height = height_;

    // Select the frame for the current direction.
    int frame = spriteFrame(direction_);
    int sourceLeft = (frame % sheetColumns) * frameSize;
    int sourceTop = (frame / sheetColumns) * frameSize;

    // Convert world coordinates to screen coordinates.
    auto &viewport = world.gameInfo().viewport();
    int left = static_cast<int>(x_ - viewport.x());
    int top = static_cast<int>(y_ - viewport.y());
    int canvasWidth = static_cast<int>(canvas.getWidth());
    int visibleWidth = std::min(viewport.width(), canvasWidth);
    int visibleHeight = std::min(viewport.height(), static_cast<int>(canvas.getHeight()));

    // Clip drawing to the visible area.
    int drawLeft = std::max(0, left);
    int drawTop = std::max(0, top);
    int drawRight = std::min(visibleWidth, left + width);
    int drawBottom = std::min(visibleHeight, top + height);
    if (drawLeft >= drawRight)
        return;
    if (drawTop >= drawBottom)
        return;

    // Draw the sprite frame for the current direction.
    for (int y = drawTop; y < drawBottom; ++y)
    {
        int sourceY = sourceTop + (y - top) * frameSize / height;
        for (int x = drawLeft; x < drawRight; ++x)
        {
            int sourceX = sourceLeft + (x - left) * frameSize / width;
            int alpha = sheet->alphaAt(sourceX, sourceY);
            auto *pixel = sheet->at(sourceX, sourceY);
            auto *destination = buffer + (y * canvasWidth + x) * 3;
            GEB::Utils::blendPixel(destination, pixel, alpha);
        }
    }
}
