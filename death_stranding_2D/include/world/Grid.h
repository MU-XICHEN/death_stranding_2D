#pragma once

#include "entities/ItemEntity.h"
#include "structure/Array.h"
#include "core/Types.h"
#include "entities/AABB.h"

class Grid
{
public:
    Grid() = default;
    virtual ~Grid();

    void addEntity(Entity *item);

    void removeEntity(const Entity *item);

    inline GEB::Structures::Array<Entity *> &itemList() { return itemList_; }

private:
    GEB::Structures::Array<Entity *> itemList_;
};

class GridSystem
{
public:
    explicit GridSystem(int rowCnt, int colCnt, int size);

    virtual ~GridSystem() = default;

    void getCandidateGridsByAABB(const AABB &aabb,
                                 const GEB::Boundary &mapBoundary,
                                 GEB::Structures::Array<GEB::Pair<int, int>> &candidateGrids);

    GEB::Pair<int, int> getGridIdxPair(const GEB::Position &center);

    void getCandidateGridsByDisVec2(
        Entity *detectedItem,
        const GEB::Vec2 &distanceVec,
        const GEB::Boundary &mapBoundary,
        GEB::Structures::Array<GEB::Pair<int, int>> &candidateGrids);

    void getCollisionCandidateListByAABB(
        Entity *detectedItem,
        const AABB &aabb,
        const GEB::Boundary &mapBoundary,
        GEB::Structures::Array<Entity *> &candidateItemList);

    void getCollisionCandidateListByGrids(
        Entity *detectedItem,
        const GEB::Structures::Array<GEB::Pair<int, int>> &candidateGrids,
        GEB::Structures::Array<Entity *> &candidateItemList);

    void getCollisionCandidateListByItem(
        Entity *detectedItem,
        const GEB::Vec2 &distanceVec,
        const GEB::Boundary &mapBoundary,
        GEB::Structures::Array<Entity *> &candidateItemList);

    // update the grid which entity belong to right now.
    void updateEntityGridIfNeed(GEB::Position oldCenter, Entity *item);

    void addEntity(int rowIdx, int colIdx, Entity *item);

    void removeEntity(int rowIdx, int colIdx, Entity *item);

    Grid *grid(int rowIdx, int colIdx);

    inline int gridSize() const { return gridSize_; }
    inline int rowCount() const { return rowCount_; }
    inline int colCount() const { return colCount_; }
    inline GEB::Structures::Array<Entity *> &itemsToUpdate() { return itemsToUpdate_; }
    inline GEB::Structures::Array<Entity *> &inactiveItems() { return inactiveItems_; }

    inline void addInactiveItem(Entity *item) { inactiveItems_.pushBack(item); }
    inline void clearInactiveItems() { inactiveItems_.clear(); }

private:
    // todo: be careful when updating the itemsToUpdate_ in the game loop to avoid modifying elements while iterating.
    void addEntityToUpdateList(Entity *item);
    void removeEntityFromUpdateList(Entity *item);
    // This method just moves the item from one grid to another without updating the update list.
    void changeEntityGrid(Entity *item, GEB::Position oldCenter, GEB::Position newCenter);

    int gridSize_;
    int rowCount_;
    int colCount_;

    GEB::Structures::Array<Grid> gridMap_;
    /**
     * just a copy of the items that need to be updated.
     * the memory of items that Entity * points to is managed by the Grid.
     */
    GEB::Structures::Array<Entity *> itemsToUpdate_;

    /**
     * items that will be removed after iteration
     * also, a copy of items which were already in GridMap_
     */
    GEB::Structures::Array<Entity *> inactiveItems_;
};
