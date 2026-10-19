#include "entities/Monster.h"
#include "Core/GameBase.h"

GEB::Direction Monster::calculateDirectionToHero(GEB::Position monsterCenter, GEB::Position heroCenter)
{
    if (monsterCenter.x > heroCenter.x && monsterCenter.y > heroCenter.y)
        return GEB::Direction::UpLeft;
    if (monsterCenter.x < heroCenter.x && monsterCenter.y > heroCenter.y)
        return GEB::Direction::UpRight;
    if (monsterCenter.x > heroCenter.x && monsterCenter.y < heroCenter.y)
        return GEB::Direction::DownLeft;
    if (monsterCenter.x < heroCenter.x && monsterCenter.y < heroCenter.y)
        return GEB::Direction::DownRight;
    if (monsterCenter.x == heroCenter.x && monsterCenter.y > heroCenter.y)
        return GEB::Direction::Up;
    if (monsterCenter.x == heroCenter.x && monsterCenter.y < heroCenter.y)
        return GEB::Direction::Down;
    if (monsterCenter.x > heroCenter.x && monsterCenter.y == heroCenter.y)
        return GEB::Direction::Left;
    if (monsterCenter.x < heroCenter.x && monsterCenter.y == heroCenter.y)
        return GEB::Direction::Right;

    return GEB::Direction::None;
}
