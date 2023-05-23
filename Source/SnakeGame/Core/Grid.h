// Snake Game, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"
#include "Utils.h"

namespace SnakeGame
{

class Grid
{
public:
    Grid(const Dim& dim, const IPositionRandomizerPtr& randomizer = MakeShared<PositionRandomizer>());

    /**
     * Returns center position in grid (walls included)
     * @param width
     * @param height
     * @return Position center of grid
     */
    static Position center(uint32 width, uint32 height) { return Position(width / 2 + 1, height / 2 + 1); }

    /**
     * Returns grid dimensions including walls (width + 2; height + 2)
     * @return Dim  grid dimensions
     */
    Dim dim() const { return c_dim; }

    /**
     * Update cells with a given cell type
     * @param links  pointer to the position link
     * @param cellType  type of the cell
     */
    void update(const TPositionPtr* links, CellType cellType);

    /**
     * Update cell with a given cell type
     * @param position  position on the grid to be updated
     * @param cellType  type of the cell
     */
    void update(const Position& position, CellType cellType);

    /**
     * Checks cell type in the given position
     * @param position  position on the grid
     * @param cellType  type of the cell
     * @return bool  if hit test was successfull
     */
    bool hitTest(const Position& position, CellType cellType) const;

    /**
     * Returns empty position by ref
     * @param[out]  position on the grid
     * @return bool  true if empty position exists (if position doesn't exist then snake body fills whole grid)
     */
    UE_NODISCARD bool randomEmptyPosition(Position& position) const;

    void printDebug();

private:
    const Dim c_dim;
    TArray<CellType> m_cells;
    TMap<CellType, TArray<uint32>> m_indByType = {
        {CellType::Snake, {}},  //
        {CellType::Wall, {}},   //
        {CellType::Food, {}},   //
    };

    TSharedPtr<IPositionRandomizer> m_positionRandomizer;

    void initWalls();
    void updateInternal(const Position& position, CellType cellType);
    void freeCellsByType(CellType cellType);

    FORCEINLINE uint32 posToIndex(uint32 x, uint32 y) const;
    FORCEINLINE uint32 posToIndex(const Position& position) const;
};

}  // namespace SnakeGame
