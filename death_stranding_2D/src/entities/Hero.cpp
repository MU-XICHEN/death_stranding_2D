#include <limits>
#include "entities/Hero.h"
#include "GamesEngineeringBase.h"
#include "world/World.h"
#include "core/GameBase.h"
#include "utils/Utils.h"
#include "layers/MainLayer.h"
#include "stages/Play.h"
#include "skills/Attack.h"
#include "skills/AttackHalfCircleSlash.h"
#include "skills/AttackRemoteMulti.h"

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

    void tryTriggerAttacks(World &world, Layer &layer)
    {
        if (host_ == nullptr)
            return;
        // try begin attacks
        for (int i = 0; i < host_->attacks_.size(); ++i)
        {
            auto *attack = host_->attacks_[i];
            if (!attack)
                continue;
            attack->tryBegin(world, layer);
        }
    }

    void tryTriggerSkills(World &world, Layer &layer)
    {
        if (host_ == nullptr)
            return;
        auto &canvas = world.canvas();
        for (int i = 0; i < host_->skills_.size(); ++i)
        {
            if (!host_->skills_[i])
                continue;
            if (canvas.keyPressed(host_->skills_[i]->triggerKey()))
            {
                if (host_->skills_[i]->active())
                    continue;

                host_->skills_[i]->activate(host_->state_);
            }
        }
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
    GEB::Pair<int, float> props[] = {
        {State::HEALTH, HERO_HP},
        {State::SPEED, HERO_PIXEL_PER_SEC},
        {State::PERCEPTION_RANGE, HERO_DANGER_DISTANCE},
        {State::DEFENSE, HERO_DEFENSE},
        {State::Damage, HERO_DAMAGE},
    };
    initializeState(props, sizeof(props) / sizeof(props[0]));
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
    gridSystem.getCollisionCandidateListByItem(this, distanceVec, candidateItemList);

    // add map tiles which may collide with the hero to the candidate list
    playStage.map().addCollisionTileToCandiListByDisVec(this,
                                                        distanceVec,
                                                        candidateItemList);

    // get old position
    auto oldCenter = center();

    // move
    moveWithCollision(distanceVec, map, candidateItemList.head(), candidateItemList.size());

    // update grid system
    gridSystem.updateEntityGridIfNeed(oldCenter, this);
}

void Hero::tryTriggerAttacks(World &world, Layer &layer)
{
    if (impl_ == nullptr)
        return;
    impl_->tryTriggerAttacks(world, layer);
}

void Hero::tryTriggerSkills(World &world, Layer &layer)
{
    if (impl_ == nullptr)
        return;
    impl_->tryTriggerSkills(world, layer);
}

void Hero::update(World &world, Layer &layer)
{
    ItemEntity::coreUpdate(world, layer);

    if (!active())
        return;

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
    const auto originPoint = getOriginPoint();
    if (dynamic_cast<AttackHalfCircleSlash *>(attack))
    {
        GEB::Pair<int, float> props[] = {
            {AttackInfo::Damage, 10.0f},
            // lifetime of the attack
            {AttackInfo::Duration, 1.0f},
            {AttackInfo::HitStartTime, 0.2f},
            {AttackInfo::HitEndTime, 0.4f},
            // hit box dimensions
            {AttackInfo::HitBoxWidth, 80.0f},
            {AttackInfo::HitBoxHeight, 30.0f},
            // set point and direction of the attack
            {AttackInfo::OriginPointX, originPoint.x},
            {AttackInfo::OriginPointY, originPoint.y},
            {AttackInfo::Direction, static_cast<float>(direction())},
        };
        int size = sizeof(props) / sizeof(props[0]);
        AttackInfo attackInfo(props, size);
        return attackInfo;
    }

    if (dynamic_cast<AttackRemoteMulti *>(attack) || dynamic_cast<AttackRemote *>(attack))
    {
        // max float value
        const float infinityFloat = std::numeric_limits<float>::max();
        const GEB::Pair<int, float> props[] = {
            {AttackInfo::Damage, 15.0f},
            {AttackInfo::Duration, infinityFloat},
            {AttackInfo::HitStartTime, 0.0f},
            {AttackInfo::HitEndTime, infinityFloat},
            // remote hitEndTime is set to match the duration of the attack
            {AttackInfo::OriginPointX, originPoint.x},
            {AttackInfo::OriginPointY, originPoint.y},
            {AttackInfo::Direction, static_cast<float>(direction())},
            {AttackInfo::RangedAttackSpeed, 300.0f},
        };
        int size = sizeof(props) / sizeof(props[0]);
        AttackInfo attackInfo(props, size);
        return attackInfo;
    }

    return AttackInfo{};
}
