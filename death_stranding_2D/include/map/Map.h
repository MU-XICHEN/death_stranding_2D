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

struct MapExit
{
    GEB::Vec2 position;
    MapType destination;
};

class Map : public Entity
{
public:
    Map(MapType type);
    ~Map() override = default;

    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;

    GEB::Vec2 entrancePosition(int entityWidth, int entityHeight) const;
    const MapExit *exit() const { return hasExit_ ? &exit_ : nullptr; }
    MapType type() const { return mapType_; }
    int tileSize() const { return source_.tileSize; }
    const GEB::Structures::Array<GEB::Vec2> &monsterPositions() const { return monsterPositions_; }
    const GEB::Structures::Array<GEB::Vec2> &potionPositions() const { return potionPositions_; }

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
    void loadPositions(MapType type, const char *suffix, int itemSize,
                       GEB::Structures::Array<GEB::Vec2> &positions);
    void cachemap();
    void mergeTiles(const int *ground, int colCnt, int rowCnt);

    MapType mapType_;
    GEB::Vec2 entrance_{}; // Tile center converted from the row/column in txt.
    MapExit exit_{};
    bool hasExit_ = false;
    MapSource source_{};
    GEB::Structures::Array<int> ground_;
    GEB::Structures::Array<GEB::Vec2> monsterPositions_; // Top-left positions loaded from .monsters.txt.
    GEB::Structures::Array<GEB::Vec2> potionPositions_; // Top-left positions loaded from .potions.txt.

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
