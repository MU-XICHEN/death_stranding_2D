#include "stages/Play.h"
#include "core/GameBase.h"
#include "entities/Hero.h"
#include "world/Viewport.h"
#include "entities/Monster.h"
#include "map/Map.h"
#include "map/MapFactory.h"

PlayStage::PlayStage()
    : gridSystem_(GRID_ROWCNT, GRID_COLCNT, GRID_SIZE),
      map_(MapType::Chapter1)
{
    auto heroPosition = map_.playerPosition();

    // add Items
    // [new] items will be recycled by the grid system
    auto hero = new Hero(
        heroPosition.x,
        heroPosition.y,
        ROLE_SIZE,
        ROLE_SIZE,
        PIXEL_PER_SEC);

    hero->setColor(255, 0, 0);
    addEntityToGridSystem(hero);

    auto monster = new Beast(
        heroPosition.x + 100,
        heroPosition.y + 100,
        ROLE_SIZE,
        ROLE_SIZE);
    monster->setColor(0, 255, 0);
    addEntityToGridSystem(monster);

    auto monster2 = new Beast(
        heroPosition.x - 100,
        heroPosition.y - 100,
        ROLE_SIZE,
        ROLE_SIZE);
    monster2->setColor(0, 255, 0);
    addEntityToGridSystem(monster2);
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
    auto center = item->center();
    auto indexPair = getGridIndex(center.x, center.y);
    gridSystem_.addEntityToGrid(indexPair.first, indexPair.second, item);
}

// updateEntities
void PlayStage::updateEntities(World &world, Layer &layer)
{
    map_.update(world, layer);

    Stage::updateEntities(world, layer); // update sections like information panels

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
    map_.render(world, layer);

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
