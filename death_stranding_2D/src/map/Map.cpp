#include "world/World.h"
#include "map/Map.h"
#include "entities/AABB.h"
#include <cstring>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>

GEB::Pair<bool, int> tileProcessed(int tileIndexX, int tileIndexY, int tileSize,
                                   const GEB::Structures::Array<TileBlock> &tileRects);
int expandTileRectSizeImpl(int direction, int tileIndexX, int tileIndexY,
                           int maxColCnt, const int *ground, int colCnt, int tileSize,
                           const GEB::Structures::Array<TileBlock> &tileRects);
int expandTileRectSize(TileBlock &tileRect, int tileIndexX, int tileIndexY,
                       const int *ground, int colCnt, int rowCnt, int tileSize,
                       const GEB::Structures::Array<TileBlock> &tileRects);

void getMapPath(MapType type, const char **path);

GEB::Vec2 Map::entrancePosition(int entityWidth, int entityHeight) const
{
    const GEB::Vec2 position{entrance_.x - entityWidth / 2.0f,
                             entrance_.y - entityHeight / 2.0f};
    if (entityWidth <= 0 || entityHeight <= 0 || position.x < 0 || position.y < 0 ||
        position.x + entityWidth > width_ || position.y + entityHeight > height_)
        throw std::runtime_error("Entity does not fit at the map entrance");

    const AABB body(position.x, position.y, entityWidth, entityHeight);
    for (int i = 0; i < tileBlocks_.size(); ++i)
    {
        if (body.intersects(tileBlocks_[i]))
            throw std::runtime_error("Map entrance overlaps an obstacle");
    }
    return position;
}

Map::Map(MapType type)
    : Entity(0, 0, 0, 0), mapType_(type)
{
    loadChapterMap(type);
}

void Map::loadChapterMap(MapType type)
{
    mapType_ = type;
    mapCache_.clear();
    tileBlocks_.clear();
    ground_.clear();
    entrance_ = {};
    exit_ = {};
    hasExit_ = false;
    source_ = {};
    width_ = 0;
    height_ = 0;

    loadMapSource(type);
    cachemap();
    // Ground values describe collision only: 0 is walkable, nonzero is blocked.
    mergeTiles(source_.ground, source_.colCnt, source_.rowCnt);
    loadPositions(type, ".monsters.txt", ROLE_SIZE, monsterPositions_);
    loadPositions(type, ".potions.txt", POTION_SIZE, potionPositions_);

    if (hasExit_)
    {
        switch (type)
        {
        case MapType::Chapter1:
            exit_.destination = MapType::Chapter2;
            break;
        default:
            throw std::runtime_error("No destination configured for this map exit");
        }
    }
}

void Map::loadMapSource(MapType type)
{
    const char *path = nullptr;
    getMapPath(type, &path);

    if (!path)
        throw std::runtime_error("Map path not found");

    std::ifstream file(std::string(path) + ".txt");
    if (!file)
        throw std::runtime_error("Cannot open map file");

    file >> source_.tileSize;
    if (!file || source_.tileSize <= 0)
        throw std::runtime_error("Invalid tile size");

    file >> source_.rowCnt >> source_.colCnt;
    if (!file || source_.rowCnt <= 0 || source_.colCnt <= 0)
        throw std::runtime_error("Invalid ground dimensions");

    const auto maxSize = (std::numeric_limits<int>::max)();
    if (source_.colCnt > maxSize / source_.tileSize ||
        source_.rowCnt > maxSize / source_.tileSize ||
        source_.rowCnt > maxSize / source_.colCnt)
        throw std::runtime_error("Map dimensions are too large");

    source_.pixelWidth = source_.colCnt * source_.tileSize;
    source_.pixelHeight = source_.rowCnt * source_.tileSize;
    source_.groundCount = source_.rowCnt * source_.colCnt;
    if (source_.pixelWidth > maxSize / 3 / source_.pixelHeight)
        throw std::runtime_error("Map cache is too large");

    for (int row = 0; row < source_.rowCnt; ++row)
    {
        for (int col = 0; col < source_.colCnt; ++col)
        {
            int ground = 0;
            if (!(file >> ground) || (ground != 0 && ground != 1))
                throw std::runtime_error("Invalid ground tile");
            ground_.pushBack(ground);
        }
    }

    // First line after the grid: entrance row, column (zero-based).
    int entranceRow = 0;
    int entranceCol = 0;
    if (!(file >> entranceRow >> entranceCol) ||
        entranceRow < 0 || entranceRow >= source_.rowCnt ||
        entranceCol < 0 || entranceCol >= source_.colCnt)
        throw std::runtime_error("Invalid map entrance configuration");
    entrance_ = {(entranceCol + 0.5f) * source_.tileSize,
                 (entranceRow + 0.5f) * source_.tileSize};

    // Optional final line: exit row, column (zero-based). No line means no exit.
    file >> std::ws;
    hasExit_ = !file.eof();
    if (hasExit_)
    {
        int exitRow = 0;
        int exitCol = 0;
        if (!(file >> exitRow >> exitCol) ||
            exitRow < 0 || exitRow >= source_.rowCnt ||
            exitCol < 0 || exitCol >= source_.colCnt)
            throw std::runtime_error("Invalid map exit coordinates");
        if (ground_[exitRow * source_.colCnt + exitCol] != 0)
            throw std::runtime_error("Map exit must be on walkable ground");
        exit_.position = {(exitCol + 0.5f) * source_.tileSize,
                          (exitRow + 0.5f) * source_.tileSize};
        file >> std::ws;
    }

    if (!file.eof() || file.bad())
        throw std::runtime_error("Unexpected content after map configuration");

    source_.ground = ground_.head();
}

void Map::loadPositions(MapType type, const char *suffix, int itemSize,
                        GEB::Structures::Array<GEB::Vec2> &positions)
{
    positions.clear();
    const char *path = nullptr;
    getMapPath(type, &path);
    std::ifstream file(std::string(path) + suffix);
    int count = 0;
    if (!(file >> count) || count < 0 || count > source_.groundCount)
        throw std::runtime_error("Invalid map item count");

    // Each entry is a tile center: row, column (zero-based), like the entrance.
    for (int i = 0; i < count; ++i)
    {
        int row = 0, col = 0;
        if (!(file >> row >> col) || row < 0 || row >= source_.rowCnt ||
            col < 0 || col >= source_.colCnt)
            throw std::runtime_error("Invalid map item coordinates");

        const GEB::Vec2 position{(col + 0.5f) * source_.tileSize - itemSize / 2.0f,
                                 (row + 0.5f) * source_.tileSize - itemSize / 2.0f};
        if (position.x < 0 || position.y < 0 || position.x + itemSize > width_ ||
            position.y + itemSize > height_)
            throw std::runtime_error("Map item does not fit inside the map");

        for (int j = 0; j < tileBlocks_.size(); ++j)
        {
            const auto &tile = tileBlocks_[j];
            if (position.x < tile.x() + tile.width() && position.x + itemSize > tile.x() &&
                position.y < tile.y() + tile.height() && position.y + itemSize > tile.y())
                throw std::runtime_error("Map item overlaps an obstacle");
        }
        positions.pushBack(position);
    }
    file >> std::ws;
    if (!file.eof() || file.bad())
        throw std::runtime_error("Unexpected content after map item positions");
}

void Map::cachemap()
{
    const char *path = nullptr;
    getMapPath(mapType_, &path);

    const std::string imagePath = std::string(path) + ".png";
    std::ifstream imageFile(imagePath, std::ios::binary);
    if (!imageFile)
        throw std::runtime_error("Cannot open map image");
    imageFile.close();

    GEB::Image image;
    if (!image.load(imagePath) || !image.data)
        throw std::runtime_error("Cannot load map image");
    if (image.channels != 3)
        throw std::runtime_error("Map image must have 3 channels");
    if (image.width != static_cast<unsigned int>(source_.pixelWidth) ||
        image.height != static_cast<unsigned int>(source_.pixelHeight))
        throw std::runtime_error("Map image dimensions do not match ground dimensions");

    width_ = source_.pixelWidth;
    height_ = source_.pixelHeight;
    mapCache_.resize(width_ * height_ * 3);
    std::memcpy(mapCache_.head(), image.data, mapCache_.size());
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

    auto aabb = AABB::createAABB(
        detectedItem,
        distanceVec);

    addCollisionTileToCandiListByAABB(detectedItem, aabb, candidateItemList);
}

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

void getMapPath(MapType type, const char **path)
{
    switch (type)
    {
    case MapType::Chapter1:
        *path = "assets/map/chapter1/chapter1";
        break;
    case MapType::Chapter2:
        *path = "assets/map/chapter2/chapter2";
        break;
    }
}
