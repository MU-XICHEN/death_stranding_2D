#include "world/GameInfo.h"

void GameInfo::updateViewport(const HeroInfo &hero)
{
    int halfWidth = VIEWPORT_WIDTH / 2;
    int halfHeight = VIEWPORT_HEIGHT / 2;
    GEB::Position heroCenter = hero.center();

    float idealWindowX = heroCenter.x - halfWidth;
    float idealWindowY = heroCenter.y - halfHeight;

    // control top and left
    float targetX = GEB::Math::max(idealWindowX, 0.0f);
    float targetY = GEB::Math::max(idealWindowY, 0.0f);

    // control bottom and right
    float minX = static_cast<float>(MAP_WIDTH - VIEWPORT_WIDTH);
    float minY = static_cast<float>(MAP_HEIGHT - VIEWPORT_HEIGHT);
    float finalX = GEB::Math::min(targetX, minX);
    float finalY = GEB::Math::min(targetY, minY);

    viewport_.setPosition(finalX, finalY);
}