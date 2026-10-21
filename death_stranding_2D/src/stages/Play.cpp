#include "stages/Play.h"
#include "entities/Hero.h"
#include "world/Viewport.h"
#include "entities/Monster.h"
#include "world/Map.h"

PlayStage::PlayStage()
{
    // ItemEntity initialization
    auto hero = new Hero(
        START_POSITION_X,
        START_POSITION_Y,
        ROLE_SIZE,
        ROLE_SIZE,
        PIXEL_PER_SEC);
    hero->setColor(255, 0, 0);
    pushEntityToItemEntityList(hero);

    auto beast1 = new Beast(
        START_POSITION_X,
        500,
        ROLE_SIZE,
        ROLE_SIZE);
    beast1->setColor(0, 0, 255);
    pushEntityToItemEntityList(beast1);

    // auto beast2 = new Beast(
    //     500,
    //     START_POSITION_Y,
    //     ROLE_SIZE,
    //     ROLE_SIZE);
    // beast2->setColor(0, 0, 255);
    // pushEntityToItemEntityList(beast2);

    // auto ghost1 = new Ghost(
    //     300,
    //     300,
    //     ROLE_SIZE,
    //     ROLE_SIZE);
    // ghost1->setColor(0, 255, 0);
    // pushEntityToItemEntityList(ghost1);

    // add map
    auto map = new Map(
        0,
        0,
        MAP_WIDTH,
        MAP_HEIGHT,
        MAP_BORDER_SIZE,
        GEB::Color(255, 255, 255));
    pushEntity(map);
}

PlayStage::~PlayStage()
{
    if (itemEntityList_.size() == 0)
        return;

    for (int i = 0; i < itemEntityList_.size(); ++i)
    {
        if (itemEntityList_[i] == nullptr)
            continue;
        delete itemEntityList_[i];
        itemEntityList_[i] = nullptr;
    }
}

void PlayStage::pushEntityToItemEntityList(ItemEntity *itemEntity)
{
    itemEntityList_.push_back(itemEntity);
}

// updateEntities
void PlayStage::updateEntities(World &world, Layer &layer)
{
    Stage::updateEntities(world, layer);

    if (itemEntityList_.size() == 0)
        return;

    for (int i = 0; i < itemEntityList_.size(); ++i)
    {
        if (itemEntityList_[i])
            itemEntityList_[i]->update(world, layer);
    }
}

// renderEntities
void PlayStage::renderEntities(World &world, Layer &layer)
{
    if (itemEntityList_.size() == 0)
        return;

    for (int i = 0; i < itemEntityList_.size(); ++i)
    {
        if (itemEntityList_[i])
            itemEntityList_[i]->render(world, layer);
    }

    Stage::renderEntities(world, layer);
}
