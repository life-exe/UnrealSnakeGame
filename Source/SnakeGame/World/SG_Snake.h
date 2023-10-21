// Snake Game, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "World/SG_WorldTypes.h"
#include "Core/Types.h"
#include "SG_Snake.generated.h"

namespace SnakeGame
{
class Snake;
}

class ASG_SnakeLink;
class USG_SnakeObjectPool;

UCLASS()
class SNAKEGAME_API ASG_Snake : public AActor
{
    GENERATED_BODY()

public:
    ASG_Snake();

    /**
     * Set core model to be observed by the Snake world actor
     * @param Snake  model object
     * @param CellSize  world size of the cell
     * @param Dims  dimensions of the grid
     */
    void SetModel(const TSharedPtr<SnakeGame::Snake>& Snake, uint32 CellSize, const SnakeGame::Dim& Dims);
    void UpdateColors(const FSnakeColors& Colors);

    void Explode();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<AActor> SnakeLinkClass;

public:
    virtual void Tick(float DeltaTime) override;

private:
    TWeakPtr<SnakeGame::Snake> Snake;
    uint32 CellSize;
    SnakeGame::Dim Dims;
    FLinearColor SnakeLinkColor;

    UPROPERTY()
    TArray<TObjectPtr<ASG_SnakeLink>> SnakeLinks;

    UPROPERTY()
    TObjectPtr<USG_SnakeObjectPool> SnakeObjectPool{nullptr};

    void InitObjectPool();
};
