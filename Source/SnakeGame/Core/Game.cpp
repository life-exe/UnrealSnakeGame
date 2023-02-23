// Snake Game, Copyright LifeEXE. All Rights Reserved.

#include "SnakeGame/Core/Game.h"
#include "SnakeGame/Core/Grid.h"

DEFINE_LOG_CATEGORY_STATIC(LogGame, All, All);

using namespace Snake;

Game::Game(const Settings& settings) : c_settings(settings)
{
    m_grid = MakeShared<Grid>(settings.gridDims);
}
