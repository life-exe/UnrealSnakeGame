// Snake Game, Copyright LifeEXE. All Rights Reserved.

#include "Framework/SG_Pawn.h"
#include "Camera/CameraComponent.h"

namespace
{
double FOVTan(double FOVDegrees)
{
    return FMath::Tan(FMath::DegreesToRadians(FOVDegrees * 0.5));
}

double VerticalFOV(double HorFOVDegrees, double ViewportAspectHW)
{
    // https://en.m.wikipedia.org/wiki/Field_of_view_in_video_games
    return FMath::RadiansToDegrees(2.0 * FMath::Atan(FMath::Tan(FMath::DegreesToRadians(HorFOVDegrees) * 0.5) * ViewportAspectHW));
}

constexpr double GridMargin = 2.0;

}  // namespace

ASG_Pawn::ASG_Pawn()
{
    PrimaryActorTick.bCanEverTick = false;

    Origin = CreateDefaultSubobject<USceneComponent>("Origin");
    check(Origin);
    SetRootComponent(Origin);

    Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
    check(Camera);
    Camera->SetupAttachment(Origin);
}

void ASG_Pawn::UpdateLocation(const Snake::Dim& InDim, int32 InCellSize, const FTransform& InGridOrigin)
{
    Dim = InDim;
    CellSize = InCellSize;
    GridOrigin = InGridOrigin;

    check(GEngine);
    check(GEngine->GameViewport);
    check(GEngine->GameViewport->Viewport);

    auto* Viewport = GEngine->GameViewport->Viewport;
    Viewport->ViewportResizedEvent.Remove(ResizeHandle);
    ResizeHandle = Viewport->ViewportResizedEvent.AddUObject(this, &ASG_Pawn::OnViewportResized);

#if WITH_EDITOR
    OnViewportResized(Viewport, 0);
#endif
}

void ASG_Pawn::OnViewportResized(FViewport* Viewport, uint32 Val)
{
    if (!Viewport || Viewport->GetSizeXY().Y == 0 || Dim.height == 0) return;

    const double WorldWidth = Dim.width * CellSize;
    const double WorldHeight = Dim.height * CellSize;

    double LocationZ = 0.0;
    const double ViewportAspect = static_cast<double>(Viewport->GetSizeXY().X) / Viewport->GetSizeXY().Y;
    const double GridAspect = static_cast<double>(Dim.width) / Dim.height;

    if (ViewportAspect <= GridAspect)
    {
        const double MarginWidth = (Dim.width + GridMargin) * CellSize;
        LocationZ = MarginWidth / FOVTan(Camera->FieldOfView);
    }
    else
    {
        check(ViewportAspect);
        const double VFOV = VerticalFOV(Camera->FieldOfView, 1.0 / ViewportAspect);
        const double MarginHeight = (Dim.height + GridMargin) * CellSize;
        LocationZ = MarginHeight / FOVTan(VFOV);
    }

    const FVector NewPawnLocation = GridOrigin.GetLocation() + 0.5 * FVector(WorldHeight, WorldWidth, LocationZ);
    SetActorLocation(NewPawnLocation);
}
