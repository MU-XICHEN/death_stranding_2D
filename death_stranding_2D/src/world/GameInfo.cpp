#include "world/GameInfo.h"

void GameInfo::updateViewport(const HeroInfo &hero, const MapInfo &map)
{
    const float halfWidth = VIEWPORT_WIDTH / 2.0f;
    const float halfHeight = VIEWPORT_HEIGHT / 2.0f;
    GEB::Vec2 heroCenter = hero.center();

    float viewportIdealX = heroCenter.x - halfWidth;
    float viewportIdealY = heroCenter.y - halfHeight;

    // control top and left
    float targetX = std::max(viewportIdealX, 0.0f);
    float targetY = std::max(viewportIdealY, 0.0f);

    // control bottom and right
    float minX = static_cast<float>(std::max(map.width() - VIEWPORT_WIDTH, 0));
    float minY = static_cast<float>(std::max(map.height() - VIEWPORT_HEIGHT, 0));
    float finalX = std::min(targetX, minX);
    float finalY = std::min(targetY, minY);

    viewport_.setPosition(finalX, finalY);
}
