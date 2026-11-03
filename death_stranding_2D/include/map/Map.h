#pragma once
#include "core/GameBase.h"
#include "entities/Entity.h"
#include "entities/AABB.h"
#include "structure/Array.h"
#include "Tile.h"

enum MapType
{
    Chapter1 = 0,
    Chapter2 = 1,
    Chapter3 = 2,
};

struct MapSource
{
    int colCnt;
    int rowCnt;
    int tileSize;
    int pixelWidth;
    int pixelHeight;
    const int *ground;
    int groundCount;
};

class Map : public Entity
{
public:
    Map(MapType type);
    ~Map() override = default;

    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;

    const GEB::Structures::Array<TileBlock> &tileBlocks() const { return tileBlocks_; }

    void addCollisionTileToCandiListByDisVec(Entity *detectedItem,
                                             GEB::Vec2 distanceVec,
                                             GEB::Structures::Array<Entity *> &candidateItemList);

    void addCollisionTileToCandiListByAABB(Entity *detectedItem,
                                           const AABB &aabb,
                                           GEB::Structures::Array<Entity *> &candidateItemList);

private:
    void loadChapterMap(MapType type);
    void loadMapSource(MapType type);
    void cachemap();
    void mergeTiles(const int *ground, int colCnt, int rowCnt);

    MapType mapType_;
    MapSource source_{};
    GEB::Structures::Array<int> ground_;

    GEB::Structures::Array<unsigned char> mapCache_;
    GEB::Structures::Array<TileBlock> tileBlocks_; // need collision detection
};

struct MapInfo
{
    inline int width() const { return map_ ? map_->width() : 0; }
    inline int height() const { return map_ ? map_->height() : 0; }

    inline float topBoundary() const { return 0; }
    inline float bottomBoundary() const { return static_cast<float>(height()); }

    inline float leftBoundary() const { return 0; }
    inline float rightBoundary() const { return static_cast<float>(width()); }

    void setMap(const Map *map) { map_ = map; }

private:
    const Map *map_;
};
