#pragma once

#include "entities/ItemEntity.h"
#include "structure/Array.h"

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

    void addEntity(ItemEntity *item)
    {
        itemList_.pushBack(item);
    }

    void removeEntity(const ItemEntity *item)
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

    inline GEB::Structures::Array<ItemEntity *> &itemList() { return itemList_; }

private:
    GEB::Structures::Array<ItemEntity *> itemList_;
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

    void updateEntityGrid(int oldRowIdx, int oldColIdx, int newRowIdx, int newColIdx, ItemEntity *item)
    {
        if (!item || !grid(oldRowIdx, oldColIdx) || !grid(newRowIdx, newColIdx))
            return;

        if (oldRowIdx == newRowIdx && oldColIdx == newColIdx)
            return;

        removeEntity(oldRowIdx, oldColIdx, item);
        addEntity(newRowIdx, newColIdx, item);
    }

    void addEntity(int rowIdx, int colIdx, ItemEntity *item)
    {
        auto *targetGrid = grid(rowIdx, colIdx);

        if (targetGrid && item)
            targetGrid->addEntity(item);
    }

    void removeEntity(int rowIdx, int colIdx, ItemEntity *item)
    {
        auto *targetGrid = grid(rowIdx, colIdx);

        if (targetGrid && item)
            targetGrid->removeEntity(item);
    }

    Grid *grid(int rowIdx, int colIdx)
    {
        if (rowIdx < 0 || rowIdx >= rowCount_ || colIdx < 0 || colIdx >= colCount_)
            return nullptr;

        return &gridMap_[rowIdx * colCount_ + colIdx];
    }

    inline int gridSize() const { return gridSize_; }
    inline int rowCount() const { return rowCount_; }
    inline int colCount() const { return colCount_; }
    inline GEB::Structures::Array<Grid> &gridMap() { return gridMap_; }

private:
    int gridSize_;
    int rowCount_;
    int colCount_;

    GEB::Structures::Array<Grid> gridMap_;
};
