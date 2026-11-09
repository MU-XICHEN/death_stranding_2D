#include <limits>
#include "entities/Archer.h"
#include "skills/AttackRemoteMulti.h"
#include "skills/AttackAutoTargetingMagic.h"

Archer::Archer(float x, float y, int width, int height)
    : Hero(x, y, width, height)
{
    // J fires arrows. K uses magic.
    addAttack(new AttackRemoteMulti('J', 0.5f));
    addAttack(new AttackAutoTargetingMagic('K'));
}

AttackInfo Archer::createAttackInfo(Attack *attack)
{
    if (dynamic_cast<AttackRemoteMulti *>(attack) || dynamic_cast<AttackRemote *>(attack))
    {
        const auto originPoint = getOriginPoint();
        const float infinityFloat = std::numeric_limits<float>::max();
        const GEB::Pair<int, float> props[] = {
            {AttackInfo::Damage, 20.0f},
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
