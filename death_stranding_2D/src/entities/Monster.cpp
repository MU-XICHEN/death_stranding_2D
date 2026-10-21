#include "entities/Monster.h"
#include "Core/GameBase.h"
#include "world/World.h"

float CopySign(float value, float signSource)
{
    float magnitude = value < 0.0f ? -value : value;
    return signSource < 0.0f ? -magnitude : magnitude;
}

// Monster

GEB::DirectionVec Monster::calculateDirVecToHero(GEB::Position monsterCenter, GEB::Position heroCenter, float moveDistance)
{
    float dx = heroCenter.x - monsterCenter.x;
    float dy = heroCenter.y - monsterCenter.y;

    float positiveDx = GEB::Math::abs(heroCenter.x - monsterCenter.x);
    float positiveDy = GEB::Math::abs(heroCenter.y - monsterCenter.y);

    float xPercent = positiveDx / (positiveDx + positiveDy);
    float yPercent = positiveDy / (positiveDx + positiveDy);

    return GEB::DirectionVec{
        CopySign(moveDistance * xPercent, dx),
        CopySign(moveDistance * yPercent, dy)};
}

void Monster::checkDangerDistance(World &world, Layer &layer)
{
    return; // todo: uncomment

    if (awakened_)
        return;
    GEB::Position heroCenter = world.gameInfo().heroInfo().center();
    float dx = GEB::Math::abs(heroCenter.x - center().x);
    float dy = GEB::Math::abs(heroCenter.y - center().y);

    if (dx <= dangerDistance_ && dy <= dangerDistance_)
    {
        awaken();
    }
}

// Beast
void Beast::move(World &world, Layer &layer)
{
    if (!awakened_)
        return;

    float dt = world.gameInfo().dt();
    float distance = dt * speed_;

    auto dirVec = calculateDirVecToHero(center(), world.gameInfo().heroInfo().center(), distance);

    x_ += dirVec.dx;
    y_ += dirVec.dy;
}

void Beast::update(World &world, Layer &layer)
{
    checkDangerDistance(world, layer);
    move(world, layer);
}

// Ghost
void Ghost::move(World &world, Layer &layer)
{
    // Todo: ghosts have their own movement manner.
    if (!awakened_)
        return;

    float dt = world.gameInfo().dt();
    float distance = dt * speed_;

    auto dirVec = calculateDirVecToHero(center(), world.gameInfo().heroInfo().center(), distance);

    x_ += dirVec.dx;
    y_ += dirVec.dy;
}

void Ghost::checkDangerDistance(World &world, Layer &layer)
{
    return; // todo: uncomment

    if (awakened_)
        return;

    GEB::Position heroCenter = world.gameInfo().heroInfo().center();
    float dx = GEB::Math::abs(heroCenter.x - center().x);
    float dy = GEB::Math::abs(heroCenter.y - center().y);

    if (dx <= dangerDistance_ && dy <= dangerDistance_)
    {
        appear();
        awaken();
    }
}

void Ghost::update(World &world, Layer &layer)
{
    checkDangerDistance(world, layer);
    move(world, layer);
}

void Ghost::render(World &world, Layer &layer)
{
    if (!visible_)
        return;

    Monster::render(world, layer);
}