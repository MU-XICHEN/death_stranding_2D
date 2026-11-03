#include <limits>
#include "entities/Archer.h"
#include "skills/AttackRemoteMulti.h"

Archer::Archer(float x, float y, int width, int height)
    : Hero(x, y, width, height)
{
    addAttack(new AttackRemoteMulti('J', 0.5f));
}

AttackInfo Archer::createAttackInfo(Attack *attack)
{
    if (dynamic_cast<AttackRemoteMulti *>(attack) || dynamic_cast<AttackRemote *>(attack))
    {
        const auto originPoint = getOriginPoint();
        const float infinityFloat = std::numeric_limits<float>::max();
        const GEB::Pair<int, float> props[] = {
            {AttackInfo::Damage, 15.0f},
            {AttackInfo::Duration, infinityFloat},
            {AttackInfo::HitStartTime, 0.0f},
            {AttackInfo::HitEndTime, infinityFloat},
            {AttackInfo::OriginPointX, originPoint.x},
            {AttackInfo::OriginPointY, originPoint.y},
            {AttackInfo::Direction, static_cast<float>(direction())},
            {AttackInfo::RangedAttackSpeed, 300.0f},
        };
        return AttackInfo(props, sizeof(props) / sizeof(props[0]));
    }

    return AttackInfo{};
}
