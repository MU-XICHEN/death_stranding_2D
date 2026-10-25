#include "world/World.h"
#include "map/Map.h"

int checkPixelInMapBorder(int x, int y, int mapWidth, int mapHeight, int borderSize)
{
    int topBorder = borderSize, leftBorder = borderSize;

    int bottomBorder = mapHeight - borderSize;
    int rightBorder = mapWidth - borderSize;

    int inVerticalBorder = !(leftBorder <= x && x < rightBorder);
    int inHorizontalBorder = !(topBorder <= y && y < bottomBorder);

    return inVerticalBorder || inHorizontalBorder;
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