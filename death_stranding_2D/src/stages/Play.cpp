#include "stages/Play.h"
#include "core/GameBase.h"
#include "entities/Hero.h"
#include "world/Viewport.h"
#include "entities/Monster.h"
#include "world/Map.h"

PlayStage::PlayStage() : gridSystem_(GRID_ROWCNT, GRID_COLCNT, GRID_SIZE)
{
    // add map
    auto map = new Map(
        0,
        0,
        MAP_WIDTH,
        MAP_HEIGHT,
        GEB::Color(255, 255, 255));

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
    pushEntityToItemEntityList(hero);

    auto beast = new Beast(
        400,
        400,
        ROLE_SIZE,
        ROLE_SIZE);
    beast->setColor(0, 255, 0);
    pushEntityToItemEntityList(beast);
    return;

    constexpr int kMovementClearancePixels = 4;
    const int monsterWidth = ROLE_SIZE;
    const int monsterHeight = ROLE_SIZE;
    const int horizontalGap = hero->width() + kMovementClearancePixels;
    const int verticalGap = hero->height() + kMovementClearancePixels;

    // Leave a full-width entrance corridor above the first row of beasts.
    for (int y = map->topBoundary() + verticalGap;
         y + monsterHeight <= map->bottomBoundary();
         y += monsterHeight + verticalGap)
    {
        for (int x = map->leftBoundary();
             x + monsterWidth <= map->rightBoundary();
             x += monsterWidth + horizontalGap)
        {
            const bool overlapsHero = x < hero->x() + hero->width() &&
                                      x + monsterWidth > hero->x() &&
                                      y < hero->y() + hero->height() &&
                                      y + monsterHeight > hero->y();
            if (overlapsHero)
                continue;

            auto beast = new Beast(static_cast<float>(x), static_cast<float>(y),
                                   monsterWidth, monsterHeight);
            beast->setColor(0, 255, 0);
            pushEntityToItemEntityList(beast);
        }
    }
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

void PlayStage::pushEntityToItemEntityList(ItemEntity *item)
{
    auto center = item->center();
    auto indexPair = getGridIndex(center.x, center.y);
    gridSystem_.addEntity(indexPair.first, indexPair.second, item);
}

// updateEntities
void PlayStage::updateEntities(World &world, Layer &layer)
{
    // iterate gridSystem
    auto &gridMap = gridSystem_.gridMap();
    int gridCount = gridMap.size();
    for (int i = 0; i < gridCount; ++i)
    {
        auto &grid = gridMap[i];
        auto &itemList = grid.itemList();
        int gridItemLen = itemList.size();
        for (int j = 0; j < gridItemLen; ++j)
        {
            auto *item = itemList[j];
            if (item)
                item->update(world, layer);
        }
    }

    Stage::updateEntities(world, layer);
}

// renderEntities
void PlayStage::renderEntities(World &world, Layer &layer)
{

    auto &gridMap = gridSystem_.gridMap();
    int gridCount = gridMap.size();
    for (int i = 0; i < gridCount; ++i)
    {
        auto &grid = gridMap[i];
        auto &itemList = grid.itemList();
        int gridItemLen = itemList.size();
        for (int j = 0; j < gridItemLen; ++j)
        {
            auto *item = itemList[j];
            if (item)
                item->render(world, layer);
        }
    }

    Stage::renderEntities(world, layer);
}
