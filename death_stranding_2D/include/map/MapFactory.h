#pragma once
#include "core/GameBase.h"
#include "structure/Array.h"
#include <string>

enum MapType
{
    Chapter1 = 0,
    Chapter2 = 1,
    Chapter3 = 2,
    Chapter4 = 3,
    Chapter5 = 4
};

struct MapSource
{
    struct TileDefinition
    {
        bool collision;
        // Index into MapSource::images; tile definitions do not own images.
        int imageIndex;
    };

    int colCnt;
    int rowCnt;
    int tileSize;
    int pixelWidth;
    int pixelHeight;
    const char *const *images;
    int imageCount;

    const TileDefinition *tileDefinitions;
    int tileDefinitionCount;

    const int *ground;
    int groundCount;
};

class MapFactory
{
public:
    static const MapSource &fetchMapSource(MapType type);
    static const MapSource &Chapter1Source();

private:
    explicit MapFactory(const char *path);

    MapSource source_{};
    GEB::Structures::Array<std::string> strings_;
    GEB::Structures::Array<const char *> images_;
    GEB::Structures::Array<MapSource::TileDefinition> tileDefinitions_;
    GEB::Structures::Array<int> ground_;
};
