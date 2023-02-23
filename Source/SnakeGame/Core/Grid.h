// Snake Game, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"

namespace Snake
{

class Grid
{
public:
    Grid(const Dim& dim);

    Dim dim() const { return c_dim; }

private:
    const Dim c_dim;
    TArray<CellType> m_cells;

    void initWalls();
    void printDebug();
    FORCEINLINE int32 posToIndex(int32 x, int32 y) const;
};

}  // namespace Snake
