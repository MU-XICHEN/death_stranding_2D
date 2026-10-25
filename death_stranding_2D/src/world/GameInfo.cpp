#include "world/GameInfo.h"

void GameInfo::updateViewport(const HeroInfo &hero, const MapInfo &map)
{
    int halfWidth = VIEWPORT_WIDTH / 2;
    int halfHeight = VIEWPORT_HEIGHT / 2;
    GEB::Position heroCenter = hero.center();

    float viewportIdealX = heroCenter.x - halfWidth;
    float viewportIdealY = heroCenter.y - halfHeight;

    // control top and left
    float targetX = GEB::Math::max(viewportIdealX, 0.0f);
    float targetY = GEB::Math::max(viewportIdealY, 0.0f);

    // control bottom and right
    float minX = GEB::Math::max(static_cast<float>(map.width - VIEWPORT_WIDTH), 0.0f);
    float minY = GEB::Math::max(static_cast<float>(map.height - VIEWPORT_HEIGHT), 0.0f);
    float finalX = GEB::Math::min(targetX, minX);
    float finalY = GEB::Math::min(targetY, minY);

    viewport_.setPosition(finalX, finalY);
}
