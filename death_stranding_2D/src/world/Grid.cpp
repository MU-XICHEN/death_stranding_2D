#include "world/Grid.h"

Grid::~Grid()
{
    for (int i = 0; i < itemList_.size(); ++i)
    {
        if (itemList_[i])
            delete itemList_[i];
    }
}

void Grid::addEntity(Entity *item)
{
    itemList_.pushBack(item);
}

void Grid::removeEntity(const Entity *item)
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

// GridSystem

GridSystem::GridSystem(int rowCnt, int colCnt, int size)
    : rowCount_(rowCnt),
      colCount_(colCnt),
      gridSize_(size),
      gridMap_(rowCnt * colCnt) {}

GEB::Pair<int, int> GridSystem::getGridIdxPair(const GEB::Position &center)
{
    int row = GEB::Math::floorToInt(center.y / gridSize_);
    int col = GEB::Math::floorToInt(center.x / gridSize_);
    return {row, col};
}

void GridSystem::getCandidateGridsByAABB(const AABB &aabb,
                                         const GEB::Boundary &mapBoundary,
                                         GEB::Structures::Array<GEB::Pair<int, int>> &candidateGrids)
{
    /**
     * [bugfix]:
     * extend one more grid in all directions
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

void GridSystem::getCandidateGridsByDisVec2(
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

    getCandidateGridsByAABB(aabb, mapBoundary, candidateGrids);
}

void GridSystem::getCollisionCandidateListByAABB(
    Entity *detectedItem,
    const AABB &aabb,
    const GEB::Boundary &mapBoundary,
    GEB::Structures::Array<Entity *> &candidateItemList)
{
    GEB::Structures::Array<GEB::Pair<int, int>> candidateGrids;
    getCandidateGridsByAABB(aabb, mapBoundary, candidateGrids);

    getCollisionCandidateListByGrids(
        detectedItem,
        candidateGrids,
        candidateItemList);
}

void GridSystem::getCollisionCandidateListByGrids(
    Entity *detectedItem,
    const GEB::Structures::Array<GEB::Pair<int, int>> &candidateGrids,
    GEB::Structures::Array<Entity *> &candidateItemList)
{
    int candidateGridSize = candidateGrids.size();
    for (int i = 0; i < candidateGridSize; i++)
    {
        auto pos = candidateGrids[i];
        auto *gridPtr = grid(pos.first, pos.second);
        if (!gridPtr)
            continue;

        int len = gridPtr->itemList().size();
        auto &itemList = gridPtr->itemList();
        for (int j = 0; j < len; j++)
        {
            if (!itemList[j] || itemList[j] == detectedItem)
                continue;
            candidateItemList.pushBack(itemList[j]);
        }
    }
}

void GridSystem::getCollisionCandidateListByItem(
    Entity *detectedItem,
    const GEB::Vec2 &distanceVec,
    const GEB::Boundary &mapBoundary,
    GEB::Structures::Array<Entity *> &candidateItemList)
{
    auto center = detectedItem->center();
    GEB::Structures::Array<GEB::Pair<int, int>> candidateGrids;
    getCandidateGridsByDisVec2(detectedItem, distanceVec, mapBoundary, candidateGrids);
    getCollisionCandidateListByGrids(detectedItem, candidateGrids, candidateItemList);
}

void GridSystem::updateEntityGridIfNeed(GEB::Position oldCenter, Entity *item)
{
    GEB::Position newCenter = item->center();
    auto [oldRowIdx, oldColIdx] = getGridIdxPair(oldCenter);
    auto [newRowIdx, newColIdx] = getGridIdxPair(newCenter);

    if (!item || !grid(oldRowIdx, oldColIdx) || !grid(newRowIdx, newColIdx))
        return;

    if (oldRowIdx == newRowIdx && oldColIdx == newColIdx)
        return;

    changeEntityGrid(item, oldCenter, newCenter);
}

void GridSystem::addEntityToUpdateList(Entity *item)
{
    if (item)
        itemsToUpdate_.pushBack(item);
}

void GridSystem::removeEntityFromUpdateList(Entity *item)
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

void GridSystem::addEntity(int rowIdx, int colIdx, Entity *item)
{
    auto *targetGrid = grid(rowIdx, colIdx);
    if (targetGrid && item)
    {
        targetGrid->addEntity(item);
        addEntityToUpdateList(item); // sysc the updated items
    }
}

void GridSystem::removeEntity(int rowIdx, int colIdx, Entity *item)
{
    auto *targetGrid = grid(rowIdx, colIdx);

    if (targetGrid && item)
    {
        targetGrid->removeEntity(item);
        removeEntityFromUpdateList(item);
    }
}

void GridSystem::changeEntityGrid(Entity *item, GEB::Position oldCenter, GEB::Position newCenter)
{
    // remove
    auto [oldRowIdx, oldColIdx] = getGridIdxPair(oldCenter);
    auto *fromGird = grid(oldRowIdx, oldColIdx);
    if (fromGird && item)
        fromGird->removeEntity(item);

    // add
    auto [newRowIdx, newColIdx] = getGridIdxPair(newCenter);
    auto *toGrid = grid(newRowIdx, newColIdx);
    if (toGrid && item)
        toGrid->addEntity(item);
}

Grid *GridSystem::grid(int rowIdx, int colIdx)
{
    if (rowIdx < 0 || rowIdx >= rowCount_ || colIdx < 0 || colIdx >= colCount_)
        return nullptr;

    return &gridMap_[rowIdx * colCount_ + colIdx];
}
