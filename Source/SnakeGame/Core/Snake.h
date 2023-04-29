// Snake Game, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"

namespace SnakeGame
{
class Snake
{
public:
    Snake(const Settings::Snake& settings);

    /**
     * Returns list of all snake links
     * @return TSnakeList  links
     */
    const TSnakeList& links() const { return m_links; }

    /**
     * Returns head position
     * @return Position  head position
     */
    Position head() const { return m_links.GetHead()->GetValue(); }

    /**
     * Moves snake links with a given input
     * @param input  user input
     */
    void move(const Input& input);

    /**
     * Increases snake length, adds link to the tail
     */
    void increase();

private:
    TSnakeList m_links;
    Input m_lastInput{1, 0};
};
}  // namespace SnakeGame
