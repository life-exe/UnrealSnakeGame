// Snake Game, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SG_WorldTypes.generated.h"

USTRUCT(BlueprintType)
struct FSnakeColors : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor GridBackgroundColor{FLinearColor::Black};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor GridWallColor{FLinearColor::Red};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor GridLineColor{FLinearColor::White};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor SkyAtmosphereColor{FLinearColor::Blue};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor SnakeHeadColor{FLinearColor::White};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor SnakeLinkColor{FLinearColor::Green};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    FLinearColor FoodColor{FLinearColor::Red};
};