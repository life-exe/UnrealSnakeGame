// Snake Game, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SnakeGame/Core/Snake.h"
#include "World/SG_WorldTypes.h"
#include "SG_Snake.generated.h"

class ASG_SnakeLink;

UCLASS()
class SNAKEGAME_API ASG_Snake : public AActor
{
    GENERATED_BODY()

public:
    ASG_Snake();

    void SetModel(const TSharedPtr<SnakeGame::Snake>& Snake, uint32 CellSize, const SnakeGame::Dim& Dims);
    void UpdateColors(const FSnakeColors& Colors);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<AActor> SnakeHeadClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<AActor> SnakeLinkClass;

public:
    virtual void Tick(float DeltaTime) override;

private:
    TWeakPtr<SnakeGame::Snake> Snake;
    uint32 CellSize;
    SnakeGame::Dim Dims;

    UPROPERTY()
    TArray<ASG_SnakeLink*> SnakeLinks;
};
