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
    virtual ~IPositionRandomizer() = default;
};

class PositionRandomizer : public IPositionRandomizer
{
public:
    virtual bool generatePosition(const Dim& dim, const TArray<CellType>& cells, Position& position) const override
    {
        const uint32 startX = FMath::RandRange(1, dim.width - 2);
        const uint32 startY = FMath::RandRange(1, dim.height - 2);
        Position randomPosition = {startX, startY};

        do
        {
            const uint32 currentIndex = posToIndex(randomPosition, dim);
            if (cells[currentIndex] == CellType::Empty)
            {
                position = randomPosition;
                return true;
            }

            if (++randomPosition.x > dim.width - 2)
            {
                randomPosition.x = 1;
                if (++randomPosition.y > dim.height - 2)
                {
                    randomPosition.y = 1;
                }
            }
        } while (randomPosition.x != startX || randomPosition.y != startY);

        return false;
    }

private:
    FORCEINLINE Position indexToPos(uint32 index, const Dim& dim) const { return Position(index % dim.width, index / dim.width); }
    FORCEINLINE uint32 posToIndex(const Position& position, const Dim& dim) const { return position.x + position.y * dim.width; }
};

using IPositionRandomizerPtr = TSharedPtr<IPositionRandomizer>;

}  // namespace SnakeGame