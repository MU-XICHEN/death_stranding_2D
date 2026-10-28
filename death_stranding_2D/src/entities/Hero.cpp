#include "entities/Hero.h"
#include "world/World.h"
#include "core/GameBase.h"
#include "GamesEngineeringBase.h"
#include "core/Types.h"
#include "layers/MainLayer.h"
#include "stages/Play.h"
#include "skills/Attack.h"
#include "skills/HalfCircleSlash.h"
#include "utils/Utils.h"

template <typename T>
using GEBArray = GEB::Structures::Array<T>;

namespace GamesEngineeringBase::Utils
{
    GEB::Vec2 translateEventsToDirectionVec2(const GamesEngineeringBase::Window &canvas);
    GEB::Direction translateEventsToDirection(const GamesEngineeringBase::Window &canvas, GEB::Direction prevDirection);
}

void Hero::handleMovement(World &world, Layer &layer)
{
    const MapInfo &map = world.gameInfo().mapInfo();
    auto &canvas = world.canvas();
    float dt = world.gameInfo().dt();
    float distance = dt * speed_;

    auto &mainLayer = dynamic_cast<MainLayer &>(layer);

    auto &playStage = dynamic_cast<PlayStage &>(mainLayer.currentStage());

    auto &gridSystem = playStage.gridSystem();

    direction_ = GEB::Utils::translateEventsToDirection(canvas, direction_);
    auto distanceVec = GEB::Math::normalize(GEB::Utils::translateEventsToDirectionVec2(canvas)) * distance;

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

void Hero::tryTiggerAttacks(World &world, Layer &layer)
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
            attacks_[i]->begin();
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

AttackInfo Hero::createAttackInfo(Attack *attack)
{
    if (dynamic_cast<HalfCircleSlash *>(attack))
    {
        return AttackInfo{
            .damage = 10,
            .duration = 0.3f,
            .hitStartTime = 0.06f,
            .hitEndTime = 0.14f,
            .hitBoxWidth = 80,
            .hitBoxHeight = 30,
            .originPoint = getOriginPoint(),
            .direction = direction(),
        };
    }
    return AttackInfo{};
}
