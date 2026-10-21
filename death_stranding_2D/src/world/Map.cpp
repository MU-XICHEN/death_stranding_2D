#include "world/Map.h"
#include "world/World.h"

int checkPixelInMapBorder(int x, int y, int mapWidth, int mapHeight, int borderSize)
{
    int topBorder = borderSize, leftBorder = borderSize;

    int bottomBorder = mapHeight - borderSize;
    int rightBorder = mapWidth - borderSize;

    int inVerticalBorder = !(leftBorder <= x && x < rightBorder);
    int inHorizontalBorder = !(topBorder <= y && y < bottomBorder);

    return inVerticalBorder || inHorizontalBorder;
}

void Map::update(World &world, Layer &layer)
{
    world.gameInfo().updateMap(*this);
}

void Map::render(World &world, Layer &layer)
{
    // render Barriers

    // render Border
    const auto &viewport = world.gameInfo().viewport();

    auto viewportX = viewport.x();
    auto viewPortY = viewport.y();
    auto viewPortBoundaryX = viewportX + viewport.width();
    auto viewPortBoundaryY = viewPortY + viewport.height();

    for (int i = viewportX; i < viewPortBoundaryX; i++)
    {
        for (int j = viewPortY; j < viewPortBoundaryY; j++)
        {
            bool inBorder = checkPixelInMapBorder(i, j, width_, height_, borderSize_);
            if (inBorder)
            {
                int windowStartXInMap = i - static_cast<int>(viewportX);
                int windowStartYInMap = j - static_cast<int>(viewPortY);
                world.canvas().draw(windowStartXInMap, windowStartYInMap, borderColor_.r, borderColor_.g, borderColor_.b);
            }
        }
    }
}