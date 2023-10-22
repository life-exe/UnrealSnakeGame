// Snake Game, Copyright LifeEXE. All Rights Reserved.

#include "World/SG_Snake.h"
#include "World/SG_SnakeLink.h"
#include "World/SG_WorldUtils.h"
#include "SnakeGame/Core/Snake.h"
#include "World/SG_ObjectPool.h"

namespace
{
constexpr int32 ReservedSnakeLinksNum = 10;
}

ASG_Snake::ASG_Snake()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASG_Snake::SetModel(const TSharedPtr<SnakeGame::Snake>& InSnake, uint32 InCellSize, const SnakeGame::Dim& InDims)
{
    InitObjectPool();

    Snake = InSnake;
    CellSize = InCellSize;
    Dims = InDims;

    for (auto LinkActor : SnakeLinks)
    {
        SnakeObjectPool->Add<ASG_SnakeLink>(LinkActor);
    }
    SnakeLinks.Empty();

    if (!Snake.IsValid() || !GetWorld()) return;

    const auto& Links = Snake.Pin()->links();

    for (const auto& Link : Links)
    {
        const FTransform Transform = FTransform(SnakeGame::WorldUtils::LinkPositionToVector(Link, CellSize, Dims));
        auto* LinkActor = SnakeObjectPool->Pop<ASG_SnakeLink>(GetWorld(), Transform, SnakeLinkClass);
        LinkActor->UpdateScale(CellSize);
        SnakeLinks.Add(LinkActor);
    }
}

void ASG_Snake::InitObjectPool()
{
    if (SnakeObjectPool) return;

    SnakeObjectPool = NewObject<USG_ObjectPool>();
    check(SnakeObjectPool);
    SnakeObjectPool->Reserve<ASG_SnakeLink>(GetWorld(), ReservedSnakeLinksNum, SnakeLinkClass);
}

void ASG_Snake::UpdateColors(const FSnakeColors& Colors)
{
    SnakeLinkColor = Colors.SnakeLinkColor;
    for (int32 i = 0; i < SnakeLinks.Num(); ++i)
    {
        SnakeLinks[i]->UpdateColor(i == 0 ? Colors.SnakeHeadColor : Colors.SnakeLinkColor);
    }
}

void ASG_Snake::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!Snake.IsValid()) return;

    const auto& Links = Snake.Pin()->links();
    auto* LinkPtr = Links.GetHead();

    for (auto LinkActor : SnakeLinks)
    {
        LinkActor->SetActorLocation(SnakeGame::WorldUtils::LinkPositionToVector(LinkPtr->GetValue(), CellSize, Dims));
        LinkPtr = LinkPtr->GetNextNode();
    }

    // add links if snake ate food
    while (LinkPtr)
    {
        const FTransform Transform = FTransform(SnakeGame::WorldUtils::LinkPositionToVector(LinkPtr->GetValue(), CellSize, Dims));
        auto* LinkActor = SnakeObjectPool->Pop<ASG_SnakeLink>(GetWorld(), Transform, SnakeLinkClass);
        LinkActor->UpdateScale(CellSize);
        LinkActor->UpdateColor(SnakeLinkColor);
        SnakeLinks.Add(LinkActor);
        LinkPtr = LinkPtr->GetNextNode();
    }
}

void ASG_Snake::Explode()
{
    for (auto LinkActor : SnakeLinks)
    {
        LinkActor->Explode();
    }
}
