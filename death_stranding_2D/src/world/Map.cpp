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
}