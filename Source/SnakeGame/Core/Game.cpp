// Snake Game, Copyright LifeEXE. All Rights Reserved.

#include "SnakeGame/Core/Game.h"
#include "SnakeGame/Core/Grid.h"
#include "SnakeGame/Core/Snake.h"

DEFINE_LOG_CATEGORY_STATIC(LogGame, All, All);

using namespace SnakeGame;

Game::Game(const Settings& settings) : c_settings(settings)
{
    m_grid = MakeShared<Grid>(settings.gridDims);
    m_snake = MakeShared<Snake>(settings.snake);
    updateGrid();
}

void Game::update(float deltaSeconds, const Input& input)
{
    if (m_gameOver || !updateTime(deltaSeconds)) return;

    move(input);

    if (died())
    {
        m_gameOver = true;
        UE_LOG(LogGame, Display, TEXT("-------------- GAME OVER --------------"));
    }
}

void Game::move(const Input& input)
{
    m_snake->move(input);
    updateGrid();
}

void Game::updateGrid()
{
    m_grid->update(m_snake->body(), CellType::Snake);
    m_grid->printDebug();
}

bool Game::updateTime(float deltaSeconds)
{
    m_moveSeconds += deltaSeconds;
    if (m_moveSeconds < c_settings.gameSpeed) return false;
    m_moveSeconds = 0.0f;
    return true;
}

bool Game::died() const
{
    return m_grid->hitTest(m_snake->head(), CellType::Wall) ||  //
           m_grid->hitTest(m_snake->head(), CellType::Snake);
}
