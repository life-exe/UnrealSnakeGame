// Snake Game, Copyright LifeEXE. All Rights Reserved.

#include "UI/SG_MenuHUD.h"
#include "UI/SG_StartGameWidget.h"

void ASG_MenuHUD::BeginPlay()
{
    Super::BeginPlay();

    StartGameWidget = CreateWidget<USG_StartGameWidget>(GetWorld(), StartGameWidgetClass);
    check(StartGameWidget);
    StartGameWidget->AddToViewport();
}
