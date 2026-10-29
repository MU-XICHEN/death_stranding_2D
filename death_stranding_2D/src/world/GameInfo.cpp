#include "world/GameInfo.h"

void GameInfo::updateViewport(const HeroInfo &hero, const MapInfo &map)
{
    const float halfWidth = VIEWPORT_WIDTH / 2.0f;
    const float halfHeight = VIEWPORT_HEIGHT / 2.0f;
    GEB::Vec2 heroCenter = hero.center();

    float viewportIdealX = heroCenter.x - halfWidth;
    float viewportIdealY = heroCenter.y - halfHeight;

    // control top and left
    float targetX = GEB::Math::max(viewportIdealX, 0.0f);
    float targetY = GEB::Math::max(viewportIdealY, 0.0f);

    // control bottom and right
    float minX = static_cast<float>(GEB::Math::max(map.width() - VIEWPORT_WIDTH, 0));
    float minY = static_cast<float>(GEB::Math::max(map.height() - VIEWPORT_HEIGHT, 0));
    float finalX = GEB::Math::min(targetX, minX);
    float finalY = GEB::Math::min(targetY, minY);

    viewport_.setPosition(finalX, finalY);
}
