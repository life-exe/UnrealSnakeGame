// Snake Game, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"

namespace SnakeGame
{
class Food

{
public:
    Food() = default;

    /**
     * Sets food position
     * @param position  position on the grid
     */
    void setPosition(const Position& position);

    /**
     * Returns food position
     * @return Position  food position
     */
    Position position() const;

private:
    Position m_position;
};
}  // namespace SnakeGame
