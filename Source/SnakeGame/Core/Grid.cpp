// Snake Game, Copyright LifeEXE. All Rights Reserved.

#include "Core/Grid.h"

DEFINE_LOG_CATEGORY_STATIC(LogGrid, All, All);

using namespace SnakeGame;

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
    for (uint32 y = 0; y < c_dim.height; ++y)
    {
        for (uint32 x = 0; x < c_dim.width; ++x)
        {
            if (x == 0 || x == c_dim.width - 1 || y == 0 || y == c_dim.height - 1)
            {
                const auto index = posToIndex(x, y);
                m_cells[index] = CellType::Wall;
                m_indByType[CellType::Wall].Add(index);
            }
        }
    }
}

void Grid::printDebug()
{
#if !UE_BUILD_SHIPPING
    for (uint32 y = 0; y < c_dim.height; ++y)
    {
        FString line;
        for (uint32 x = 0; x < c_dim.width; ++x)
        {
            TCHAR symbol{};
            switch (m_cells[posToIndex(x, y)])
            {
                case CellType::Empty: symbol = '0'; break;
                case CellType::Wall: symbol = '*'; break;
                case CellType::Snake: symbol = '_'; break;
                case CellType::Food: symbol = 'F'; break;
            }
            line.AppendChar(symbol).AppendChar(' ');
        }
        UE_LOG(LogGrid, Display, TEXT("%s"), *line);
    }
#endif
}

void Grid::update(const TPositionPtr* links, CellType cellType)
{
    freeCellsByType(cellType);
    auto* link = links;
    while (link)
    {
        updateInternal(link->GetValue(), cellType);
        link = link->GetNextNode();
    }
}

void Grid::update(const Position& position, CellType cellType)
{
    freeCellsByType(cellType);
    updateInternal(position, cellType);
}

void Grid::updateInternal(const Position& position, CellType cellType)
{
    const auto index = posToIndex(position);
    m_cells[index] = cellType;
    m_indByType[cellType].Add(index);
}

void Grid::freeCellsByType(CellType cellType)
{
    for (int32 i = 0; i < m_indByType[cellType].Num(); ++i)
    {
        const uint32 ind = m_indByType[cellType][i];
        m_cells[ind] = CellType::Empty;
    }
    m_indByType[cellType].Empty();
}

bool Grid::hitTest(const Position& position, CellType cellType) const
{
    return m_cells[posToIndex(position)] == cellType;
}

bool Grid::randomEmptyPosition(Position& position) const
{
    const auto gridSize = c_dim.width * c_dim.height;
    const uint32 index = FMath::RandRange(0, gridSize - 1);

    for (uint32 i = index; i < gridSize; ++i)
    {
        if (m_cells[i] == CellType::Empty)
        {
            position = indexToPos(i);
            return true;
        }
    }

    for (uint32 i = 0; i < index; ++i)
    {
        if (m_cells[i] == CellType::Empty)
        {
            position = indexToPos(i);
            return true;
        }
    }

    return false;
}

uint32 Grid::posToIndex(uint32 x, uint32 y) const
{
    return x + y * c_dim.width;
}

uint32 Grid::posToIndex(const Position& position) const
{
    return posToIndex(position.x, position.y);
}

Position Grid::indexToPos(uint32 index) const
{
    return Position(index % c_dim.width, index / c_dim.width);
}
