#include "world/World.h"
#include "map/Map.h"

GEB::Pair<bool, int> tileProcessed(int tileIndexX, int tileIndexY, const GEB::Structures::Array<TileBlock> &tileRects)
{
    // first for processed, second for next idxX
    GEB::Pair<bool, int> res{false, 0};
    for (int i = 0; i < tileRects.size(); i++)
    {
        const TileBlock &tileRect = tileRects[i];
        if (tileIndexX >= tileRect.x() && tileIndexX < tileRect.x() + tileRect.width() &&
            tileIndexY >= tileRect.y() && tileIndexY < tileRect.y() + tileRect.height())
        {
            res.first = true;
            res.second = static_cast<int>(tileRect.x()) + tileRect.width();
            break;
        }
    }
    return res;
}

int expandTileRectSizeImpl(int direction, int tileIndexX, int tileIndexY,
                           int maxColCnt, const int *ground, int colCnt,
                           const GEB::Structures::Array<TileBlock> &tileRects)
{
    int rowWidth = 0;
    const int step = direction == 1 ? 1 : -1;
    for (int x = tileIndexX; rowWidth < maxColCnt && x >= 0 && x < colCnt; x += step)
    {
        // if current tile does not need collision detection or has been processed, then stop.
        if (ground[tileIndexY * colCnt + x] == 0 ||
            tileProcessed(x, tileIndexY, tileRects).first)
            break;
        ++rowWidth;
    }
    return rowWidth;
}

void expandTileRectSize(TileBlock &tileRect, int tileIndexX, int tileIndexY,
                        const int *ground, int colCnt, int rowCnt,
                        const GEB::Structures::Array<TileBlock> &tileRects)
{
    // calculate the malColCnt first
    const int maxColCnt = expandTileRectSizeImpl(
        1, tileIndexX, tileIndexY, colCnt - tileIndexX, ground, colCnt, tileRects);

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
            tileRects);

        // reject current row, as it's shorter than  maxColCnt.
        if (rowWidth < maxColCnt)
            break;
        ++blockRowCnt;
        direction = 1 - direction;
    }

    tileRect = TileBlock{static_cast<float>(tileIndexX),
                         static_cast<float>(tileIndexY),
                         maxColCnt, blockRowCnt};
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

            auto res = tileProcessed(j, i, tileBlocks_);
            if (res.first)
            {
                j = res.second - 1;
                continue;
            }

            // if not processed, create a new tile block.
            TileBlock newTile{static_cast<float>(j), static_cast<float>(i), 1, 1};
            expandTileRectSize(newTile, j, i, ground, colCnt, rowCnt, tileBlocks_);

            tileBlocks_.pushBack(newTile);
            j += newTile.width() - 1;
        }
    }
}

void Map::initialize()
{
    mapInfo_.width = source_.pixelWidth;
    mapInfo_.height = source_.pixelHeight;

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
    world.gameInfo().updateMapInfo(mapInfo_);
}

void Map::render(World &world, Layer &layer)
{
    auto &canvas = world.canvas();

    const Viewport &viewport = world.gameInfo().viewport();

    // ceil
    int visibleWidth = viewport.width();
    int visibleHeight = viewport.height();
    int viewportX = static_cast<int>(viewport.x());
    int viewportY = static_cast<int>(viewport.y());

    // draw map to viewport by each pixel
    for (int i = 0; i < visibleWidth; i++)
    {
        for (int j = 0; j < visibleHeight; j++)
        {
            int targetX = viewportX + i;
            int targetY = viewportY + j;
            int tileIdxX = targetX / source_.tileSize;
            int tileIdxY = targetY / source_.tileSize;
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
                                      float distance,
                                      GEB::Structures::Array<Entity *> &candidateItemList)
{
    
}