#include "entities/Monster.h"
#include "skills/AttackCollision.h"

Beast::Beast(float x, float y, int width, int height)
    : Monster(x, y, width, height)
{
    GEB::Pair<int, float> props[] = {
        {State::HEALTH, M_BEAST_HP},
        {State::SCORE, M_BEAST_SCORE},
        {State::SPEED, M_BEAST_PIXEL_PER_SEC},
        {State::PERCEPTION_RANGE, M_BEAST_DANGER_DISTANCE},
        {State::DEFENSE, M_BEAST_DEFENSE}};
    initializeState(props, sizeof(props) / sizeof(props[0]));
    addAttack(new AttackCollision(0));
}

AttackInfo Beast::createAttackInfo(Attack *attack)
{
    if (!dynamic_cast<AttackCollision *>(attack))
        return AttackInfo{};

    GEB::Pair<int, float> props[] = {
        {AttackInfo::Damage, M_BEAST_COLLITION_DAMAGE},
        {AttackInfo::Duration, 2.0f},
        {AttackInfo::HitStartTime, 0.0f},
        {AttackInfo::HitEndTime, 0.5f},
        {AttackInfo::HitBoxWidth, static_cast<float>(width_)},
        {AttackInfo::HitBoxHeight, static_cast<float>(height_)},
        {AttackInfo::OriginPointX, x_},
        {AttackInfo::OriginPointY, y_},
    };
    return AttackInfo(props, sizeof(props) / sizeof(props[0]));
}
