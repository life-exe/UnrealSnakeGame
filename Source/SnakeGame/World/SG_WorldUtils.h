// Snake Game, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "SnakeGame/Core/Types.h"
#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedActionKeyMapping.h"

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

    static FText FormatSeconds(float TimeSeconds)
    {
        const int32 TotalSeconds = FMath::RoundToInt(TimeSeconds);
        // const int32 Hours = TotalSeconds / 3600;
        const int32 Minutes = (TotalSeconds % 3600) / 60;
        const int32 Seconds = TotalSeconds % 60;
        const FString FormattedTime = FString::Printf(TEXT("%02i:%02i"), Minutes, Seconds);
        return FText::FromString(FormattedTime);
    }

    static FText FormatScore(uint32 Score)
    {
        const FString FormattedScore = FString::Printf(TEXT("%02i"), Score);
        return FText::FromString(FormattedScore);
    }

    static FString FindActionKeyName(const TObjectPtr<UInputMappingContext>& InputMapping, const TObjectPtr<UInputAction>& Action)
    {
        auto* FoundAcionKeyMapping = InputMapping->GetMappings().FindByPredicate([&](const FEnhancedActionKeyMapping& Mapping)  //
            { return Mapping.Action == Action; });

        return FoundAcionKeyMapping ? FoundAcionKeyMapping->Key.GetDisplayName().ToString() : FString{};
    }
};
}  // namespace SnakeGame
