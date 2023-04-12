// Snake Game, Copyright LifeEXE. All Rights Reserved.

#include "World/SG_SnakeLink.h"
#include "Components/StaticMeshComponent.h"

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

void ASG_SnakeLink::UpdateColors(const FLinearColor& Color)
{
    if (auto* LinkMaterial = LinkMesh->CreateAndSetMaterialInstanceDynamic(0))
    {
        LinkMaterial->SetVectorParameterValue("Color", Color);
    }
}

void ASG_SnakeLink::UpdateScale(uint32 CellSize)
{
    // scale mesh
    check(LinkMesh->GetStaticMesh());
    const FBox Box = LinkMesh->GetStaticMesh()->GetBoundingBox();
    const auto Size = Box.GetSize();

    check(Size.X);
    check(Size.Y);
    LinkMesh->SetRelativeScale3D(FVector(CellSize / Size.X, CellSize / Size.Y, CellSize / Size.Z));
}
