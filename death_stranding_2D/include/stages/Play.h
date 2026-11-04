#pragma once

#include "stages/Stage.h"
#include "entities/ItemEntity.h"
#include "world/Grid.h"
#include "map/Map.h"
#include "entities/Hero.h"

class PlayStage : public Stage
{
public:
    explicit PlayStage(HeroType heroType = HeroType::Swordsman);
    virtual ~PlayStage() override;

    PlayStage(const PlayStage &) = delete;
    PlayStage &operator=(const PlayStage &) = delete;

    void initialize();
    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;
    void renderFinished(World &world, Layer &layer) override;

    GridSystem *gridSystem() { return gridSystem_; }
    Map *map() { return map_; }

    void addEntityNextFrame(ItemEntity *itemEntity);

private:
    HeroType heroType_;
    bool addEntityToGridSystem(ItemEntity *itemEntity);
    bool findAvailablePosition(const ItemEntity &item, GEB::Vec2 &position) const;
    bool canPlaceEntity(const ItemEntity &item, float x, float y) const;
    bool checkChangeChapter() const;
    void changeChapterIfNeed(World &world);

    bool changeMapFlag_ = false;

    GridSystem *gridSystem_ = nullptr;
    Map *map_ = nullptr;
    Hero *hero_ = nullptr; // Owned by the current grid system.

    GEB::Structures::Array<ItemEntity *> entitiesNextFrame_;
};
