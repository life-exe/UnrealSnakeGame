// Snake Game, Copyright LifeEXE. All Rights Reserved.

#include "UI/SG_GameplayWidget.h"
#include "Components/TextBlock.h"
#include "World/SG_WorldUtils.h"

void USG_GameplayWidget::SetGameTime(float Seconds)
{
    if (TimeText)
    {
        TimeText->SetText(SnakeGame::WorldUtils::FormatSeconds(Seconds));
    }
}

void USG_GameplayWidget::SetScore(uint32 Score)
{
    ScoreText->SetText(SnakeGame::WorldUtils::FormatScore(Score));
}

void USG_GameplayWidget::SetResetGameKeyName(const FString& ResetGameKeyName)
{
    if (ResetGameText)
    {
        const FString ResetGameInfo = FString::Printf(TEXT("press <%s> to reset"), *ResetGameKeyName.ToLower());
        ResetGameText->SetText(FText::FromString(ResetGameInfo));
    }
}
