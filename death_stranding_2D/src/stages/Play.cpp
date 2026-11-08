#include "stages/Play.h"
#include "core/GameBase.h"
#include "entities/Hero.h"
#include "entities/Swordsman.h"
#include "entities/Archer.h"
#include "entities/InfoEntity.h"
#include "entities/Monster.h"
#include "entities/MonsterNest.h"
#include "map/Map.h"
#include "layers/MainLayer.h"
#include "world/World.h"
#include "skills/SkillCreateMonster.h"
#include "entities/HealthPotion.h"
#include "entities/ManaPotion.h"
#include "entities/Track.h"
#include "skills/AttackCollision.h"

bool overlaps(const Entity &obstacle, float x, float y, int width, int height);

PlayStage::PlayStage(HeroType heroType)
    : heroType_(heroType)
{
    map_ = new Map(MapType::Chapter1);
    if (!map_)
        return;
    gridSystem_ = new GridSystem(
        (map_->height() + GRID_SIZE - 1) / GRID_SIZE,
        (map_->width() + GRID_SIZE - 1) / GRID_SIZE,
        GRID_SIZE);

    initialize();
}

PlayStage::~PlayStage()
{
    delete map_;
    delete gridSystem_;

    for (int i = 0; i < entitiesNextFrame_.size(); ++i)
    {
        delete entitiesNextFrame_[i];
    }

    entitiesNextFrame_.clear();
}

void PlayStage::initialize()
{
    if (!gridSystem_ || !map_)
        return;

    const auto entrance = map_->entrancePosition(ROLE_SIZE, ROLE_SIZE);
    switch (heroType_)
    {
    case HeroType::Archer:
        hero_ = new Archer(entrance.x, entrance.y);
        break;
    case HeroType::Swordsman:
    default:
        hero_ = new Swordsman(entrance.x, entrance.y);
        break;
    }
    addEntityToGridSystem(hero_);
    pushEntity(new InfoEntity(*hero_));

    float nestX = (map_->width() - ROLE_SIZE) / 2.0f;
    float nestY = (map_->height() - ROLE_SIZE) / 2.0f;
    auto *nest = new MonsterNest(nestX, nestY);
    addEntityNextFrame(nest);
}

bool PlayStage::canPlaceEntity(const ItemEntity &item, float x, float y) const
{
    if (x < 0 || y < 0)
        return false;
    if (x + item.width() > map_->width())
        return false;
    if (y + item.height() > map_->height())
        return false;

    // Check map obstacles.
    auto &tiles = map_->tileBlocks();
    for (int i = 0; i < tiles.size(); ++i)
    {
        if (overlaps(tiles[i], x, y, item.width(), item.height()))
            return false;
    }

    // Check existing entities.
    auto &items = gridSystem_->itemsToUpdate();
    for (int i = 0; i < items.size(); ++i)
    {
        auto *existing = items[i];
        if (!existing || existing == &item)
            continue;
        if (!existing->active())
            continue;
        if (overlaps(*existing, x, y, item.width(), item.height()))
            return false;
    }
    return true;
}

bool PlayStage::findAvailablePosition(const ItemEntity &item, GEB::Vec2 &position) const
{
    if (item.width() <= 0 || item.height() <= 0)
        return false;
    if (item.width() > map_->width() || item.height() > map_->height())
        return false;
    // Start near the entity that requested the spawn, inside the map.
    float startX = item.x();
    float startY = item.y();
    float maxX = static_cast<float>(map_->width() - item.width());
    float maxY = static_cast<float>(map_->height() - item.height());
    startX = std::min(std::max(startX, 0.0f), maxX);
    startY = std::min(std::max(startY, 0.0f), maxY);

    if (canPlaceEntity(item, startX, startY))
    {
        position = {startX, startY};
        return true;
    }

    // Walk each ring clockwise from its top-left corner, up to three rings.
    GEB::Vec2 directions[] = {
        {1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    float step = ROLE_SIZE;
    for (int ring = 1; ring <= 2; ++ring)
    {
        float distance = ring * step;
        GEB::Vec2 candidate{startX - distance, startY - distance};
        for (int side = 0; side < 4; ++side)
        {
            for (int i = 0; i < ring * 2; ++i)
            {
                if (canPlaceEntity(item, candidate.x, candidate.y))
                {
                    position = candidate;
                    return true;
                }

                candidate.x += directions[side].x * step;
                candidate.y += directions[side].y * step;
            }
        }
    }
    return false;
}

bool PlayStage::addEntityToGridSystem(ItemEntity *item)
{
    if (!gridSystem_ || !map_)
        return false;
    if (!item)
        return false;

    GEB::Vec2 position;
    if (!findAvailablePosition(*item, position))
        return false;

    item->setPos(position.x, position.y);
    auto [row, col] = gridSystem_->getGridIdxPair(item->center());
    gridSystem_->addEntity(row, col, item);
    return true;
}

void PlayStage::addEntityNextFrame(ItemEntity *itemEntity)
{
    if (!itemEntity)
        return;

    entitiesNextFrame_.pushBack(itemEntity);
}

// updateEntities
void PlayStage::update(World &world, Layer &layer)
{
    if (!gridSystem_ || !map_)
        return;

    world.gameInfo().updateMapInfo(*map_);

    auto &itemsToUpdate = gridSystem_->itemsToUpdate();
    int itemCount = itemsToUpdate.size();
    for (int i = 0; i < itemCount; ++i)
    {
        auto *item = itemsToUpdate[i];
        if (item)
            item->update(world, layer);
    }

    Stage::update(world, layer);

    changeMapFlag_ = checkChangeChapter();
}

// renderEntities
void PlayStage::render(World &world, Layer &layer)
{
    if (!gridSystem_ || !map_)
        return;

    map_->render(world, layer);

    auto &itemsToUpdate = gridSystem_->itemsToUpdate();
    int itemCount = itemsToUpdate.size();
    for (int i = 0; i < itemCount; ++i)
    {
        auto *item = itemsToUpdate[i];
        if (item)
            item->render(world, layer);
    }

    Stage::render(world, layer);
}

void PlayStage::renderFinished(World &world, Layer &layer)
{
    if (!gridSystem_)
        return;

    auto &inactiveItems = gridSystem_->inactiveItems();
    int inactiveItemCount = inactiveItems.size();
    for (int i = 0; i < inactiveItemCount; ++i)
    {
        auto *item = inactiveItems[i];
        if (!item)
            continue;
        if (item == hero_)
            hero_ = nullptr;
        const auto &center = item->center();
        auto [rowIdx, colIdx] = gridSystem_->getGridIdxPair(center);
        gridSystem_->removeEntity(rowIdx, colIdx, item);
    }

    // clear
    gridSystem_->clearInactiveItems();

    // Newly generated entities begin updating in the next frame.
    for (int i = 0; i < entitiesNextFrame_.size();)
    {
        if (!addEntityToGridSystem(entitiesNextFrame_[i]))
        {
            ++i;
            continue;
        }
        entitiesNextFrame_.removeAt(i);
    }

    changeChapterIfNeed(world);
}

bool PlayStage::checkChangeChapter() const
{
    if (!map_ || !gridSystem_)
        return false;
    if (!hero_ || !hero_->active() || hero_->state().health() <= 0.0f)
        return false;

    const auto *exit = map_->exit();

    if (!exit)
        return false;

    const AABB heroBounds(hero_->x(), hero_->y(), hero_->width(), hero_->height());
    return heroBounds.contains(exit->position);
}

void PlayStage::changeChapterIfNeed(World &world)
{
    if (!changeMapFlag_)
        return;

    auto nextMap = new Map(map_->exit()->destination);
    auto nextGrid = new GridSystem(
        (nextMap->height() + GRID_SIZE - 1) / GRID_SIZE,
        (nextMap->width() + GRID_SIZE - 1) / GRID_SIZE,
        GRID_SIZE);
    const auto entrance = nextMap->entrancePosition(hero_->width(), hero_->height());

    for (int i = 0; i < hero_->attacks().size(); ++i)
    {
        if (auto *attack = hero_->attacks()[i])
            attack->end();
    }

    // Manage the memory of old map and grid system.
    for (int i = 0; i < entitiesNextFrame_.size(); ++i)
        delete entitiesNextFrame_[i];
    entitiesNextFrame_.clear();

    const auto [row, col] = gridSystem_->getGridIdxPair(hero_->center());
    gridSystem_->removeEntity(row, col, hero_);
    delete gridSystem_;
    delete map_;
    // Assign the new map and grid system to the stage.
    map_ = nextMap;
    gridSystem_ = nextGrid;
    hero_->setPos(entrance.x, entrance.y);
    addEntityToGridSystem(hero_);

    // Update related game information after changing the map.
    HeroInfo heroInfo{hero_->x(), hero_->y(), hero_->width(), hero_->height()};
    world.gameInfo().updateMapInfo(*map_);
    world.gameInfo().updateHeroInfo(heroInfo);
    world.gameInfo().updateViewport(heroInfo, world.gameInfo().mapInfo());
    changeMapFlag_ = false;
}

bool overlaps(const Entity &obstacle, float x, float y, int width, int height)
{
    // Touching edges are allowed; only occupied area counts as overlap.
    if (x + width <= obstacle.x())
        return false;
    if (x >= obstacle.x() + obstacle.width())
        return false;
    if (y + height <= obstacle.y())
        return false;
    if (y >= obstacle.y() + obstacle.height())
        return false;
    return true;
}
