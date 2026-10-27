#pragma once
#include "core/GameBase.h"
#include "entities/Entity.h"
#include "structure/Array.h"
#include "Tile.h"
#include "MapFactory.h"

class Map : public Entity
{
public:
    explicit Map(MapType mapType);
    ~Map();

    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;

    const GEB::Structures::Array<TileBlock> &tileBlocks() const { return tileBlocks_; }
    GEB::Position playerPosition() const;

    void addCollisionTileToCandiList(Entity *detectedItem,
                                     GEB::Vec2 distanceVec,
                                     GEB::Structures::Array<Entity *> &candidateItemList);

private:
    void initialize();
    void cachemap();
    void mergeTiles(const int *ground, int colCnt, int rowCnt);

    const MapSource &source_;

    GEB::Structures::Array<GEB::Image *> images_;
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
