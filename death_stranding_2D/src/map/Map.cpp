#include "world/World.h"
#include "map/Map.h"
#include "entities/AABB.h"
#include <cstring>

GEB::Pair<bool, int> tileProcessed(int tileIndexX, int tileIndexY, int tileSize,
                                   const GEB::Structures::Array<TileBlock> &tileRects)
{
    // first for processed, second for next idxX
    GEB::Pair<bool, int> res{false, 0};
    for (int i = 0; i < tileRects.size(); i++)
    {
        const TileBlock &tileRect = tileRects[i];
        const int firstCol = static_cast<int>(tileRect.x() / tileSize + 0.5f);
        const int firstRow = static_cast<int>(tileRect.y() / tileSize + 0.5f);
        const int endCol = static_cast<int>((tileRect.x() + tileRect.width()) / tileSize + 0.5f);
        const int endRow = static_cast<int>((tileRect.y() + tileRect.height()) / tileSize + 0.5f);
        if (tileIndexX >= firstCol && tileIndexX < endCol &&
            tileIndexY >= firstRow && tileIndexY < endRow)
        {
            res.first = true;
            res.second = endCol;
            break;
        }
    }
    return res;
}

int expandTileRectSizeImpl(int direction, int tileIndexX, int tileIndexY,
                           int maxColCnt, const int *ground, int colCnt, int tileSize,
                           const GEB::Structures::Array<TileBlock> &tileRects)
{
    int rowWidth = 0;
    const int step = direction == 1 ? 1 : -1;
    for (int x = tileIndexX; rowWidth < maxColCnt && x >= 0 && x < colCnt; x += step)
    {
        // if current tile does not need collision detection or has been processed, then stop.
        if (ground[tileIndexY * colCnt + x] == 0 ||
            tileProcessed(x, tileIndexY, tileSize, tileRects).first)
            break;
        ++rowWidth;
    }
    return rowWidth;
}

int expandTileRectSize(TileBlock &tileRect, int tileIndexX, int tileIndexY,
                       const int *ground, int colCnt, int rowCnt, int tileSize,
                       const GEB::Structures::Array<TileBlock> &tileRects)
{
    // calculate the malColCnt first
    const int maxColCnt = expandTileRectSizeImpl(
        1, tileIndexX, tileIndexY, colCnt - tileIndexX, ground, colCnt, tileSize, tileRects);

    int blockRowCnt = 1;
    // the first row went right; the next row goes left.
    int direction = 0;

    // jump to next row, use maxColCnt as new argument.
    for (int y = tileIndexY + 1; y < rowCnt; ++y)
    {
        const int startX = direction == 1 ? tileIndexX : tileIndexX + maxColCnt - 1;
        const int rowWidth = expandTileRectSizeImpl(
            direction,
            startX,
            y,
            maxColCnt,
            ground,
            colCnt,
            tileSize,
            tileRects);

        // reject current row, as it's shorter than  maxColCnt.
        if (rowWidth < maxColCnt)
            break;

        ++blockRowCnt;
        direction = 1 - direction;
    }

    tileRect = TileBlock{static_cast<float>(tileIndexX * tileSize),
                         static_cast<float>(tileIndexY * tileSize),
                         maxColCnt * tileSize, blockRowCnt * tileSize};
    return maxColCnt;
}

Map::Map(MapType mapType)
    : Entity(0, 0, 0, 0),
      source_(MapFactory::fetchMapSource(mapType))
{
    initialize();
}

Map::~Map()
{
    for (int i = 0; i < images_.size(); ++i)
    {
        delete images_[i];
    }
}

void Map::mergeTiles(const int *ground, int colCnt, int rowCnt)
{
    tileBlocks_.resize(0);
    if (!ground || colCnt <= 0 || rowCnt <= 0)
        return;

    for (int i = 0; i < rowCnt; i++)
    {
        for (int j = 0; j < colCnt; j++)
        {
            //  skip tiles, which do not need collision detection.
            if (ground[i * colCnt + j] == 0)
                continue;

            auto res = tileProcessed(j, i, source_.tileSize, tileBlocks_);
            if (res.first)
            {
                j = res.second - 1;
                continue;
            }

            // if not processed, create a new tile block.
            TileBlock newTile;
            const int mergedColCount = expandTileRectSize(
                newTile, j, i, ground, colCnt, rowCnt, source_.tileSize, tileBlocks_);

            tileBlocks_.pushBack(newTile);
            j += mergedColCount - 1;
        }
    }
}

void Map::initialize()
{
    width_ = source_.pixelWidth;
    height_ = source_.pixelHeight;

    // load images
    for (int i = 0; i < source_.imageCount; ++i)
    {
        GEB::Image *image = new GEB::Image();
        if (!image->load(source_.images[i]))
        {
            delete image;
            image = nullptr;
        }

        images_.pushBack(image);
    }

    // merge tiles to large pieces
    GEB::Structures::Array<int> collisionGround(source_.groundCount);
    for (int i = 0; i < source_.groundCount; ++i)
        collisionGround[i] = source_.tileDefinitions[source_.ground[i]].collision ? 1 : 0;
    mergeTiles(collisionGround.head(), source_.colCnt, source_.rowCnt);

    cachemap();
}

void Map::cachemap()
{
    mapCache_.resize(width_ * height_ * 3);

    std::memset(mapCache_.head(), 0, mapCache_.size());

    for (int row = 0; row < source_.rowCnt; ++row)
    {
        for (int col = 0; col < source_.colCnt; ++col)
        {
            const int tileIndex = source_.ground[row * source_.colCnt + col];
            const int imageIndex = source_.tileDefinitions[tileIndex].imageIndex;
            const auto *image = images_[imageIndex];

            if (!image || !image->data || image->width == 0 || image->height == 0)
                continue;

            for (int y = 0; y < source_.tileSize; ++y)
            {
                for (int x = 0; x < source_.tileSize; ++x)
                {
                    const int mapX = col * source_.tileSize + x;
                    const int mapY = row * source_.tileSize + y;

                    const int cacheIndex = (mapY * width_ + mapX) * 3;
                    const auto *pixel = image->at(x, y);
                    mapCache_[cacheIndex] = pixel[0];
                    mapCache_[cacheIndex + 1] = pixel[1];
                    mapCache_[cacheIndex + 2] = pixel[2];
                }
            }
        }
    }
}

void Map::update(World &world, Layer &layer) {}

void Map::render(World &world, Layer &layer)
{
    if (mapCache_.size() == 0)
        return;

    auto &canvas = world.canvas();
    const Viewport &viewport = world.gameInfo().viewport();

    const int viewportWidth = viewport.width();
    const int viewportHeight = viewport.height();

    const int mapStartX = std::max(static_cast<int>(viewport.x()), 0);
    const int mapStartY = std::max(static_cast<int>(viewport.y()), 0);

    const int mapEndX = std::min(mapStartX + viewportWidth, width_);
    const int mapEndY = std::min(mapStartY + viewportHeight, height_);

    if (mapStartX >= mapEndX || mapStartY >= mapEndY)
        return;

    const int rowBytes = (mapEndX - mapStartX) * 3;

    for (int mapY = mapStartY; mapY < mapEndY; ++mapY)
    {
        const int canvasX = 0;
        const int canvasY = mapY - mapStartY;

        const int cacheIndex = (mapY * width_ + mapStartX) * 3;

        const int canvasIndex = (canvasY * viewportWidth + canvasX) * 3;

        auto *canvasPixels = canvas.getBackBuffer();

        // speed up, copy a whole row of pixels at once.
        std::memcpy(&canvasPixels[canvasIndex], &mapCache_[cacheIndex], rowBytes);
    }
}

void Map::addCollisionTileToCandiListByAABB(Entity *detectedItem,
                                            const AABB &aabb,
                                            GEB::Structures::Array<Entity *> &candidateItemList)
{
    for (int i = 0; i < tileBlocks_.size(); i++)
    {
        auto &tileBlock = tileBlocks_[i];
        bool intersected = aabb.intersects(tileBlock);
        if (intersected)
        {
            candidateItemList.pushBack(&tileBlock);
        }
    }
}

void Map::addCollisionTileToCandiListByDisVec(Entity *detectedItem,
                                              GEB::Vec2 distanceVec,
                                              GEB::Structures::Array<Entity *> &candidateItemList)
{
    if (!detectedItem)
        return;

    GEB::Boundary mapBoundary{
        .top = 0,
        .bottom = static_cast<float>(height_),
        .left = 0,
        .right = static_cast<float>(width_),
    };

    auto aabb = AABB::createAABB(
        detectedItem,
        distanceVec);

    addCollisionTileToCandiListByAABB(detectedItem, aabb, candidateItemList);
}
