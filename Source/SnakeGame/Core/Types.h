// Snake Game, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

namespace Snake
{

struct Dim
{
    int32 width;
    int32 height;
};

enum class CellType
{
    Empty = 0,
    Wall
    // Food
    // Snake
};

struct Settings
{
    Dim gridDims;
};

}  // namespace Snake