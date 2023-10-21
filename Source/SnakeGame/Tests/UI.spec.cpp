// Snake Game, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "SnakeGame/Tests/Utils/TestUtils.h"
#include "UI/SG_StartGameWidget.h"
#include "UI/SG_GameplayWidget.h"
#include "UI/SG_GameOverWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"

BEGIN_DEFINE_SPEC(FSnakeUI, "Snake",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
UWorld* World;
END_DEFINE_SPEC(FSnakeUI)

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FGameOverWidgetVisibleLatentCommand, USG_GameOverWidget*, Widget);
bool FGameOverWidgetVisibleLatentCommand::Update()
{
    return Widget->GetVisibility() == ESlateVisibility::Visible;
}

void FSnakeUI::Define()
{
    using namespace LifeExe::Test;

    Describe("UI",
        [this]()
        {
            It("AllMenuComponentsShouldExist",
                [this]()
                {
                    AutomationOpenMap("/Game/Levels/MenuLevel");
                    World = GetTestGameWorld();

                    auto* StartGameWidget = FindWidgetByClass<USG_StartGameWidget>();
                    TestTrueExpr(StartGameWidget != nullptr);

                    auto* StartGameButton = Cast<UButton>(FindWidgetByName(StartGameWidget, "StartGameButton"));
                    TestTrueExpr(StartGameButton != nullptr);

                    auto* CloseGameButton = Cast<UButton>(FindWidgetByName(StartGameWidget, "CloseGameButton"));
                    TestTrueExpr(CloseGameButton != nullptr);

                    auto* GameSpeedCombobox = Cast<UComboBoxString>(FindWidgetByName(StartGameWidget, "GameSpeedCombobox"));
                    TestTrueExpr(GameSpeedCombobox != nullptr);

                    auto* GridSizeCombobox = Cast<UComboBoxString>(FindWidgetByName(StartGameWidget, "GridSizeCombobox"));
                    TestTrueExpr(GridSizeCombobox != nullptr);
                });

            It("AllGameplayComponentsShouldExist",
                [this]()
                {
                    AutomationOpenMap("/Game/Levels/GameLevel");
                    World = GetTestGameWorld();

                    auto* GameplayWidget = FindWidgetByClass<USG_GameplayWidget>();
                    TestTrueExpr(GameplayWidget != nullptr);

                    auto* TimeText = Cast<UTextBlock>(FindWidgetByName(GameplayWidget, "TimeText"));
                    TestTrueExpr(TimeText != nullptr);

                    auto* ScoreText = Cast<UTextBlock>(FindWidgetByName(GameplayWidget, "ScoreText"));
                    TestTrueExpr(ScoreText != nullptr);

                    auto* ResetGameText = Cast<UTextBlock>(FindWidgetByName(GameplayWidget, "ResetGameText"));
                    TestTrueExpr(ResetGameText != nullptr);
                });

            It("AllGameOverComponentsShouldExist",
                [this]()
                {
                    AutomationOpenMap("/Game/Levels/GameLevel");
                    World = GetTestGameWorld();
                    auto* GameOverWidget = FindWidgetByClass<USG_GameOverWidget>();

                    TestTrueExpr(GameOverWidget != nullptr);
                    TestTrueExpr(GameOverWidget->GetVisibility() == ESlateVisibility::Collapsed);

                    ADD_LATENT_AUTOMATION_COMMAND(FGameOverWidgetVisibleLatentCommand(GameOverWidget));
                    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
                        [this, GameOverWidget]()
                        {
                            auto* ScoreText = Cast<UTextBlock>(FindWidgetByName(GameOverWidget, "ScoreText"));
                            TestTrueExpr(ScoreText != nullptr);

                            auto* ResetGameText = Cast<UTextBlock>(FindWidgetByName(GameOverWidget, "ResetGameText"));
                            TestTrueExpr(ResetGameText != nullptr);

                            auto* BackToMenuButton = Cast<UButton>(FindWidgetByName(GameOverWidget, "BackToMenuButton"));
                            TestTrueExpr(BackToMenuButton != nullptr);
                            return true;
                        }));
                });
        });
}

#endif
