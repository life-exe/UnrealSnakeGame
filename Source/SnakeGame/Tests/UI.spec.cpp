// Snake Game, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "SnakeGame/Tests/Utils/TestUtils.h"
#include "UI/SG_StartGameWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"

BEGIN_DEFINE_SPEC(FSnakeUI, "Snake",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
UWorld* World;
END_DEFINE_SPEC(FSnakeUI)

void FSnakeUI::Define()
{
    using namespace LifeExe::Test;

    Describe("UI",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap("/Game/Levels/MenuLevel");
                    World = GetTestGameWorld();
                });

            It("AllMenuComponentsShouldExist",
                [this]()
                {
                    auto* StartGameWidget = FindWidgetByClass<USG_StartGameWidget>();

                    auto* StartGameButton = Cast<UButton>(FindWidgetByName(StartGameWidget, "StartGameButton"));
                    TestTrueExpr(StartGameButton != nullptr);

                    auto* CloseGameButton = Cast<UButton>(FindWidgetByName(StartGameWidget, "CloseGameButton"));
                    TestTrueExpr(CloseGameButton != nullptr);

                    auto* GameSpeedCombobox = Cast<UComboBoxString>(FindWidgetByName(StartGameWidget, "GameSpeedCombobox"));
                    TestTrueExpr(GameSpeedCombobox != nullptr);

                    auto* GridSizeCombobox = Cast<UComboBoxString>(FindWidgetByName(StartGameWidget, "GridSizeCombobox"));
                    TestTrueExpr(GridSizeCombobox != nullptr);
                });

            xIt("AllGameplayComponentsShouldExist", [this]() { unimplemented(); });
            xIt("AllGameOverComponentsShouldExist", [this]() { unimplemented(); });
        });
}

#endif
