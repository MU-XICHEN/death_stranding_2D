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

template <typename T>
using GEBArray = GEB::Structures::Array<T>;

namespace
{
    int spriteFrame(GEB::Direction direction)
    {
        switch (direction)
        {
        case GEB::Direction::Up:
            return 0;
        case GEB::Direction::UpRight:
            return 1;
        case GEB::Direction::Right:
            return 2;
        case GEB::Direction::DownRight:
            return 3;
        case GEB::Direction::Down:
            return 4;
        case GEB::Direction::DownLeft:
            return 5;
        case GEB::Direction::Left:
            return 6;
        case GEB::Direction::UpLeft:
            return 7;
        default:
            return 4;
        }
    }
}

namespace GamesEngineeringBase::Utils
{
    GEB::Vec2 translateEventsToDirectionVec2(const GamesEngineeringBase::Window &canvas);
    GEB::Direction translateEventsToDirection(const GamesEngineeringBase::Window &canvas, GEB::Direction prevDirection);
}

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
        {State::DAMAGE, HERO_DAMAGE},
        {State::MANA, HERO_MANA},
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
    auto *gridSystem = playStage.gridSystem();
    auto *stageMap = playStage.map();
    if (!gridSystem || !stageMap)
        return;

    bool lockDirection = impl_->checkLockDirection();

    if (!lockDirection)
        direction_ = GEB::Utils::translateEventsToDirection(canvas, direction_);

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

void Hero::render(World &world, Layer &layer)
{
    if (!active())
        return;
    if (!impl_)
        return;
    impl_->renderSprite(world, layer);

    for (int i = 0; i < attacks_.size(); ++i)
    {
        if (attacks_[i] && attacks_[i]->isActive())
            attacks_[i]->render(world, layer);
    }
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

bool Hero::onUsePotion(Potion *potion)
{
    state_.apply(potion->effectState());
    return true;
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

    void renderSprite(World &world, Layer &layer)
    {
        if (!host_)
            return;

        auto &canvas = world.canvas();
        auto *sheet = world.assetManager().getImage(host_->spriteAsset());
        if (!sheet)
            return;

        auto *buffer = canvas.getBackBuffer();
        if (!buffer)
            return;

        // Validate the eight-direction sheet and target size.
        int frameSize = ROLE_SIZE;
        int sheetColumns = 4;
        int sheetRows = 2;
        if (sheet->width != frameSize * sheetColumns)
            return;
        if (sheet->height != frameSize * sheetRows)
            return;

        int width = host_->width();
        int height = host_->height();

        // Select the frame for the current direction.
        int frame = spriteFrame(host_->direction());
        int sourceLeft = (frame % sheetColumns) * frameSize;
        int sourceTop = (frame / sheetColumns) * frameSize;

        // Convert world coordinates to screen coordinates.
        auto &viewport = world.gameInfo().viewport();
        int left = static_cast<int>(host_->x() - viewport.x());
        int top = static_cast<int>(host_->y() - viewport.y());
        int canvasWidth = static_cast<int>(canvas.getWidth());
        int visibleWidth = std::min(viewport.width(), canvasWidth);
        int visibleHeight = std::min(viewport.height(), static_cast<int>(canvas.getHeight()));

        // Clip drawing to the visible area.
        int drawLeft = std::max(0, left);
        int drawTop = std::max(0, top);
        int drawRight = std::min(visibleWidth, left + width);
        int drawBottom = std::min(visibleHeight, top + height);
        if (drawLeft >= drawRight)
            return;
        if (drawTop >= drawBottom)
            return;

        // Draw the sprite frame for the current direction.
        for (int y = drawTop; y < drawBottom; ++y)
        {
            int sourceY = sourceTop + (y - top) * frameSize / height;
            for (int x = drawLeft; x < drawRight; ++x)
            {
                int sourceX = sourceLeft + (x - left) * frameSize / width;
                int alpha = sheet->alphaAt(sourceX, sourceY);
                auto *pixel = sheet->at(sourceX, sourceY);
                auto *destination = buffer + (y * canvasWidth + x) * 3;
                GEB::Utils::blendPixel(destination, pixel, alpha);
            }
        }
    }

private:
    Hero *host_;
};
