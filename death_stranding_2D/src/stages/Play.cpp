#include "stages/Play.h"
#include "core/GameBase.h"
#include "entities/Hero.h"
#include "entities/Swordsman.h"
#include "entities/Archer.h"
#include "entities/Mage.h"
#include "entities/InfoEntity.h"
#include "entities/Monster.h"
#include "map/Map.h"
#include "layers/MainLayer.h"
#include "world/World.h"
#include "skills/SkillSpeedUp.h"
#include "entities/HealthPotion.h"
#include "entities/ManaPotion.h"
#include "entities/Track.h"
#include "skills/AttackCollision.h"

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
    case HeroType::Mage:
        hero_ = new Mage(entrance.x, entrance.y);
        break;
    case HeroType::Swordsman:
    default:
        hero_ = new Swordsman(entrance.x, entrance.y);
        break;
    }
    hero_->addSkill(new SkillSpeedUp('U'));
    addEntityToGridSystem(hero_);
    pushEntity(new InfoEntity(*hero_));
}

GEB::Pair<int, int> getGridIndex(float x, float y)
{
    GEB::Pair<int, int> index;
    int rolIdx = GEB::Math::floorToInt(y / GRID_SIZE);
    int corIdx = GEB::Math::floorToInt(x / GRID_SIZE);

    index.first = rolIdx;
    index.second = corIdx;
    return index;
}

void PlayStage::addEntityToGridSystem(ItemEntity *item)
{
    if (!gridSystem_ || !item)
        return;

    // todo: adjust item's position by checking this position is already occupied.

    auto center = item->center();
    auto indexPair = getGridIndex(center.x, center.y);
    gridSystem_->addEntity(indexPair.first, indexPair.second, item);
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
    for (int i = 0; i < entitiesNextFrame_.size(); ++i)
        addEntityToGridSystem(entitiesNextFrame_[i]);
    entitiesNextFrame_.clear();

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
