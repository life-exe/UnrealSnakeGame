// Snake Game, Copyright LifeEXE. All Rights Reserved.

#pragma once

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "SnakeGame/Core/Utils.h"

namespace SnakeGame
{
namespace Test
{
class MockPositionRandomizer : public IPositionRandomizer
{
public:
    virtual bool generatePosition(const Dim& dim, const TArray<CellType>& cells, Position& position) const override
    {
        position = m_positions[m_index++];
        return true;
    }

    void setPositions(const TArray<Position>& positions)
    {
        m_positions = positions;
        m_index = 0;
    }

private:
    TArray<Position> m_positions;
    mutable int32 m_index{0};
};

}  // namespace Test
}  // namespace SnakeGame

#endif