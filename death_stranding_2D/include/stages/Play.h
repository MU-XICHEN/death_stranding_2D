#pragma once

#include "stages/Stage.h"
#include "entities/ItemEntity.h"
#include "world/Grid.h"
#include "map/Map.h"

class Hero;

class PlayStage : public Stage
{
public:
    PlayStage();
    virtual ~PlayStage() override;

    PlayStage(const PlayStage &) = delete;
    PlayStage &operator=(const PlayStage &) = delete;

    void initialize();
    void addEntityToGridSystem(ItemEntity *itemEntity);
    void update(World &world, Layer &layer) override;
    void render(World &world, Layer &layer) override;
    void renderFinished(World &world, Layer &layer) override;

    GridSystem *gridSystem() { return gridSystem_; }
    Map *map() { return map_; }

private:
    bool checkChangeChapter() const;
    void changeChapterIfNeed(World &world);

    bool changeMapFlag_ = false;

    GridSystem *gridSystem_ = nullptr;
    Map *map_ = nullptr;
    Hero *hero_ = nullptr; // Owned by the current grid system.
};
