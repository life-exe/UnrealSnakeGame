// Snake Game, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SG_MenuHUD.generated.h"

class USG_StartGameWidget;

UCLASS()
class SNAKEGAME_API ASG_MenuHUD : public AHUD
{
    GENERATED_BODY()

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<USG_StartGameWidget> StartGameWidgetClass;

    virtual void BeginPlay() override;

private:
    UPROPERTY()
    TObjectPtr<USG_StartGameWidget> StartGameWidget;
};
