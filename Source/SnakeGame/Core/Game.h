// Snake Game, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"

namespace SnakeGame
{

class Grid;
class Snake;

class Game
{
public:
    Game(const Settings& settings);

    /**
     * Returns the pointer to the grid object
     * @return TSharedPtr<Grid>  pointer to the grid object
     */
    TSharedPtr<Grid> grid() const { return m_grid; }

    TSharedPtr<Snake> snake() const { return m_snake; }

    void update(float deltaSeconds, const Input& input);

private:
    const Settings c_settings;
    TSharedPtr<Grid> m_grid;
    TSharedPtr<Snake> m_snake;

    float m_moveSeconds{0.0f};
    bool m_gameOver{false};

    void move(const Input& input);
    void updateGrid();
    bool updateTime(float deltaSeconds);
    bool died() const;
};
}  // namespace SnakeGame
