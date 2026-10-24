#pragma once

#include "entities/ItemEntity.h"
#include "structure/Array.h"
#include "core/Types.h"

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

    void updateEntityGridIfNeed(GEB::Position oldCenter, GEB::Position newCenter, ItemEntity *item)
    {
        int oldRowIdx = oldCenter.y / GRID_SIZE;
        int oldColIdx = oldCenter.x / GRID_SIZE;
        int newRowIdx = newCenter.y / GRID_SIZE;
        int newColIdx = newCenter.x / GRID_SIZE;

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

    static void getCandidateGrids(float centerX, float centerY, GEB::Structures::Array<GEB::Pair<int, int>> &candidateGrids)
    {
        const int centerRow = static_cast<int>(centerY) / GRID_SIZE;
        const int centerCol = static_cast<int>(centerX) / GRID_SIZE;

        // GridSystem takes (row, column): y selects the row, x selects the column.
        for (int row = centerRow - 1; row <= centerRow + 1; ++row)
        {
            if (row < 0 || row >= GRID_ROWCNT)
                continue;

            for (int col = centerCol - 1; col <= centerCol + 1; ++col)
            {
                if (col < 0 || col >= GRID_COLCNT)
                    continue;

                candidateGrids.pushBack({row, col});
            }
        }
    }

    static void getCandidateItemList(GridSystem &gridSystem,
                                     ItemEntity *detectedItem,
                                     GEB::Structures::Array<ItemEntity *> &candidateItemList)
    {
        auto center = detectedItem->center();
        GEB::Structures::Array<GEB::Pair<int, int>> candidateGrids;
        getCandidateGrids(center.x, center.y, candidateGrids);
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
    inline GEB::Structures::Array<Grid> &gridMap() { return gridMap_; }

private:
    int gridSize_;
    int rowCount_;
    int colCount_;

    GEB::Structures::Array<Grid> gridMap_;
};
