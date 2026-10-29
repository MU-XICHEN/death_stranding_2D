#include "map/MapFactory.h"
#include <fstream>
#include <stdexcept>

MapFactory::MapFactory(const char *path)
{
    std::ifstream file(path);
    if (!file)
        throw std::runtime_error("Cannot open map file");

    std::string text;
    file >> source_.tileSize;
    file >> source_.imageCount;
    if (!file || source_.tileSize <= 0 || source_.imageCount <= 0)
        throw std::runtime_error("Invalid tile size or image count");

    for (int i = 0; i < source_.imageCount; ++i)
    {
        if (!(file >> text))
            throw std::runtime_error("Cannot read image path");
        strings_.pushBack(text);
    }

    file >> source_.tileDefinitionCount;
    if (!file || source_.tileDefinitionCount <= 0)
        throw std::runtime_error("Invalid tile count");
    for (int i = 0; i < source_.tileDefinitionCount; ++i)
    {
        MapSource::TileDefinition tile{};
        file >> tile.collision >> tile.imageIndex;
        if (!file || tile.imageIndex < 0 || tile.imageIndex >= source_.imageCount)
            throw std::runtime_error("Invalid tile definition");
        tileDefinitions_.pushBack(tile);
    }

    file >> source_.rowCnt >> source_.colCnt;
    if (!file || source_.rowCnt <= 0 || source_.colCnt <= 0)
        throw std::runtime_error("Invalid ground dimensions");
    source_.pixelWidth = source_.colCnt * source_.tileSize;
    source_.pixelHeight = source_.rowCnt * source_.tileSize;
    source_.groundCount = source_.rowCnt * source_.colCnt;
    for (int row = 0; row < source_.rowCnt; ++row)
    {
        for (int col = 0; col < source_.colCnt; ++col)
        {
            int tileIndex = 0;
            if (!(file >> tileIndex) || tileIndex < 0 || tileIndex >= source_.tileDefinitionCount)
                throw std::runtime_error("Invalid ground tile");
            ground_.pushBack(tileIndex);
        }
    }

    file >> std::ws;
    if (!file.eof() || file.bad())
        throw std::runtime_error("Unexpected content after map");

    for (int i = 0; i < source_.imageCount; ++i)
        images_.pushBack(strings_[i].c_str());
    source_.images = images_.head();
    source_.tileDefinitions = tileDefinitions_.head();
    source_.ground = ground_.head();
}

const MapSource &MapFactory::fetchMapSource(MapType type)
{
    return Chapter1Source();
}

const MapSource &MapFactory::Chapter1Source()
{
    static const MapFactory factory("assets/map/chapter1.txt");
    return factory.source_;
}
