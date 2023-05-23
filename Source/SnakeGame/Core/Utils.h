// Snake Game, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"

namespace SnakeGame
{
class IPositionRandomizer
{
public:
    virtual bool generatePosition(const Dim& dim, const TArray<CellType>& cells, Position& position) const = 0;
};

class PositionRandomizer : public IPositionRandomizer
{
public:
    virtual bool generatePosition(const Dim& dim, const TArray<CellType>& cells, Position& position) const override
    {
        const auto gridSize = dim.width * dim.height;
        const uint32 index = FMath::RandRange(0, gridSize - 1);

        for (uint32 i = index; i < gridSize; ++i)
        {
            if (cells[i] == CellType::Empty)
            {
                position = indexToPos(i, dim);
                return true;
            }
        }

        for (uint32 i = 0; i < index; ++i)
        {
            if (cells[i] == CellType::Empty)
            {
                position = indexToPos(i, dim);
                return true;
            }
        }

        return false;
    }

private:
    FORCEINLINE Position indexToPos(uint32 index, const Dim& dim) const { return Position(index % dim.width, index / dim.width); }
};

using IPositionRandomizerPtr = TSharedPtr<IPositionRandomizer>;

}  // namespace SnakeGame