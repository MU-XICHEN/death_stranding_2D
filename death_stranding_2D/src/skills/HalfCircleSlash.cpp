#include "skills/HalfCircleSlash.h"
#include "world/World.h"
#include "utils/Utils.h"

void HalfCircleSlash::update(World &world)
{
    if (!active_)
        return;

    const float dt = world.gameInfo().dt();
    const auto &attachInfo = attackInfo();
}

void HalfCircleSlash::render(World &world)
{
    const auto &attackInfo = this->attackInfo();

    const auto unitDirVec2 = GEB::Utils::transDirToUnitVec2(attackInfo.direction);

    const auto hitBoxWidth = attackInfo.hitBoxWidth;
    const auto hitBoxHeight = attackInfo.hitBoxHeight;

    GEB::Vec2 originPoint1 = attackInfo.originPoint;

    auto degree = attackInfo.angleSpeed * world.gameInfo().dt();

    const auto adjustedDirVec2 = GEB::Utils::rotateClockwise(unitDirVec2, attackInfo.originPoint, degree);

    // todo: draw, calculate the position using matrix transformation in the loop
}