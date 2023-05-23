// Snake Game, Copyright LifeEXE. All Rights Reserved.

#include "Framework/SG_GameMode.h"
#include "SG_GameMode.h"
#include "SnakeGame/Core/Types.h"
#include "SnakeGame/Core/Grid.h"
#include "World/SG_Grid.h"
#include "World/SG_Snake.h"
#include "World/SG_Food.h"
#include "World/SG_WorldTypes.h"
#include "Framework/SG_Pawn.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogSnakeGameMode, All, All);

ASG_GameMode::ASG_GameMode()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ASG_GameMode::StartPlay()
{
    Super::StartPlay();

    // init core game
    Game = MakeUnique<SnakeGame::Game>(MakeSettings());
    check(Game.IsValid());
    SubscribeOnGameEvents();

    // init world grid
    const FTransform GridOrigin = FTransform::Identity;
    check(GetWorld());
    GridVisual = GetWorld()->SpawnActorDeferred<ASG_Grid>(GridVisualClass, GridOrigin);
    check(GridVisual);
    GridVisual->SetModel(Game->grid(), CellSize);
    GridVisual->FinishSpawning(GridOrigin);

    // init world snake
    SnakeVisual = GetWorld()->SpawnActorDeferred<ASG_Snake>(SnakeVisualClass, GridOrigin);
    SnakeVisual->SetModel(Game->snake(), CellSize, Game->grid()->dim());
    SnakeVisual->FinishSpawning(GridOrigin);

    // init world food
    FoodVisual = GetWorld()->SpawnActorDeferred<ASG_Food>(FoodVisualClass, GridOrigin);
    FoodVisual->SetModel(Game->food(), CellSize, Game->grid()->dim());
    FoodVisual->FinishSpawning(GridOrigin);

    // set pawn location fitting grid in viewport
    auto* PC = GetWorld()->GetFirstPlayerController();
    check(PC);

    auto* Pawn = Cast<ASG_Pawn>(PC->GetPawn());
    check(Pawn);
    check(Game->grid().IsValid());
    Pawn->UpdateLocation(Game->grid()->dim(), CellSize, GridOrigin);

    //
    FindFog();

    // update colors
    check(ColorsTable);
    const auto RowsCount = ColorsTable->GetRowNames().Num();
    check(RowsCount >= 1);
    ColorTableIndex = FMath::RandRange(0, RowsCount - 1);
    UpdateColors();

    //
    SetupInput();
}

void ASG_GameMode::NextColor()
{
    if (ColorsTable)
    {
        ColorTableIndex = (ColorTableIndex + 1) % ColorsTable->GetRowNames().Num();
        UpdateColors();
    }
}

void ASG_GameMode::FindFog()
{
    TArray<AActor*> Fogs;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AExponentialHeightFog::StaticClass(), Fogs);
    if (Fogs.Num() > 0)
    {
        Fog = Cast<AExponentialHeightFog>(Fogs[0]);
    }
}

void ASG_GameMode::UpdateColors()
{
    const auto RowName = ColorsTable->GetRowNames()[ColorTableIndex];
    const auto* ColorSet = ColorsTable->FindRow<FSnakeColors>(RowName, {});
    if (ColorSet)
    {
        GridVisual->UpdateColors(*ColorSet);
        SnakeVisual->UpdateColors(*ColorSet);
        FoodVisual->UpdateColor(ColorSet->FoodColor);

        // update scene ambient color via fog
        if (Fog && Fog->GetComponent())
        {
            Fog->GetComponent()->SkyAtmosphereAmbientContributionColorScale = ColorSet->SkyAtmosphereColor;
            Fog->MarkComponentsRenderStateDirty();
        }
    }
}

void ASG_GameMode::SetupInput()
{
    if (!GetWorld()) return;

    if (auto* PC = GetWorld()->GetFirstPlayerController())
    {
        if (auto* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            InputSystem->AddMappingContext(InputMapping, 0);
        }

        auto* Input = Cast<UEnhancedInputComponent>(PC->InputComponent);
        check(Input);
        Input->BindAction(MoveForwardInputAction, ETriggerEvent::Started, this, &ThisClass::OnMoveForward);
        Input->BindAction(MoveRightInputAction, ETriggerEvent::Started, this, &ThisClass::OnMoveRight);
        Input->BindAction(ResetGameInputAction, ETriggerEvent::Started, this, &ThisClass::OnGameReset);
    }
}

void ASG_GameMode::OnMoveForward(const FInputActionValue& Value)
{
    const float InputValue = Value.Get<float>();
    if (InputValue == 0.0f) return;
    SnakeInput = SnakeGame::Input{0, static_cast<int8>(InputValue)};
}

void ASG_GameMode::OnMoveRight(const FInputActionValue& Value)
{
    const float InputValue = Value.Get<float>();
    if (InputValue == 0.0f) return;
    SnakeInput = SnakeGame::Input{static_cast<int8>(InputValue), 0};
}

void ASG_GameMode::OnGameReset(const FInputActionValue& Value)
{
    if (const bool InputValue = Value.Get<bool>())
    {
        Game.Reset(new SnakeGame::Game(MakeSettings()));
        check(Game.IsValid());
        SubscribeOnGameEvents();
        GridVisual->SetModel(Game->grid(), CellSize);
        SnakeVisual->SetModel(Game->snake(), CellSize, Game->grid()->dim());
        FoodVisual->SetModel(Game->food(), CellSize, Game->grid()->dim());
        SnakeInput = SnakeGame::Input::Default;
        NextColor();
    }
}

void ASG_GameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (Game.IsValid())
    {
        Game->update(DeltaSeconds, SnakeInput);
    }
}

SnakeGame::Settings ASG_GameMode::MakeSettings() const
{
    SnakeGame::Settings GS;
    GS.gridDims = SnakeGame::Dim{GridDims.X, GridDims.Y};
    GS.gameSpeed = GameSpeed;
    GS.snake.defaultSize = SnakeDefaultSize;
    GS.snake.startPosition = SnakeGame::Grid::center(GridDims.X, GridDims.Y);
    return GS;
}

void ASG_GameMode::SubscribeOnGameEvents()
{
    using namespace SnakeGame;

    Game->subscribeOnGameplayEvent(
        [&](GameplayEvent Event)
        {
            switch (Event)
            {
                case GameplayEvent::GameOver:
                    UE_LOG(LogSnakeGameMode, Display, TEXT("-------------- GAME OVER --------------"));
                    UE_LOG(LogSnakeGameMode, Display, TEXT("-------------- SCORE: %i --------------"), Game->score());
                    SnakeVisual->Explode();
                    break;
                case GameplayEvent::GameCompleted:
                    UE_LOG(LogSnakeGameMode, Display, TEXT("-------------- GAME COMPLETED --------------"));
                    UE_LOG(LogSnakeGameMode, Display, TEXT("-------------- SCORE: %i --------------"), Game->score());
                    break;
                case GameplayEvent::FoodTaken:  //
                    UE_LOG(LogSnakeGameMode, Display, TEXT("-------------- FOOD TAKEN --------------"));
                    FoodVisual->Explode();
                    break;
            }
        });
}
