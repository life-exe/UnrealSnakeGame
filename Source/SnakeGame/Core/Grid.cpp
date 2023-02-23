// Snake Game, Copyright LifeEXE. All Rights Reserved.

#include "Core/Grid.h"

DEFINE_LOG_CATEGORY_STATIC(LogGrid, All, All);

using namespace Snake;

Grid::Grid(const Dim& dim)  //
    : c_dim(Dim{dim.width + 2, dim.height + 2})
{
    /*
     * * * * * *
     * 0 0 0 0 *
     * 0 0 0 0 *
     * 0 0 0 0 *
     * * * * * *
     */

    m_cells.Init(CellType::Empty, c_dim.width * c_dim.height);
    initWalls();
    printDebug();
}

void Grid::initWalls()
{
    for (int32 y = 0; y < c_dim.height; ++y)
    {
        for (int32 x = 0; x < c_dim.width; ++x)
        {
            if (x == 0 || x == c_dim.width - 1 || y == 0 || y == c_dim.height - 1)
            {
                m_cells[posToIndex(x, y)] = CellType::Wall;
            }
        }
    }
}

void Grid::printDebug()
{
#if !UE_BUILD_SHIPPING
    for (int32 y = 0; y < c_dim.height; ++y)
    {
        FString line;
        for (int32 x = 0; x < c_dim.width; ++x)
        {
            TCHAR symbol{};
            switch (m_cells[posToIndex(x, y)])
            {
                case CellType::Empty: symbol = '0'; break;
                case CellType::Wall: symbol = '*'; break;
            }
            line.AppendChar(symbol).AppendChar(' ');
        }
        UE_LOG(LogGrid, Display, TEXT("%s"), *line);
    }
#endif
}

int32 Grid::posToIndex(int32 x, int32 y) const
{
    return x + y * c_dim.width;
}
