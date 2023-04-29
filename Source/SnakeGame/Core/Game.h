// Snake Game, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"

namespace SnakeGame
{

class Grid;
class Snake;
class Food;

class Game
{
public:
    Game(const Settings& settings);

    /**
     * Returns the pointer to the grid object
     * @return TSharedPtr<Grid>  pointer to the grid object
     */
    TSharedPtr<Grid> grid() const { return m_grid; }

    /**
     * Returns the pointer to the snake object
     * @return TSharedPtr<Snake>  pointer to the snake object
     */
    TSharedPtr<Snake> snake() const { return m_snake; }

    /**
     * Returns the pointer to the food object
     * @return TSharedPtr<Food>  pointer to the food object
     */
    TSharedPtr<Food> food() const { return m_food; }

    /**
     * Updates game
     * @param deltaSeconds  delta time
     * @param input  user input
     */
    void update(float deltaSeconds, const Input& input);

private:
    const Settings c_settings;
    TSharedPtr<Grid> m_grid;
    TSharedPtr<Snake> m_snake;
    TSharedPtr<Food> m_food;

    float m_moveSeconds{0.0f};
    bool m_gameOver{false};
    uint32 m_score{0};

    void updateGrid();
    bool updateTime(float deltaSeconds);
    bool died() const;

    void generateFood();
    bool foodTaken() const;
};
}  // namespace SnakeGame
