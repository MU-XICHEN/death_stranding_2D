#include "entities/Hero.h"
#include "world/World.h"
#include "core/GameBase.h"
#include "GamesEngineeringBase.h"
#include "core/Types.h"
#include "layers/MainLayer.h"
#include "stages/Play.h"
#include "skills/Attack.h"
#include "skills/AttackHalfCircleSlash.h"
#include "utils/Utils.h"

template <typename T>
using GEBArray = GEB::Structures::Array<T>;

namespace GamesEngineeringBase::Utils
{
    GEB::Vec2 translateEventsToDirectionVec2(const GamesEngineeringBase::Window &canvas);
    GEB::Direction translateEventsToDirection(const GamesEngineeringBase::Window &canvas, GEB::Direction prevDirection);
}

class HeroImpl
{
public:
    explicit HeroImpl(Hero *hero)
        : host_(hero) {}

    bool checkLockDirection() const
    {
        for (int i = 0; i < host_->attacks_.size(); ++i)
        {
            if (!host_->attacks_[i])
                continue;
            if (host_->attacks_[i]->lockDirection())
                return true;
        }
        return false;
    }

private:
    Hero *host_;
};

Hero::Hero(
    float x,
    float y,
    int width,
    int height)
    : ItemEntity(x, y, width, height),
      impl_(new HeroImpl(this))
{
    GEB::Pair<int, float> props[State::propCount] = {
        {State::HEALTH, 100.0f},
        {State::SPEED, HERO_PIXEL_PER_SEC},
        {State::PERCEPTION_RANGE, HERO_DANGER_DISTANCE},
        {State::DEFENSE, HERO_DEFENSE},
        {State::DAMAGE, HERO_DAMAGE},
    };
    initializeState(props, State::propCount);
}

Hero::~Hero()
{
    if (impl_)
        delete impl_;
    impl_ = nullptr;
}

void Hero::handleMovement(World &world, Layer &layer)
{
    const MapInfo &map = world.gameInfo().mapInfo();
    auto &canvas = world.canvas();
    float dt = world.gameInfo().dt();
    float distance = dt * state_.speed();

    auto &mainLayer = dynamic_cast<MainLayer &>(layer);

    auto &playStage = dynamic_cast<PlayStage &>(mainLayer.currentStage());

    auto &gridSystem = playStage.gridSystem();

    bool lockDirection = impl_->checkLockDirection();

    if (!lockDirection)
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

    // update grid system
    gridSystem.updateEntityGridIfNeed(oldCenter, this);
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
    if (dynamic_cast<AttackHalfCircleSlash *>(attack))
    {
        const auto &attackInfo = attack->attackInfo();
        return AttackInfo{
            .damage = attackInfo.damage,
            // lifetime of the attack
            .duration = 0.3f,
            .hitStartTime = 0.06f,
            .hitEndTime = 0.14f,
            // hit box dimensions
            .hitBoxWidth = 80,
            .hitBoxHeight = 30,
            // set point and direction of the attack
            .originPoint = getOriginPoint(),
            .direction = direction(),
        };
    }
    return AttackInfo{};
}
