#include "entities/Hero.h"
#include "world/World.h"
#include "core/GameBase.h"
#include "GamesEngineeringBase.h"
#include "core/Types.h"
#include "stages/Play.h"

GEB::Direction translateEventsToDirection(const GamesEngineeringBase::Window &canvas)
{
    if (canvas.keyPressed('W') && canvas.keyPressed('A'))
        return GEB::Direction::UpLeft;
    if (canvas.keyPressed('W') && canvas.keyPressed('D'))
        return GEB::Direction::UpRight;
    if (canvas.keyPressed('S') && canvas.keyPressed('A'))
        return GEB::Direction::DownLeft;
    if (canvas.keyPressed('S') && canvas.keyPressed('D'))
        return GEB::Direction::DownRight;
    if (canvas.keyPressed('W'))
        return GEB::Direction::Up;
    if (canvas.keyPressed('S'))
        return GEB::Direction::Down;
    if (canvas.keyPressed('A'))
        return GEB::Direction::Left;
    if (canvas.keyPressed('D'))
        return GEB::Direction::Right;
    return GEB::Direction::None;
}

bool checkIsSingleDirection(GEB::Direction direction)
{
    switch (direction)
    {
    case GEB::Direction::Up:
    case GEB::Direction::Down:
    case GEB::Direction::Left:
    case GEB::Direction::Right:
        return true;
    default:
        return false;
    }
}

void calculateCandidateGrids(float centerX, float centerY, GEB::Structures::Array<GEB::Pair<int, int>> &candidateGrids)
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

void Hero::handleEvents(World &world, Layer &layer)
{
    auto &canvas = world.canvas();

    float dt = world.gameInfo().dt();

    float distance = dt * speed_;

    GEB::Structures::Array<ItemEntity *> candidateItemList;
    GEB::Structures::Array<GEB::Pair<int, int>> candidateGrids;
    auto &playStage = dynamic_cast<PlayStage &>(layer.currentStage());
    auto &gridSystem = playStage.gridSystem();

    auto oldCenter = center();

    calculateCandidateGrids(oldCenter.x, oldCenter.y, candidateGrids);
    int oldColIdx = oldCenter.x / GRID_SIZE;
    int oldRowIdx = oldCenter.y / GRID_SIZE;
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
            if (!itemList[j] || itemList[j] == this)
                continue;
            candidateItemList.pushBack(itemList[j]);
        }
    }

    // calculate boundaries horizontally and vertically, respectively, in case of diagonal movement
    GEB::Direction direction = translateEventsToDirection(canvas);
    auto candidateItemHead = candidateItemList.head();

    switch (direction)
    {
    case GEB::Direction::Up:
    case GEB::Direction::UpLeft:
    case GEB::Direction::UpRight:
        moveUp(distance, calculateBoundary(world, layer, GEB::Direction::Up, candidateItemHead, candidateItemList.size()).top);
        break;
    case GEB::Direction::Down:
    case GEB::Direction::DownLeft:
    case GEB::Direction::DownRight:
        moveDown(distance, calculateBoundary(world, layer, GEB::Direction::Down, candidateItemHead, candidateItemList.size()).bottom);
        break;
    default:
        break;
    }

    switch (direction)
    {
    case GEB::Direction::Left:
    case GEB::Direction::UpLeft:
    case GEB::Direction::DownLeft:
        moveLeft(distance, calculateBoundary(world, layer, GEB::Direction::Left, candidateItemHead, candidateItemList.size()).left);
        break;
    case GEB::Direction::Right:
    case GEB::Direction::UpRight:
    case GEB::Direction::DownRight:
        moveRight(distance, calculateBoundary(world, layer, GEB::Direction::Right, candidateItemHead, candidateItemList.size()).right);
        break;
    default:
        break;
    }

    auto newCenter = center();
    int newRowIdx = newCenter.y / GRID_SIZE;
    int newColIdx = newCenter.x / GRID_SIZE;

    if (oldRowIdx != newRowIdx || oldColIdx != newColIdx)
    {
        gridSystem.updateEntityGrid(oldRowIdx, oldColIdx, newRowIdx, newColIdx, this);
    }
}

void Hero::update(World &world, Layer &layer)
{
    // handle keyboard events for hero
    handleEvents(world, layer);

    HeroInfo heroInfo{
        .x = x_,
        .y = y_,
        .width = width_,
        .height = height_};

    world.gameInfo()
        .updateHeroInfo(heroInfo);

    world.gameInfo().updateViewport(heroInfo);
}
