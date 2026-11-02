#include "stages/Play.h"
#include "core/GameBase.h"
#include "entities/Hero.h"
#include "entities/Monster.h"
#include "map/Map.h"
#include "layers/MainLayer.h"
#include "world/World.h"
#include "skills/AttackHalfCircleSlash.h"
#include "skills/AttackRemoteMulti.h"
#include "skills/AttackAutoTargetingMagic.h"
#include "skills/SkillSpeedUp.h"
#include "entities/HealthPotion.h"
#include "entities/ManaPotion.h"
#include "entities/Track.h"
#include "skills/AttackCollision.h"

PlayStage::PlayStage()
{
    map_ = new Map(MapType::Chapter1);
    if (!map_)
        return;
    gridSystem_ = new GridSystem(
        map_->height() / GRID_SIZE,
        map_->width() / GRID_SIZE,
        GRID_SIZE);

    initialize();
}

PlayStage::~PlayStage()
{
    delete map_;
    delete gridSystem_;
}

void PlayStage::initialize()
{
    if (!gridSystem_ || !map_)
        return;
    auto mapCenter = map_->center();

    // add Items
    // [new] items will be recycled by the grid system
    auto hero = new Hero(
        mapCenter.x,
        mapCenter.y,
        ROLE_SIZE,
        ROLE_SIZE);
    hero->setColor(255, 0, 0);
    // attacks
    auto *attack = new AttackHalfCircleSlash('J', true);
    hero->addAttack(attack);
    auto *remoteAttack = new AttackRemoteMulti('K', 0.5f);
    hero->addAttack(remoteAttack);
    auto *autoTargetingMagic = new AttackAutoTargetingMagic('L');
    hero->addAttack(autoTargetingMagic);
    // skills
    auto *speedUpSkill = new SkillSpeedUp('U');
    hero->addSkill(speedUpSkill);
    addEntityToGridSystem(hero);
}

GEB::Pair<int, int> getGridIndex(float x, float y)
{
    GEB::Pair<int, int> index;
    int rolIdx = GEB::Math::floorToInt(y / GRID_SIZE);
    int corIdx = GEB::Math::floorToInt(x / GRID_SIZE);

    index.first = rolIdx;
    index.second = corIdx;
    return index;
}

void PlayStage::addEntityToGridSystem(ItemEntity *item)
{
    if (!gridSystem_ || !item)
        return;

    auto center = item->center();
    auto indexPair = getGridIndex(center.x, center.y);
    gridSystem_->addEntity(indexPair.first, indexPair.second, item);
}

// updateEntities
void PlayStage::update(World &world, Layer &layer)
{
    if (!gridSystem_ || !map_)
        return;

    world.gameInfo().updateMapInfo(*map_);

    Stage::update(world, layer); // update sections like information panels

    auto &itemsToUpdate = gridSystem_->itemsToUpdate();
    int itemCount = itemsToUpdate.size();
    for (int i = 0; i < itemCount; ++i)
    {
        auto *item = itemsToUpdate[i];
        if (item)
            item->update(world, layer);
    }
}

// renderEntities
void PlayStage::render(World &world, Layer &layer)
{
    if (!gridSystem_ || !map_)
        return;

    map_->render(world, layer);

    Stage::render(world, layer);

    auto &itemsToUpdate = gridSystem_->itemsToUpdate();
    int itemCount = itemsToUpdate.size();
    for (int i = 0; i < itemCount; ++i)
    {
        auto *item = itemsToUpdate[i];
        if (item)
            item->render(world, layer);
    }
}

void PlayStage::renderFinished(World &world, Layer &layer)
{
    if (!gridSystem_)
        return;

    auto &inactiveItems = gridSystem_->inactiveItems();
    int inactiveItemCount = inactiveItems.size();
    for (int i = 0; i < inactiveItemCount; ++i)
    {
        auto *item = inactiveItems[i];
        if (!item)
            continue;
        const auto &center = item->center();
        auto [rowIdx, colIdx] = gridSystem_->getGridIdxPair(center);
        gridSystem_->removeEntity(rowIdx, colIdx, item);
    }

    // clear
    gridSystem_->clearInactiveItems();
}
