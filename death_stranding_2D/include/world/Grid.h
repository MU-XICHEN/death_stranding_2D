#pragma once

#include "structure/Array.h"
#include "structure/DoublyLinkedList.h"
#include "core/Types.h"
#include "entities/AABB.h"

class Grid
{
public:
    Grid() = default;
    virtual ~Grid();

    void addEntity(Entity *item);

    void removeEntity(const Entity *item);

    inline GEB::Structures::DoublyLinkedList<Entity *> &itemList() { return itemList_; }

private:
    GEB::Structures::DoublyLinkedList<Entity *> itemList_;
};

class GridSystem
{
public:
    explicit GridSystem(int rowCnt, int colCnt, int size);

    virtual ~GridSystem() = default;

    GEB::Pair<int, int> getGridIdxPair(const GEB::Vec2 &center);

    void getCandidateListByAABB(
        Entity *detectedItem,
        const AABB &aabb,
        GEB::Structures::Array<Entity *> &candidateItemList);

    void getCandidateListByItem(
        Entity *detectedItem,
        const GEB::Vec2 &distanceVec,
        GEB::Structures::Array<Entity *> &candidateItemList);

    // update the grid which entity belong to right now.
    void updateEntityGridIfNeed(GEB::Vec2 oldCenter, Entity *item);

    void addEntity(int rowIdx, int colIdx, Entity *item);

    void removeEntity(int rowIdx, int colIdx, Entity *item);

    inline int gridSize() const { return gridSize_; }
    inline int rowCount() const { return rowCount_; }
    inline int colCount() const { return colCount_; }
    inline GEB::Structures::Array<Entity *> &itemsToUpdate() { return itemsToUpdate_; }
    inline GEB::Structures::Array<Entity *> &inactiveItems() { return inactiveItems_; }

    inline void addInactiveItem(Entity *item) { inactiveItems_.pushBack(item); }
    inline void clearInactiveItems() { inactiveItems_.clear(); }

private:
    // Change the update list only after the entity update loop finishes.
    void addEntityToUpdateList(Entity *item);
    void removeEntityFromUpdateList(Entity *item);
    // Move between cells while keeping the same update list entry.
    void changeEntityGrid(Entity *item, GEB::Vec2 oldCenter, GEB::Vec2 newCenter);

    int gridSize_;
    int rowCount_;
    int colCount_;

    GEB::Structures::Array<Grid> gridMap_;
    // References for the update loop. The grids own these entities.
    GEB::Structures::Array<Entity *> itemsToUpdate_;

    // Entities to remove from the grid after this frame.
    GEB::Structures::Array<Entity *> inactiveItems_;

    void getCandidateGridsByAABB(const AABB &box,
                                 GEB::Structures::Array<GEB::Pair<int, int>> &candidateGrids);

    void getCandidateGridsByDisVec2(
        Entity *detectedItem,
        const GEB::Vec2 &distanceVec,
        GEB::Structures::Array<GEB::Pair<int, int>> &candidateGrids);

    void getCandidateListByGrids(
        Entity *detectedItem,
        const GEB::Structures::Array<GEB::Pair<int, int>> &candidateGrids,
        GEB::Structures::Array<Entity *> &candidateItemList);

    Grid *grid(int rowIdx, int colIdx);
};
