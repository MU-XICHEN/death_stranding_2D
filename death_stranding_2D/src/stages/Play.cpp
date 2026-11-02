#include "stages/Play.h"
#include "core/GameBase.h"
#include "entities/Hero.h"
#include "entities/Monster.h"
#include "map/Map.h"
#include "map/MapFactory.h"
#include "layers/MainLayer.h"
#include "world/World.h"
#include "skills/AttackHalfCircleSlash.h"
#include "skills/AttackRemoteMulti.h"
#include "skills/AttackAutoTargetingMagic.h"
#include "skills/SkillSpeedUp.h"

PlayStage::PlayStage()
    : gridSystem_(GRID_ROWCNT, GRID_COLCNT, GRID_SIZE),
      map_(MapType::Chapter1)
{
    initialize();
}

void PlayStage::initialize()
{
    mapUpdated_ = true;

    auto heroPosition = map_.playerPosition();

    // add Items
    // [new] items will be recycled by the grid system
    auto hero = new Hero(
        heroPosition.x,
        heroPosition.y,
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

    auto monster = new Beast(
        heroPosition.x + 100,
        heroPosition.y + 100,
        ROLE_SIZE,
        ROLE_SIZE);
    monster->setColor(0, 255, 0);
    addEntityToGridSystem(monster);

    auto monster2 = new Beast(
        heroPosition.x - 100,
        heroPosition.y - 100,
        ROLE_SIZE,
        ROLE_SIZE);
    monster2->setColor(0, 255, 0);
    addEntityToGridSystem(monster2);
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
    auto center = item->center();
    auto indexPair = getGridIndex(center.x, center.y);
    gridSystem_.addEntity(indexPair.first, indexPair.second, item);
}

// updateEntities
void PlayStage::update(World &world, Layer &layer)
{
    Stage::handleEvents(world, layer);

    if (mapUpdated_)
    {
        world.gameInfo().updateMapInfo(map_);
        mapUpdated_ = false;
    }

    Stage::update(world, layer); // update sections like information panels

    auto &itemsToUpdate = gridSystem_.itemsToUpdate();
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
    map_.render(world, layer);

    Stage::render(world, layer);

    auto &itemsToUpdate = gridSystem_.itemsToUpdate();
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
    auto &inactiveItems = gridSystem_.inactiveItems();
    int inactiveItemCount = inactiveItems.size();
    for (int i = 0; i < inactiveItemCount; ++i)
    {
        auto *item = inactiveItems[i];
        if (!item)
            continue;
        const auto &center = item->center();
        auto [rowIdx, colIdx] = gridSystem_.getGridIdxPair(center);
        gridSystem_.removeEntity(rowIdx, colIdx, item);
    }

    // clear
    gridSystem_.clearInactiveItems();
}
