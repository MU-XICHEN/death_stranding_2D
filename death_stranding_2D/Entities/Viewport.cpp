#include "Viewport.h"
#include "GameBase.h"
#include "World.h"
#include "GamesEngineeringBase.h"

int checkPixelInMapBorder(int x, int y, int mapWidth, int mapHeight, int borderSize)
{
    int topBorder = borderSize, leftBorder = borderSize;

    int bottomBorder = mapHeight - borderSize;
    int rightBorder = mapWidth - borderSize;

    int inVerticalBorder = !(leftBorder <= x && x < rightBorder);
    int inHorizontalBorder = !(topBorder <= y && y < bottomBorder);

    return inVerticalBorder || inHorizontalBorder;
}

void Viewport::update(World &world)
{
    int windowX, windowY;
    const Hero &hero = world.gameInfo().hero();

    int halfWidth = WINDOW_WIDTH / 2;
    int halfHeight = WINDOW_HEIGHT / 2;
    GEB::Position heroCenter = hero.center();

    int idealWindowX = heroCenter.x - halfWidth;
    int idealWindowY = heroCenter.y - halfHeight;

    // control top and left
    int targetX = GEB::Math::max(idealWindowX, 0);
    int targetY = GEB::Math::max(idealWindowY, 0);

    // control bottom and right
    int minX = MAP_WIDTH - WINDOW_WIDTH;
    int minY = MAP_HEIGHT - WINDOW_HEIGHT;
    int finalX = GEB::Math::min(targetX, minX);
    int finalY = GEB::Math::min(targetY, minY);

    x_ = finalX;
    y_ = finalY;

    world.gameInfo().updateViewport(*this);
}

void Viewport::render(World &world)
{
    const auto &map = world.gameInfo().map();
    for (int i = x_; i < x_ + width_; i++)
    {
        for (int j = y_; j < y_ + height_; j++)
        {
            bool inBorder = checkPixelInMapBorder(i, j, map.width, map.height, map.borderSize);
            if (inBorder)
            {
                int windowStartXInMap = i - x_;
                int windowStartYInMap = j - y_;
                world.canvas().draw(windowStartXInMap, windowStartYInMap, map.borderColor.r, map.borderColor.g, map.borderColor.b);
            }
        }
    }
}
