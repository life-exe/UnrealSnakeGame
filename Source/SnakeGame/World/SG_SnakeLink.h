// Snake Game, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SG_SnakeLink.generated.h"

class UStaticMeshComponent;
class UNiagaraSystem;

UCLASS()
class SNAKEGAME_API ASG_SnakeLink : public AActor
{
    GENERATED_BODY()

public:
    ASG_SnakeLink();

    void UpdateColor(const FLinearColor& Color);
    void UpdateScale(uint32 CellSize);
    void Explode();

protected:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USceneComponent> Origin;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> LinkMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TObjectPtr<UNiagaraSystem> ExplosionEffect;

private:
    FLinearColor LinkColor;
};
