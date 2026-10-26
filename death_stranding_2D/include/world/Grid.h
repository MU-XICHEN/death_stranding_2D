#pragma once

#include "entities/ItemEntity.h"
#include "structure/Array.h"
#include "core/Types.h"
#include "entities/AABB.h"

class Grid
{
public:
    Grid() = default;
    virtual ~Grid()
    {
        for (int i = 0; i < itemList_.size(); ++i)
        {
            if (itemList_[i])
                delete itemList_[i];
        }
    }

    void addEntity(Entity *item)
    {
        itemList_.pushBack(item);
    }

    void removeEntity(const Entity *item)
    {
        for (int i = 0; i < itemList_.size(); ++i)
        {
            if (itemList_[i] == item)
            {
                // do not delete the item(ItemEntity *), just remove it from the list。
                // all memory management of the item is handled in ~Grid().
                itemList_.removeAt(i);
                break;
            }
        }
    }

    inline GEB::Structures::Array<Entity *> &itemList() { return itemList_; }

private:
    GEB::Structures::Array<Entity *> itemList_;
};

class GridSystem
{
public:
    explicit GridSystem(int rowCnt, int colCnt, int size)
        : rowCount_(rowCnt),
          colCount_(colCnt),
          gridSize_(size),
          gridMap_(rowCnt * colCnt) {}

    virtual ~GridSystem() = default;

    void updateEntityGridIfNeed(GEB::Position oldCenter, GEB::Position newCenter, Entity *item)
    {
        int oldRowIdx = GEB::Math::floorToInt(oldCenter.y / gridSize_);
        int oldColIdx = GEB::Math::floorToInt(oldCenter.x / gridSize_);
        int newRowIdx = GEB::Math::floorToInt(newCenter.y / gridSize_);
        int newColIdx = GEB::Math::floorToInt(newCenter.x / gridSize_);

        if (!item || !grid(oldRowIdx, oldColIdx) || !grid(newRowIdx, newColIdx))
            return;

        if (oldRowIdx == newRowIdx && oldColIdx == newColIdx)
            return;

        changeEntityGrid(item, oldCenter, newCenter);
    }

    // todo: be careful when updating the itemsToUpdate_ in the game loop to avoid modifying elements while iterating.
    void addEntityToUpdateList(Entity *item)
    {
        if (item)
            itemsToUpdate_.pushBack(item);
    }

    void removeEntityFromUpdateList(Entity *item)
    {
        for (int i = 0; i < itemsToUpdate_.size(); ++i)
        {
            if (itemsToUpdate_[i] == item)
            {
                itemsToUpdate_.removeAt(i);
                break;
            }
        }
    }

    void addEntityToGrid(int rowIdx, int colIdx, Entity *item)
    {
        auto *targetGrid = grid(rowIdx, colIdx);
        if (targetGrid && item)
        {
            targetGrid->addEntity(item);
            addEntityToUpdateList(item); // sysc the updated items
        }
    }

    void removeEntityFromGrid(int rowIdx, int colIdx, Entity *item)
    {
        auto *targetGrid = grid(rowIdx, colIdx);

        if (targetGrid && item)
        {
            targetGrid->removeEntity(item);
            removeEntityFromUpdateList(item);
        }
    }

    // This method just moves the item from one grid to another without updating the update list.
    void changeEntityGrid(Entity *item, GEB::Position oldCenter, GEB::Position newCenter)
    {
        // remove
        auto *fromGird = grid(GEB::Math::floorToInt(oldCenter.y / gridSize_),
                              GEB::Math::floorToInt(oldCenter.x / gridSize_));
        if (fromGird && item)
            fromGird->removeEntity(item);

        // add
        auto *toGrid = grid(GEB::Math::floorToInt(newCenter.y / gridSize_),
                            GEB::Math::floorToInt(newCenter.x / gridSize_));
        if (toGrid && item)
            toGrid->addEntity(item);
    }

    Grid *grid(int rowIdx, int colIdx)
    {
        if (rowIdx < 0 || rowIdx >= rowCount_ || colIdx < 0 || colIdx >= colCount_)
            return nullptr;

        return &gridMap_[rowIdx * colCount_ + colIdx];
    }

    void getCandidateGrids(
        Entity *detectedItem,
        const GEB::Vec2 &distanceVec,
        const GEB::Boundary &mapBoundary,
        GEB::Structures::Array<GEB::Pair<int, int>> &candidateGrids)
    {

        if (!detectedItem)
            return;
        auto aabb = AABB::createAABB(detectedItem, distanceVec, mapBoundary);

        if (aabb.width() <= 0 || aabb.height() <= 0)
            return;

        /**
         * [bugfix]: 
         * extend the candidate grid range by 1 in all directions
         * to account for items which belong to neighboring grids.
         */
        int startRow = GEB::Math::max(0, GEB::Math::floorToInt(aabb.y() / gridSize_) - 1);
        int startCol = GEB::Math::max(0, GEB::Math::floorToInt(aabb.x() / gridSize_) - 1);
        int endRow = GEB::Math::min(rowCount_ - 1, GEB::Math::floorToInt((aabb.y() + aabb.height()) / gridSize_) + 1);
        int endCol = GEB::Math::min(colCount_ - 1, GEB::Math::floorToInt((aabb.x() + aabb.width()) / gridSize_) + 1);

        for (int row = startRow; row <= endRow; row++)
        {
            for (int col = startCol; col <= endCol; col++)
            {
                candidateGrids.pushBack({row, col});
            }
        }
    }

    static void getCandidateItemList(GridSystem &gridSystem,
                                     Entity *detectedItem,
                                     const GEB::Vec2 &distanceVec,
                                     const GEB::Boundary &mapBoundary,
                                     GEB::Structures::Array<Entity *> &candidateItemList)
    {
        auto center = detectedItem->center();
        GEB::Structures::Array<GEB::Pair<int, int>> candidateGrids;
        gridSystem.getCandidateGrids(detectedItem, distanceVec, mapBoundary, candidateGrids);
        // use nine grids(center grid and its eight neighbors) to detect potential collisions with items, as grid size is larger than role size.
        // todo: prune branches in the furture.
        int candidateGridSize = candidateGrids.size();
        for (int i = 0; i < candidateGridSize; i++)
        {
            auto pos = candidateGrids[i];
            auto *grid = gridSystem.grid(pos.first, pos.second);
            if (!grid)
                continue;

            int len = grid->itemList().size();
            auto &itemList = grid->itemList();
            for (int j = 0; j < len; j++)
            {
                if (!itemList[j] || itemList[j] == detectedItem)
                    continue;
                candidateItemList.pushBack(itemList[j]);
            }
        }
    }

    inline int gridSize() const { return gridSize_; }
    inline int rowCount() const { return rowCount_; }
    inline int colCount() const { return colCount_; }
    inline GEB::Structures::Array<Entity *> &itemsToUpdate() { return itemsToUpdate_; }

private:
    int gridSize_;
    int rowCount_;
    int colCount_;

    GEB::Structures::Array<Grid> gridMap_;
    /**
     * just a copy of the items that need to be updated.
     * the memory of items that Entity * points to is managed by the Grid.
     */
    GEB::Structures::Array<Entity *> itemsToUpdate_;
};
