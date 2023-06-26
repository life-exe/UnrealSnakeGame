// Snake Game, Copyright LifeEXE. All Rights Reserved.

#include "SnakeGame/Core/Game.h"
#include "SnakeGame/Core/Grid.h"
#include "SnakeGame/Core/Snake.h"
#include "SnakeGame/Core/Food.h"

DEFINE_LOG_CATEGORY_STATIC(LogGame, All, All);

using namespace SnakeGame;

Game::Game(const Settings& settings, const IPositionRandomizerPtr& randomizer) : c_settings(settings)
{
    m_grid = MakeShared<Grid>(settings.gridDims, randomizer);
    checkf(m_grid->dim().width / 2 >= settings.snake.defaultSize, TEXT("Snake initial length [%i] doesn't fit grid width [%i]"),
        settings.snake.defaultSize, m_grid->dim().width);
    m_snake = MakeShared<Snake>(settings.snake);
    m_food = MakeShared<Food>();
    updateGrid();
    generateFood();
}

void Game::update(float deltaSeconds, const Input& input)
{
    if (m_gameOver || !updateTime(deltaSeconds)) return;

    m_snake->move(input);

    if (died())
    {
        m_gameOver = true;
        dispatchEvent(GameplayEvent::GameOver);
        return;
    }

    if (foodTaken())
    {
        ++m_score;
        m_snake->increase();
        dispatchEvent(GameplayEvent::FoodTaken);
        generateFood();
    }

    updateGrid();
}

void Game::updateGrid()
{
    m_grid->update(m_snake->links().GetHead(), CellType::Snake);
    // m_grid->printDebug();
}

bool Game::updateTime(float deltaSeconds)
{
    m_gameTime += deltaSeconds;
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

void Game::generateFood()
{
    Position foodPosition;
    if (m_grid->randomEmptyPosition(foodPosition))
    {
        m_food->setPosition(foodPosition);
        m_grid->update(m_food->position(), CellType::Food);
    }
    else
    {
        m_gameOver = true;
        dispatchEvent(GameplayEvent::GameCompleted);
    }
}

bool Game::foodTaken() const
{
    return m_grid->hitTest(m_snake->head(), CellType::Food);
}

void Game::subscribeOnGameplayEvent(GameplayEventCallback callback)
{
    m_gameplayEventCallbacks.Add(callback);
}

void Game::dispatchEvent(GameplayEvent Event)
{
    for (const auto& callback : m_gameplayEventCallbacks)
    {
        if (callback)
        {
            callback(Event);
        }
    }
}
