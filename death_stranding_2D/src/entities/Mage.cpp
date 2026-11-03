#include "entities/Mage.h"
#include "skills/AttackAutoTargetingMagic.h"

Mage::Mage(float x, float y, int width, int height)
    : Hero(x, y, width, height)
{
    addAttack(new AttackAutoTargetingMagic('J'));
}

AttackInfo Mage::createAttackInfo(Attack *attack)
{
    if (dynamic_cast<AttackAutoTargetingMagic *>(attack))
    {
        const GEB::Pair<int, float> props[] = {
            {AttackInfo::Damage, 20.0f},
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
