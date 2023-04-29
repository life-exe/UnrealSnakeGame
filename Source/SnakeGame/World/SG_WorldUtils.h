#pragma once

#include "SnakeGame/Core/Types.h"
#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"

namespace SnakeGame
{
class WorldUtils
{
public:
    static FVector LinkPositionToVector(const SnakeGame::Position& Position, uint32 CellSize, const SnakeGame::Dim& Dims)
    {
        return FVector((Dims.height - 1 - Position.y) * CellSize, Position.x * CellSize, 0.0) + FVector(CellSize * 0.5);
    }

    static void ScaleMesh(UStaticMeshComponent* Mesh, const FVector& WorldSize)
    {
        if (Mesh && Mesh->GetStaticMesh())
        {
            const FBox Box = Mesh->GetStaticMesh()->GetBoundingBox();
            const auto Size = Box.GetSize();

            check(!Size.IsZero());
            Mesh->SetRelativeScale3D(FVector(WorldSize / Size));
        }
    }
};
}  // namespace SnakeGame
