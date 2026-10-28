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
GEB::Direction translateEventsToDirection(const GamesEngineeringBase::Window &canvas);

void Hero::handleEvents(World &world, Layer &layer)
{
    handleMovement(world, layer);
    handleSkills(world, layer);
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

    direction_ = translateEventsToDirection(canvas);
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

void Hero::handleSkills(World &world, Layer &layer)
{
    auto &canvas = world.canvas();

    auto &mainLayer = dynamic_cast<MainLayer &>(layer);
    auto &playStage = dynamic_cast<PlayStage &>(mainLayer.currentStage());
    auto &gridSystem = playStage.gridSystem();

    auto dt = world.gameInfo().dt();

    GEB::Boundary mapBoundary{
        .top = 0,
        .bottom = static_cast<float>(playStage.map().height()),
        .left = 0,
        .right = static_cast<float>(playStage.map().width()),
    };

    for (int i = 0; i < attacks_.size(); i++)
    {
        auto *attack = attacks_[i];
        if (!attack)
            continue;

        attack->updateElapsedTime(dt);

        // attackContainer is used for filtering candidate itemlist which might handle the attack
        // The actual hit detection happens in 'virtual Attack::tryHit' method.
        // pointers in list do not need to be owned, just references to existing entities, like 'this'.
        AABB attackContainer = attack->getAttackContainer(world);
        GEBArray<Entity *> candidateItemList;
        gridSystem.getCandidateListByAABB(this, attackContainer, mapBoundary, candidateItemList);

        // try hit
        for (int j = 0; j < candidateItemList.size(); ++j)
        {
            auto *entity = dynamic_cast<ItemEntity *>(candidateItemList[j]);
            if (!entity)
                continue;

            bool hitted = attack->tryHit(world, *this, reinterpret_cast<std::uintptr_t>(entity));

            if (hitted)
            {
                const auto &attackInfo = attack->attackInfo();
                entity->onAttack(attackInfo);
            }
        }
    }

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
    // handle keyboard events for hero
    handleEvents(world, layer);

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
            .damage = 10.0f,
            .duration = 0.3f,
            .hitStartTime = 0.06f,
            .hitEndTime = 0.14f,
            .hitBoxWidth = 80,
            .hitBoxHeight = 30,
            .angleSpeed = 3000.0f, // 3000 ° per second
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

GEB::Direction translateEventsToDirection(const GamesEngineeringBase::Window &canvas)
{
    if (canvas.keyPressed('W') && canvas.keyPressed('A'))
        return GEB::Direction::UpLeft;
    if (canvas.keyPressed('W') && canvas.keyPressed('D'))
        return GEB::Direction::UpRight;
    if (canvas.keyPressed('S') && canvas.keyPressed('A'))
        return GEB::Direction::DownLeft;
    if (canvas.keyPressed('S') && canvas.keyPressed('D'))
        return GEB::Direction::DownRight;
    if (canvas.keyPressed('W'))
        return GEB::Direction::Up;
    if (canvas.keyPressed('S'))
        return GEB::Direction::Down;
    if (canvas.keyPressed('A'))
        return GEB::Direction::Left;
    if (canvas.keyPressed('D'))
        return GEB::Direction::Right;

    return GEB::Direction::None;
}

GEB::Vec2 translateEventsToDirectionVec2(const GamesEngineeringBase::Window &canvas)
{
    // wsad
    if (canvas.keyPressed('W') && canvas.keyPressed('A'))
        return GEB::Vec2{-1, -1};
    if (canvas.keyPressed('W') && canvas.keyPressed('D'))
        return GEB::Vec2{1, -1};
    if (canvas.keyPressed('S') && canvas.keyPressed('A'))
        return GEB::Vec2{-1, 1};
    if (canvas.keyPressed('S') && canvas.keyPressed('D'))
        return GEB::Vec2{1, 1};
    if (canvas.keyPressed('W'))
        return GEB::Vec2{0, -1};
    if (canvas.keyPressed('S'))
        return GEB::Vec2{0, 1};
    if (canvas.keyPressed('A'))
        return GEB::Vec2{-1, 0};
    if (canvas.keyPressed('D'))
        return GEB::Vec2{1, 0};

    // up\down\left\right keys
    if (canvas.keyPressed(VK_UP) && canvas.keyPressed(VK_LEFT))
        return GEB::Vec2{-1, -1};
    if (canvas.keyPressed(VK_UP) && canvas.keyPressed(VK_RIGHT))
        return GEB::Vec2{1, -1};
    if (canvas.keyPressed(VK_DOWN) && canvas.keyPressed(VK_LEFT))
        return GEB::Vec2{-1, 1};
    if (canvas.keyPressed(VK_DOWN) && canvas.keyPressed(VK_RIGHT))
        return GEB::Vec2{1, 1};
    if (canvas.keyPressed(VK_UP))
        return GEB::Vec2{0, -1};
    if (canvas.keyPressed(VK_DOWN))
        return GEB::Vec2{0, 1};
    if (canvas.keyPressed(VK_LEFT))
        return GEB::Vec2{-1, 0};
    if (canvas.keyPressed(VK_RIGHT))
        return GEB::Vec2{1, 0};

    return GEB::Vec2{0, 0};
}