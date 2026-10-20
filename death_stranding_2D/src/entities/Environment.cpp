#include "entities/Environment.h"
#include "core/GameBase.h"
#include "world/World.h"
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

void Environment::update(World &world, Layer &layer)
{
    const Hero &hero = world.gameInfo().hero();

    int halfWidth = ENVIRONMENT_WIDTH / 2;
    int halfHeight = ENVIRONMENT_HEIGHT / 2;
    GEB::Position heroCenter = hero.center();

    float idealWindowX = heroCenter.x - halfWidth;
    float idealWindowY = heroCenter.y - halfHeight;

    // control top and left
    float targetX = GEB::Math::max(idealWindowX, 0.0f);
    float targetY = GEB::Math::max(idealWindowY, 0.0f);

    // control bottom and right
    float minX = static_cast<float>(MAP_WIDTH - ENVIRONMENT_WIDTH);
    float minY = static_cast<float>(MAP_HEIGHT - ENVIRONMENT_HEIGHT);
    float finalX = GEB::Math::min(targetX, minX);
    float finalY = GEB::Math::min(targetY, minY);

    x_ = finalX;
    y_ = finalY;

    world.gameInfo().updateEnvironment(*this);
}

void Environment::render(World &world, Layer &layer)
{
    const auto &map = world.gameInfo().map();
    
    for (int i = x_; i < x_ + width_; i++)
    {
        for (int j = y_; j < y_ + height_; j++)
        {
            bool inBorder = checkPixelInMapBorder(i, j, map.width, map.height, map.borderSize);
            if (inBorder)
            {
                int windowStartXInMap = i - static_cast<int>(x_);
                int windowStartYInMap = j - static_cast<int>(y_);
                world.canvas().draw(windowStartXInMap, windowStartYInMap, map.borderColor.r, map.borderColor.g, map.borderColor.b);
            }
        }
    }
}
