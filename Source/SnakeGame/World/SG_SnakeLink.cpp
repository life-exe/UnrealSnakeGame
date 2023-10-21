// Snake Game, Copyright LifeEXE. All Rights Reserved.

#include "World/SG_SnakeLink.h"
#include "Components/StaticMeshComponent.h"
#include "World/SG_WorldUtils.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

ASG_SnakeLink::ASG_SnakeLink()
{
    PrimaryActorTick.bCanEverTick = false;

    Origin = CreateDefaultSubobject<USceneComponent>("Origin");
    check(Origin);
    SetRootComponent(Origin);

    LinkMesh = CreateDefaultSubobject<UStaticMeshComponent>("LinkMesh");
    check(LinkMesh);
    LinkMesh->SetupAttachment(Origin);
}

void ASG_SnakeLink::UpdateColor(const FLinearColor& Color)
{
    LinkColor = Color;
    if (auto* LinkMaterial = LinkMesh->CreateAndSetMaterialInstanceDynamic(0))
    {
        LinkMaterial->SetVectorParameterValue("Color", Color);
    }
}

void ASG_SnakeLink::UpdateScale(uint32 CellSize)
{
    SnakeGame::WorldUtils::ScaleMesh(LinkMesh, FVector(CellSize));
}

void ASG_SnakeLink::Explode()
{
    if (auto* NS = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, GetActorLocation()))
    {
        NS->SetVariableLinearColor("SnakeColor", LinkColor);
    }
    SetActorHiddenInGame(true);
}
