// Snake Game, Copyright LifeEXE. All Rights Reserved.

#include "UI/SG_GameOverWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "World/SG_WorldUtils.h"
#include "Kismet/GameplayStatics.h"

void USG_GameOverWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(BackToMenuButton);
    BackToMenuButton->OnClicked.AddDynamic(this, &ThisClass::OnBackToMenu);
}

void USG_GameOverWidget::SetScore(uint32 Score)
{
    if (ScoreText)
    {
        ScoreText->SetText(SnakeGame::WorldUtils::FormatScore(Score));
    }
}

void USG_GameOverWidget::SetResetGameKeyName(const FString& ResetGameKeyName)
{
    if (ResetGameText)
    {
        const FString ResetGameInfo = FString::Printf(TEXT("press <%s> to reset"), *ResetGameKeyName.ToLower());
        ResetGameText->SetText(FText::FromString(ResetGameInfo));
    }
}

void USG_GameOverWidget::OnBackToMenu()
{
    UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), MenuLevel);
}
