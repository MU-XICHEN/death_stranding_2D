#include "entities/Swordsman.h"
#include "skills/AttackHalfCircleSlash.h"
#include "skills/AttackAutoTargetingMagic.h"

Swordsman::Swordsman(float x, float y, int width, int height)
    : Hero(x, y, width, height)
{
    addAttack(new AttackHalfCircleSlash('J', true));
    addAttack(new AttackAutoTargetingMagic('K'));
}

AttackInfo Swordsman::createAttackInfo(Attack *attack)
{
    if (dynamic_cast<AttackHalfCircleSlash *>(attack))
    {
        const auto originPoint = getOriginPoint();
        const GEB::Pair<int, float> props[] = {
            {AttackInfo::Damage, 40.0f},
            {AttackInfo::Duration, 1.0f},
            {AttackInfo::HitStartTime, 0.2f},
            {AttackInfo::HitEndTime, 0.4f},
            {AttackInfo::HitBoxWidth, 120.0f},
            {AttackInfo::HitBoxHeight, 20.0f},
            {AttackInfo::OriginPointX, originPoint.x},
            {AttackInfo::OriginPointY, originPoint.y},
            {AttackInfo::Direction, static_cast<float>(direction())},
        };
        return AttackInfo(props, sizeof(props) / sizeof(props[0]));
    }

    if (dynamic_cast<AttackAutoTargetingMagic *>(attack))
    {
        const GEB::Pair<int, float> props[] = {
            {AttackInfo::Damage, 40.0f},
            {AttackInfo::Duration, 1.0f},
            {AttackInfo::HitStartTime, 0.1f},
            {AttackInfo::HitEndTime, 0.5f},
            {AttackInfo::AttackRaidus, 200.0f},
            {AttackInfo::AttackedCount, 3},
            {AttackInfo::RequiredMana, 10},
        };
        return AttackInfo(props, sizeof(props) / sizeof(props[0]));
    }

    return AttackInfo{};
}
