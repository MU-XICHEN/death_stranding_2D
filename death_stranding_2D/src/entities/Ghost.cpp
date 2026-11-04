#include "entities/Monster.h"
#include "world/World.h"
#include "layers/Layer.h"

#include "skills/AttackCollision.h"

Ghost::Ghost(float x, float y, int width, int height)
    : Monster(x, y, width, height)
{
    GEB::Pair<int, float> props[] = {
        {State::HEALTH, M_GHOST_HP},
        {State::SCORE, M_GHOST_SCORE},
        {State::SPEED, M_GHOST_PIXEL_PER_SEC},
        {State::PERCEPTION_RANGE, M_GHOST_DANGER_DISTANCE},
        {State::DEFENSE, M_GHOST_DEFENSE},
        {State::DAMAGE, M_GHOST_DAMAGE}};
    initializeState(props, sizeof(props) / sizeof(props[0]));
    addAttack(new AttackCollision(0));
}

AttackInfo Ghost::createAttackInfo(Attack *attack)
{
    if (!dynamic_cast<AttackCollision *>(attack))
        return AttackInfo{};

    GEB::Pair<int, float> props[] = {
        {AttackInfo::Damage, state_.damage()},
        {AttackInfo::Duration, 0.5f},
        {AttackInfo::HitStartTime, 0.0f},
        {AttackInfo::HitEndTime, 0.5f},
        {AttackInfo::HitBoxWidth, static_cast<float>(width_)},
        {AttackInfo::HitBoxHeight, static_cast<float>(height_)},
        {AttackInfo::OriginPointX, x_},
        {AttackInfo::OriginPointY, y_},
    };
    return AttackInfo(props, sizeof(props) / sizeof(props[0]));
}

void Ghost::checkDangerDistance(World &world, Layer &layer)
{
    if (awakened_)
        return;

    auto &hero = world.gameInfo().heroInfo();
    auto heroCenter = hero.center();
    float dx = GEB::Math::abs(heroCenter.x - center().x) - (width_ + hero.width) / 2.0f;
    float dy = GEB::Math::abs(heroCenter.y - center().y) - (height_ + hero.height) / 2.0f;

    if (dx <= state_.perceptionRange() && dy <= state_.perceptionRange())
    {
        appear();
        awaken();
    }
}

void Ghost::render(World &world, Layer &layer)
{
    if (!visible_)
        return;

    ItemEntity::render(world, layer);
}
