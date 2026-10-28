#include "entities/Hero.h"
#include "world/World.h"
#include "core/GameBase.h"
#include "GamesEngineeringBase.h"
#include "core/Types.h"
#include "layers/MainLayer.h"
#include "stages/Play.h"
#include "skills/Attack.h"
#include "skills/HalfCircleSlash.h"

template <typename T>
using GEBArray = GEB::Structures::Array<T>;

GEB::Vec2 translateEventsToDirectionVec2(const GamesEngineeringBase::Window &canvas);
GEB::Direction translateEventsToDirection(const GamesEngineeringBase::Window &canvas, GEB::Direction prevDirection);

void Hero::handleMovement(World &world, Layer &layer)
{
    const MapInfo &map = world.gameInfo().mapInfo();
    auto &canvas = world.canvas();
    float dt = world.gameInfo().dt();
    float distance = dt * speed_;

    auto &mainLayer = dynamic_cast<MainLayer &>(layer);

    auto &playStage = dynamic_cast<PlayStage &>(mainLayer.currentStage());

    auto &gridSystem = playStage.gridSystem();

    direction_ = translateEventsToDirection(canvas, direction_);
    auto distanceVec = GEB::Math::normalize(translateEventsToDirectionVec2(canvas)) * distance;

    // get candidate items for collision detection
    GEBArray<Entity *> candidateItemList;
    GEB::Boundary mapBoundary{
        .top = 0,
        .bottom = static_cast<float>(map.height()),
        .left = 0,
        .right = static_cast<float>(map.width()),
    };
    gridSystem.getCollisionCandidateListByItem(this, distanceVec, mapBoundary, candidateItemList);

    // add map tiles which may collide with the hero to the candidate list
    playStage.map().addCollisionTileToCandiList(this,
                                                distanceVec,
                                                candidateItemList);

    // get old position
    auto oldCenter = center();

    // move
    moveWithCollision(distanceVec, map, candidateItemList.head(), candidateItemList.size());

    // calculate new position
    auto newCenter = center();

    // update grid system
    playStage.gridSystem().updateEntityGridIfNeed(oldCenter, newCenter, this);
}

void Hero::tryTiggerSkills(World &world, Layer &layer)
{
    auto &canvas = world.canvas();
    // try begin attacks
    for (int i = 0; i < attacks_.size(); ++i)
    {
        if (!attacks_[i])
            continue;
        if (canvas.keyPressed(attacks_[i]->triggerKey()))
        {
            if (attacks_[i]->isActive())
                continue;

            // handle different types of attacks
            if (dynamic_cast<HalfCircleSlash *>(attacks_[i]))
                attacks_[i]->begin(createAttackInfo<HalfCircleSlash>());
        }
    }
}

void Hero::update(World &world, Layer &layer)
{
    ItemEntity::update(world, layer);

    HeroInfo heroInfo{
        .x = x_,
        .y = y_,
        .width = width_,
        .height = height_};

    world.gameInfo()
        .updateHeroInfo(heroInfo);

    world.gameInfo().updateViewport(heroInfo, world.gameInfo().mapInfo());
}

template <typename T>
AttackInfo Hero::createAttackInfo()
{
    if constexpr (std::is_same_v<T, HalfCircleSlash>)
    {
        AttackInfo slashAttackInfo{
            .damage = 10,
            .duration = 0.3f,
            .hitStartTime = 0.06f,
            .hitEndTime = 0.14f,
            .hitBoxWidth = 80,
            .hitBoxHeight = 30,
            .originPoint = getOriginPoint(),
            .direction = direction_,
        };
        return slashAttackInfo;
    }
    else
    {
        return AttackInfo{};
    }
}

GEB::Direction translateEventsToDirection(const GamesEngineeringBase::Window &canvas, GEB::Direction prevDirection)
{
    if (canvas.keyPressed('W') && canvas.keyPressed('A'))
        return GEB::Direction::UpLeft;
    else if (canvas.keyPressed('W') && canvas.keyPressed('D'))
        return GEB::Direction::UpRight;
    else if (canvas.keyPressed('S') && canvas.keyPressed('A'))
        return GEB::Direction::DownLeft;
    else if (canvas.keyPressed('S') && canvas.keyPressed('D'))
        return GEB::Direction::DownRight;
    else if (canvas.keyPressed('W'))
        return GEB::Direction::Up;
    else if (canvas.keyPressed('S'))
        return GEB::Direction::Down;
    else if (canvas.keyPressed('A'))
        return GEB::Direction::Left;
    else if (canvas.keyPressed('D'))
        return GEB::Direction::Right;

    // up\down\left\right keys
    else if (canvas.keyPressed(VK_UP) && canvas.keyPressed(VK_LEFT))
        return GEB::Direction::UpLeft;
    else if (canvas.keyPressed(VK_UP) && canvas.keyPressed(VK_RIGHT))
        return GEB::Direction::UpRight;
    else if (canvas.keyPressed(VK_DOWN) && canvas.keyPressed(VK_LEFT))
        return GEB::Direction::DownLeft;
    else if (canvas.keyPressed(VK_DOWN) && canvas.keyPressed(VK_RIGHT))
        return GEB::Direction::DownRight;
    else if (canvas.keyPressed(VK_UP))
        return GEB::Direction::Up;
    else if (canvas.keyPressed(VK_DOWN))
        return GEB::Direction::Down;
    else if (canvas.keyPressed(VK_LEFT))
        return GEB::Direction::Left;
    else if (canvas.keyPressed(VK_RIGHT))
        return GEB::Direction::Right;

    return prevDirection;
}

GEB::Vec2 translateEventsToDirectionVec2(const GamesEngineeringBase::Window &canvas)
{
    // wsad
    if (canvas.keyPressed('W') && canvas.keyPressed('A'))
        return GEB::Vec2{-1, -1};
    else if (canvas.keyPressed('W') && canvas.keyPressed('D'))
        return GEB::Vec2{1, -1};
    else if (canvas.keyPressed('S') && canvas.keyPressed('A'))
        return GEB::Vec2{-1, 1};
    else if (canvas.keyPressed('S') && canvas.keyPressed('D'))
        return GEB::Vec2{1, 1};
    else if (canvas.keyPressed('W'))
        return GEB::Vec2{0, -1};
    else if (canvas.keyPressed('S'))
        return GEB::Vec2{0, 1};
    else if (canvas.keyPressed('A'))
        return GEB::Vec2{-1, 0};
    else if (canvas.keyPressed('D'))
        return GEB::Vec2{1, 0};

    // up\down\left\right keys
    else if (canvas.keyPressed(VK_UP) && canvas.keyPressed(VK_LEFT))
        return GEB::Vec2{-1, -1};
    else if (canvas.keyPressed(VK_UP) && canvas.keyPressed(VK_RIGHT))
        return GEB::Vec2{1, -1};
    else if (canvas.keyPressed(VK_DOWN) && canvas.keyPressed(VK_LEFT))
        return GEB::Vec2{-1, 1};
    else if (canvas.keyPressed(VK_DOWN) && canvas.keyPressed(VK_RIGHT))
        return GEB::Vec2{1, 1};
    else if (canvas.keyPressed(VK_UP))
        return GEB::Vec2{0, -1};
    else if (canvas.keyPressed(VK_DOWN))
        return GEB::Vec2{0, 1};
    else if (canvas.keyPressed(VK_LEFT))
        return GEB::Vec2{-1, 0};
    else if (canvas.keyPressed(VK_RIGHT))
        return GEB::Vec2{1, 0};

    return GEB::Vec2{0, 0};
}