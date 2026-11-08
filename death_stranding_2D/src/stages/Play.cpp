#include "stages/Play.h"
#include "core/GameBase.h"
#include "entities/Hero.h"
#include "entities/Swordsman.h"
#include "entities/Archer.h"
#include "entities/InfoEntity.h"
#include "entities/Monster.h"
#include "entities/MonsterNest.h"
#include "entities/Key.h"
#include "entities/Door.h"
#include "map/Map.h"
#include "layers/MainLayer.h"
#include "world/World.h"
#include "entities/HealthPotion.h"
#include "entities/ManaPotion.h"
#include <algorithm>
#include <stdexcept>

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

    initializeMapMonsters();
    initializeMapObjects();
}

void PlayStage::initializeMapObjects()
{
    if (door_)
        door_->setActive(false);
    key_ = nullptr;
    door_ = nullptr;
    hero_->useKey();

    const auto *exit = map_->exit();
    auto center = map_->center();
    // The last chapter's door leads to the tally page.
    GEB::Vec2 doorPosition = exit ? exit->position : GEB::Vec2{static_cast<float>(map_->width() - 128), center.y};

    // Keep the door inside the map, at the exit.
    float x = std::clamp(doorPosition.x - 64.0f, 0.0f, static_cast<float>(map_->width() - 128));
    float y = std::clamp(doorPosition.y - 32.0f, 0.0f, static_cast<float>(map_->height() - 64));
    door_ = new Door(*hero_, x, y);
    pushEntity(door_);
}

void PlayStage::initializeMapMonsters()
{
    const auto &positions = map_->monsterPositions();
    for (int i = 0; i < positions.size(); ++i)
    {
        const auto position = positions[i];
        ItemEntity *monster = nullptr;
        if (map_->type() == MapType::Chapter2)
            monster = new MonsterNest(position.x, position.y);
        else
            monster = new Beast(position.x, position.y);
        if (!canPlaceEntity(*monster, position.x, position.y) || !addEntityToGridSystem(monster))
        {
            delete monster;
            throw std::runtime_error("Cannot place monster at its configured map position");
        }
    }
}

bool PlayStage::isHeroAlive() const
{
    return hero_ && hero_->active() && hero_->state().health() > 0.0f;
}

void PlayStage::finishRun(World &world, Layer &layer, bool victory)
{
    gameOver_ = true;
    changeMapFlag_ = false;
    // Preserve the final values before the stage and its hero are destroyed.
    world.gameInfo().setRunResult({victory, heroType_,
                                   hero_ ? hero_->defeatedMonsterCount() : 0,
                                   hero_ ? static_cast<int>(hero_->state().score()) : 0});
    dynamic_cast<MainLayer &>(layer).goToNextStageInNextFrame();
}

bool PlayStage::monsterCleared() const
{
    const auto &items = gridSystem_->itemsToUpdate();
    for (int i = 0; i < items.size(); ++i)
    {
        const auto *monster = dynamic_cast<const Monster *>(items[i]);
        if (monster && monster->active())
            return false;
        const auto *nest = dynamic_cast<const MonsterNest *>(items[i]);
        if (nest && nest->active())
            return false;
    }
    // Consider monsters that appear next frame as well.
    for (int i = 0; i < entitiesNextFrame_.size(); ++i)
        if (dynamic_cast<const Monster *>(entitiesNextFrame_[i]))
            return false;
    return true;
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
    const float startX = std::clamp(item.x(), 0.0f, static_cast<float>(map_->width() - item.width()));
    const float startY = std::clamp(item.y(), 0.0f, static_cast<float>(map_->height() - item.height()));

    // Try the requested position, then the surrounding squares from near to far.
    for (int radius = 0; radius <= 2; ++radius)
    {
        for (int row = -radius; row <= radius; ++row)
        {
            for (int col = -radius; col <= radius; ++col)
            {
                if (std::abs(row) != radius && std::abs(col) != radius)
                    continue;
                position = {startX + col * ROLE_SIZE, startY + row * ROLE_SIZE};
                if (canPlaceEntity(item, position.x, position.y))
                    return true;
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

void PlayStage::update(World &world, Layer &layer)
{
    if (gameOver_)
        return;
    Stage::enterState.justPressed(world.canvas().keyPressed(VK_RETURN));
    world.gameInfo().updateMapInfo(*map_);

    // update entities
    updateEntities(world, layer);

    // check hero death
    if (checkHeroDeath(world, layer))
        return;

    // update chapter progress
    updateChapterProgress(world, layer);
}

void PlayStage::updateEntities(World &world, Layer &layer)
{
    auto &items = gridSystem_->itemsToUpdate();
    // Stop the remaining entities from updating as soon as the hero dies.
    for (int i = 0; i < items.size() && isHeroAlive(); ++i)
        items[i]->update(world, layer);

    Stage::update(world, layer);
}

bool PlayStage::checkHeroDeath(World &world, Layer &layer)
{
    if (isHeroAlive())
        return false;

    finishRun(world, layer, false);
    return true;
}

void PlayStage::updateChapterProgress(World &world, Layer &layer)
{
    if (!monsterCleared())
        return;

    // Generate key if all monsters are defeated.
    if (!key_)
    {
        key_ = new Key(*hero_, *map_);
        pushEntity(key_);
    }

    // Check door status
    if (!door_ || !door_->unlocked())
        return;

    // To be here, all monsters are defeated and the door is unlocked.
    if (map_->type() == MapType::Chapter2)
        finishRun(world, layer, true);
    else
        changeMapFlag_ = true;
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
    if (!gridSystem_ || gameOver_)
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
    initializeMapMonsters();
    initializeMapObjects();

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
