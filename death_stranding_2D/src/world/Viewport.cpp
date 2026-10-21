#include "world/Viewport.h"
#include "core/GameBase.h"
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

// void Viewport::render(World &world, Layer &layer)
// {
//     const auto &map = world.gameInfo().map();

//     for (int i = x_; i < x_ + width_; i++)
//     {
//         for (int j = y_; j < y_ + height_; j++)
//         {
//             bool inBorder = checkPixelInMapBorder(i, j, map.width, map.height, map.borderSize);
//             if (inBorder)
//             {
//                 int windowStartXInMap = i - static_cast<int>(x_);
//                 int windowStartYInMap = j - static_cast<int>(y_);
//                 world.canvas().draw(windowStartXInMap, windowStartYInMap, map.borderColor.r, map.borderColor.g, map.borderColor.b);
//             }
//         }
//     }
// }
