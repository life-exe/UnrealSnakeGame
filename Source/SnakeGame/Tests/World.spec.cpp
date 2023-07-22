// Snake Game, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "SnakeGame/Tests/Utils/TestUtils.h"
#include "SnakeGame/World/SG_Grid.h"
#include "SnakeGame/World/SG_Snake.h"
#include "SnakeGame/World/SG_Food.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "SnakeGame/Core/Grid.h"
#include "SnakeGame/World/SG_WorldTypes.h"
#include "SnakeGame/World/SG_WorldUtils.h"

BEGIN_DEFINE_SPEC(FSnakeWorld, "Snake",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
UWorld* World;
SnakeGame::Dim Dims;
uint32 CellSize;
TSharedPtr<SnakeGame::Grid> ModelGrid;
UStaticMeshComponent* GridStaticMesh;
ASG_Grid* GridVisual;
END_DEFINE_SPEC(FSnakeWorld)

void FSnakeWorld::Define()
{
    using namespace LifeExe::Test;

    Describe("World.Grid",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap("/Game/Tests/TestEmptyLevel");
                    World = GetTestGameWorld();

                    constexpr char* GridBPName = "Blueprint'/Game/World/BP_SnakeGrid.BP_SnakeGrid'";

                    Dims = SnakeGame::Dim{10, 10};
                    CellSize = 20;
                    ModelGrid = MakeShared<SnakeGame::Grid>(Dims);

                    const FTransform Origin = FTransform::Identity;
                    GridVisual = CreateBlueprintDeferred<ASG_Grid>(World, GridBPName, Origin);
                    GridVisual->SetModel(ModelGrid, CellSize);
                    GridVisual->FinishSpawning(Origin);

                    auto* Comp = GridVisual->GetComponentByClass(UStaticMeshComponent::StaticClass());
                    GridStaticMesh = Cast<UStaticMeshComponent>(Comp);
                });

            It("StaticGridMightHaveCorrectTransform",
                [this]()
                {
                    const FBox Box = GridStaticMesh->GetStaticMesh()->GetBoundingBox();
                    const auto Size = Box.GetSize();

                    const auto WorldWidth = ModelGrid->dim().width * CellSize;
                    const auto WorldHeight = ModelGrid->dim().height * CellSize;

                    TestTrueExpr(GridStaticMesh->GetRelativeLocation().Equals(0.5 * FVector(WorldHeight, WorldWidth, -Size.Z)));
                    TestTrueExpr(GridStaticMesh->GetRelativeScale3D().Equals(FVector(WorldHeight / Size.X, WorldWidth / Size.Y, 1.0)));
                });

            It("ColorsMightBeSetupCorrectly",
                [this]()
                {
                    FSnakeColors Colors;
                    Colors.GridBackgroundColor = FLinearColor::Green;
                    Colors.GridWallColor = FLinearColor::Red;
                    Colors.GridLineColor = FLinearColor::Yellow;
                    GridVisual->UpdateColors(Colors);

                    auto* Material = GridStaticMesh->GetMaterial(0);
                    FLinearColor ColorToCheck;

                    Material->GetVectorParameterValue(FName("BackgroundColor"), ColorToCheck);
                    TestTrueExpr(ColorToCheck.Equals(Colors.GridBackgroundColor));

                    Material->GetVectorParameterValue(FName("WallColor"), ColorToCheck);
                    TestTrueExpr(ColorToCheck.Equals(Colors.GridWallColor));

                    Material->GetVectorParameterValue(FName("LineColor"), ColorToCheck);
                    TestTrueExpr(ColorToCheck.Equals(Colors.GridLineColor));
                });
        });

    Describe("World",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap("/Game/Levels/GameLevel");
                    World = GetTestGameWorld();
                });
            It("OnlyOneValidModelActorShouldExist",
                [this]()
                {
                    TArray<AActor*> Actors;

                    UGameplayStatics::GetAllActorsOfClass(World, ASG_Grid::StaticClass(), Actors);
                    TestTrueExpr(Actors.Num() == 1);
                    TestNotNull("Grid actor exists", Actors[0]);

                    UGameplayStatics::GetAllActorsOfClass(World, ASG_Snake::StaticClass(), Actors);
                    TestTrueExpr(Actors.Num() == 1);
                    TestNotNull("Snake actor exists", Actors[0]);

                    UGameplayStatics::GetAllActorsOfClass(World, ASG_Food::StaticClass(), Actors);
                    TestTrueExpr(Actors.Num() == 1);
                    TestNotNull("Food actor exists", Actors[0]);
                });
        });

    Describe("World.Utils",
        [this]()
        {
            It("SecondsShouldBeFormattedCorrectly",
                [this]()
                {
                    using namespace SnakeGame;

                    TestTrueExpr(WorldUtils::FormatSeconds(0.0f).EqualTo(FText::FromString("00:00")));
                    TestTrueExpr(WorldUtils::FormatSeconds(7.0f).EqualTo(FText::FromString("00:07")));
                    TestTrueExpr(WorldUtils::FormatSeconds(10.0f).EqualTo(FText::FromString("00:10")));
                    TestTrueExpr(WorldUtils::FormatSeconds(65.0f).EqualTo(FText::FromString("01:05")));
                    TestTrueExpr(WorldUtils::FormatSeconds(605.0f).EqualTo(FText::FromString("10:05")));
                    TestTrueExpr(WorldUtils::FormatSeconds(3599.0f).EqualTo(FText::FromString("59:59")));
                    TestTrueExpr(WorldUtils::FormatSeconds(3600.0f).EqualTo(FText::FromString("60:00")));
                    TestTrueExpr(WorldUtils::FormatSeconds(3601.0f).EqualTo(FText::FromString("60:01")));
                    TestTrueExpr(WorldUtils::FormatSeconds(4200.0f).EqualTo(FText::FromString("70:00")));
                });

            It("ScoreShouldBeFormattedCorrectly",
                [this]()
                {
                    using namespace SnakeGame;

                    TestTrueExpr(WorldUtils::FormatScore(0).EqualTo(FText::FromString("00")));
                    TestTrueExpr(WorldUtils::FormatScore(7).EqualTo(FText::FromString("07")));
                    TestTrueExpr(WorldUtils::FormatScore(13).EqualTo(FText::FromString("13")));
                    TestTrueExpr(WorldUtils::FormatScore(999).EqualTo(FText::FromString("999")));
                });
        });
}

#endif
