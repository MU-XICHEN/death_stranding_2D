#include "entities/Swordsman.h"
#include "skills/AttackHalfCircleSlash.h"

Swordsman::Swordsman(float x, float y, int width, int height)
    : Hero(x, y, width, height)
{
    addAttack(new AttackHalfCircleSlash('J', true));
}

AttackInfo Swordsman::createAttackInfo(Attack *attack)
{
    if (dynamic_cast<AttackHalfCircleSlash *>(attack))
    {
        const auto originPoint = getOriginPoint();
        const GEB::Pair<int, float> props[] = {
            {AttackInfo::Damage, 10.0f},
            {AttackInfo::Duration, 1.0f},
            {AttackInfo::HitStartTime, 0.2f},
            {AttackInfo::HitEndTime, 0.4f},
            {AttackInfo::HitBoxWidth, 80.0f},
            {AttackInfo::HitBoxHeight, 30.0f},
            {AttackInfo::OriginPointX, originPoint.x},
            {AttackInfo::OriginPointY, originPoint.y},
            {AttackInfo::Direction, static_cast<float>(direction())},
        };
        return AttackInfo(props, sizeof(props) / sizeof(props[0]));
    }

    return AttackInfo{};
}
