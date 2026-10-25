#include "stages/Play.h"
#include "core/GameBase.h"
#include "entities/Hero.h"
#include "world/Viewport.h"
#include "entities/Monster.h"
#include "map/Map.h"
#include "map/MapFactory.h"

PlayStage::PlayStage() : gridSystem_(GRID_ROWCNT, GRID_COLCNT, GRID_SIZE)
{
    // add map
    auto map = new Map(MapType::Chapter1);

    pushEntity(map);

    // add Items
    // [new] items will be recycled by the grid system
    auto hero = new Hero(
        START_POSITION_X,
        START_POSITION_Y,
        ROLE_SIZE,
        ROLE_SIZE,
        PIXEL_PER_SEC);
    hero->setColor(255, 0, 0);
    addEntityToGridSystem(hero);

    // for (int i = 0; i < 10; ++i)
    // {
    //     auto monster = new Beast(
    //         200 + i * (ROLE_SIZE + 10),
    //         200 + i * (ROLE_SIZE + 10),
    //         ROLE_SIZE,
    //         ROLE_SIZE);
    //     monster->setColor(0, 255, 0);
    //     addEntityToGridSystem(monster);
    // }
}

GEB::Pair<int, int> getGridIndex(float x, float y)
{
    GEB::Pair<int, int> index;
    int rolIdx = static_cast<int>(y) / GRID_SIZE;
    int corIdx = static_cast<int>(x) / GRID_SIZE;

    index.first = rolIdx;
    index.second = corIdx;
    return index;
}

void PlayStage::addEntityToGridSystem(ItemEntity *item)
{
    auto center = item->center();
    auto indexPair = getGridIndex(center.x, center.y);
    gridSystem_.addEntityToGrid(indexPair.first, indexPair.second, item);
}

// updateEntities
void PlayStage::updateEntities(World &world, Layer &layer)
{
    Stage::updateEntities(world, layer);

    auto &itemsToUpdate = gridSystem_.itemsToUpdate();
    int itemCount = itemsToUpdate.size();
    for (int i = 0; i < itemCount; ++i)
    {
        auto *item = itemsToUpdate[i];
        if (item)
            item->update(world, layer);
    }
}

// renderEntities
void PlayStage::renderEntities(World &world, Layer &layer)
{
    Stage::renderEntities(world, layer);
    auto &itemsToUpdate = gridSystem_.itemsToUpdate();
    int itemCount = itemsToUpdate.size();
    for (int i = 0; i < itemCount; ++i)
    {
        auto *item = itemsToUpdate[i];
        if (item)
            item->render(world, layer);
    }
}
