// Snake Game, Copyright LifeEXE. All Rights Reserved.

#include "World/SG_Snake.h"
#include "World/SG_SnakeLink.h"

namespace
{
FVector LinkPositionToVector(const SnakeGame::Position& Position, uint32 CellSize, const SnakeGame::Dim& Dims)
{
    return FVector((Dims.height - 1 - Position.y) * CellSize, Position.x * CellSize, 0.0) + FVector(CellSize * 0.5);
}
}  // namespace

ASG_Snake::ASG_Snake()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASG_Snake::SetModel(const TSharedPtr<SnakeGame::Snake>& InSnake, uint32 InCellSize, const SnakeGame::Dim& InDims)
{
    Snake = InSnake;
    CellSize = InCellSize;
    Dims = InDims;
}

void ASG_Snake::UpdateColors(const FSnakeColors& Colors)
{
    for (int32 i = 0; i < SnakeLinks.Num(); ++i)
    {
        SnakeLinks[i]->UpdateColors(i == 0 ? Colors.SnakeHeadColor : Colors.SnakeLinkColor);
    }
}

void ASG_Snake::BeginPlay()
{
    Super::BeginPlay();

    if (!Snake.IsValid() || !GetWorld()) return;

    const auto& Links = Snake.Pin()->links();

    uint32 i = 0;
    for (const auto& Link : Links)
    {
        const bool IsHead = i == 0;
        const FTransform Transform = FTransform(LinkPositionToVector(Link, CellSize, Dims));
        auto* LinkActor = GetWorld()->SpawnActorDeferred<ASG_SnakeLink>(IsHead ? SnakeHeadClass : SnakeLinkClass, Transform);
        LinkActor->UpdateScale(CellSize);
        LinkActor->FinishSpawning(Transform);
        SnakeLinks.Add(LinkActor);
        ++i;
    }
}

void ASG_Snake::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!Snake.IsValid()) return;

    const auto& Links = Snake.Pin()->links();
    auto* LinkPtr = Links.GetHead();

    for (auto* LinkActor : SnakeLinks)
    {
        LinkActor->SetActorLocation(LinkPositionToVector(LinkPtr->GetValue(), CellSize, Dims));
        LinkPtr = LinkPtr->GetNextNode();
    }
}
