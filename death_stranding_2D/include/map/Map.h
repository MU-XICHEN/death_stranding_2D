#pragma once
#include "core/GameBase.h"
#include "entities/Entity.h"
#include "structure/Array.h"
#include "structure/String.h"
#include "Tile.h"
#include "MapFactory.h"

struct MapInfo
{
    int width;
    int height;

    inline int topBoundary() const { return 0; }
    inline int bottomBoundary() const { return height; }

    inline int leftBoundary() const { return 0; }
    inline int rightBoundary() const { return width; }
};

class Map : public Entity
{
public:
    explicit Map(MapType mapType);
    ~Map();

    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;

private:
    void initialize();
    void addCollisionTileToCandiList(Entity *detectedItem,
                                     float distance,
                                     GEB::Structures::Array<Entity *> &candidateItemList);

    const MapSource &source_;

    MapInfo mapInfo_;
    GEB::Structures::Array<GEB::Image *> images_;
    GEB::Structures::Array<Tile> tilePool_;
};