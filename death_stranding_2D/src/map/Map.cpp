#include "world/World.h"
#include "map/Map.h"
#include "entities/AABB.h"

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

GEB::Position Map::playerPosition() const
{
    return center();
}

void Map::MergeTiles(const int *ground, int colCnt, int rowCnt)
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
        image->load(source_.images[i]);
        if (!image->load(source_.images[i]))
        {
            delete image;
            continue;
        }

        images_.pushBack(image);
    }

    // merge tiles to large pieces
    MergeTiles(source_.ground, source_.colCnt, source_.rowCnt);
}

void Map::update(World &world, Layer &layer)
{
    world.gameInfo().updateMapInfo(*this);
}

void Map::render(World &world, Layer &layer)
{
    auto &canvas = world.canvas();

    const Viewport &viewport = world.gameInfo().viewport();

    const int visiblePixelWidth = viewport.width();
    const int visiblePixelHeight = viewport.height();

    const int viewportX = GEB::Math::floorToInt(viewport.x());
    const int viewportY = GEB::Math::floorToInt(viewport.y());

    // draw map to viewport by each pixel
    for (int i = 0; i < visiblePixelWidth; i++)
    {
        for (int j = 0; j < visiblePixelHeight; j++)
        {
            int targetX = viewportX + i;
            int targetY = viewportY + j;

            if (targetX < 0 || targetY < 0)
                continue;

            int tileIdxX = targetX / source_.tileSize;
            int tileIdxY = targetY / source_.tileSize;

            if (tileIdxX < 0 || tileIdxX >= source_.colCnt ||
                tileIdxY < 0 || tileIdxY >= source_.rowCnt)
                continue;

            int tileX = tileIdxX * source_.tileSize;
            int tileY = tileIdxY * source_.tileSize;

            int imageIdxX = targetX - tileX;
            int imageIdxY = targetY - tileY;

            auto tileIndex = source_.ground[tileIdxY * source_.colCnt + tileIdxX];
            auto imageIndex = source_.tileDefinitions[tileIndex].imageIndex;
            auto image = images_[imageIndex];

            if (!image)
                return;

            canvas.draw(i, j, image->at(imageIdxX, imageIdxY));
        }
    }
}

void Map::addCollisionTileToCandiList(Entity *detectedItem,
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
        distanceVec,
        mapBoundary);

    for (int i = 0; i < tileBlocks_.size(); i++)
    {
        auto &tileBlock = tileBlocks_[i];
        bool intersected = aabb.intersects(AABB(tileBlock.x(), tileBlock.y(), tileBlock.width(), tileBlock.height()));
        if (intersected)
        {
            candidateItemList.pushBack(&tileBlock);
        }
    }
}
