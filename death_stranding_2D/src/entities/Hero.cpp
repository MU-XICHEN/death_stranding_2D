#include "entities/Hero.h"
#include "entities/Monster.h"
#include "entities/Potion.h"
#include "GamesEngineeringBase.h"
#include "world/World.h"
#include "core/GameBase.h"
#include "utils/Utils.h"
#include "layers/MainLayer.h"
#include "stages/Play.h"
#include "skills/Attack.h"
#include "skills/SkillSpeedUp.h"

template <typename T>
using GEBArray = GEB::Structures::Array<T>;

namespace GamesEngineeringBase::Utils
{
    GEB::Vec2 translateEventsToDirectionVec2(const GamesEngineeringBase::Window &canvas);
    GEB::Direction translateEventsToDirection(const GamesEngineeringBase::Window &canvas, GEB::Direction prevDirection);
}

bool Hero::checkLockDirection() const
{
    for (int i = 0; i < attacks_.size(); ++i)
    {
        if (!attacks_[i])
            continue;
        if (attacks_[i]->lockDirection())
            return true;
    }
    return false;
}

Hero::Hero(
    float x,
    float y,
    int width,
    int height)
    : ItemEntity(x, y, width, height)
{
    GEB::Pair<int, float> props[] = {
        {State::HEALTH, HERO_HP},
        {State::SPEED, HERO_PIXEL_PER_SEC},
        {State::PERCEPTION_RANGE, HERO_DANGER_DISTANCE},
        {State::DEFENSE, HERO_DEFENSE},
        {State::MANA, HERO_MANA},
    };
    initializeState(props, sizeof(props) / sizeof(props[0]));
    addSkill(new SkillSpeedUp('L'));
}

void Hero::handleMovement(World &world, Layer &layer)
{
    const MapInfo &map = world.gameInfo().mapInfo();
    auto &canvas = world.canvas();
    float dt = world.gameInfo().dt();
    float distance = dt * state_.speed();

    auto &mainLayer = dynamic_cast<MainLayer &>(layer);
    auto &playStage = dynamic_cast<PlayStage &>(mainLayer.currentStage());
    auto *gridSystem = playStage.gridSystem();
    auto *stageMap = playStage.map();
    if (!gridSystem || !stageMap)
        return;

    // Keep facing the same way while an attack locks the direction.
    bool lockDirection = checkLockDirection();

    if (!lockDirection)
        direction_ = GEB::Utils::translateEventsToDirection(canvas, direction_);

    // Keep diagonal movement at the same speed as straight movement.
    auto distanceVec = GEB::Math::normalize(GEB::Utils::translateEventsToDirectionVec2(canvas)) * distance;

    // get candidate items for collision detection
    GEBArray<Entity *> candidateItemList;
    gridSystem->getCandidateListByItem(this, distanceVec, candidateItemList);

    // add map tiles which may collide with the hero to the candidate list
    stageMap->addCollisionTileToCandiListByDisVec(this,
                                                  distanceVec,
                                                  candidateItemList);

    // get old position
    auto oldCenter = center();

    // move
    moveWithCollision(distanceVec, map, candidateItemList.head(), candidateItemList.size());

    // update grid system
    gridSystem->updateEntityGridIfNeed(oldCenter, this);
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

void Hero::onTargetDefeated(const ItemEntity &target)
{
    if (dynamic_cast<const Monster *>(&target))
    {
        ++defeatedMonsterCount_;
        const float newScore = state_.score() + target.state().score();
        state_.setScore(newScore);
    }
}

void Hero::onUsePotion(Potion *potion)
{
    if (!active() || state_.health() <= 0)
        return;
    // Apply the potion, then keep health and mana within their limits.
    state_.apply(potion->effectState());
    state_.setHealth(std::min(state_.health(), HERO_HP));
    state_.setMana(std::min(state_.mana(), HERO_MANA));
    return;
}
