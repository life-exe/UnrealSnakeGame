// Snake Game, Copyright LifeEXE. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "SnakeGame/Core/Types.h"
#include "SG_GameUserSettings.generated.h"

UENUM()
enum class EGameSpeed : uint8
{
    Worm = 0,
    Snake,
    Python
};

UENUM()
enum class EGridSize : uint8
{
    Size_30x10 = 0,
    Size_50x15,
    Size_80x20
};

UCLASS()
class SNAKEGAME_API USG_GameUserSettings : public UGameUserSettings
{
    GENERATED_BODY()

public:
    USG_GameUserSettings();

    static USG_GameUserSettings* Get();

    TArray<FString> GameSpeedOptions() const { return OptionNames(GameSpeeds); }
    FString CurrentGameSpeedOption() const { return CurrentSpeed.Name; }

    TArray<FString> GridSizeOptions() const { return OptionNames(GridSizes); }
    FString CurrentGridSizeOption() const { return CurrentGridSize.Name; }

    void SaveSnakeSettings(EGameSpeed GameSpeed, EGridSize GridSize);

    EGameSpeed GameSpeedByName(const FString& Name) const { return FindOptionByNameOrDefault(GameSpeeds, Name, EGameSpeed::Snake); }
    EGridSize GridSizeByName(const FString& Name) const { return FindOptionByNameOrDefault(GridSizes, Name, EGridSize::Size_50x15); }

    float GameSpeed() const { return CurrentSpeed.Value; }
    SnakeGame::Dim GridSize() const { return CurrentGridSize.Value; }

private:
    struct FSpeedData
    {
        FString Name;
        float Value;
    };
    const TMap<EGameSpeed, FSpeedData> GameSpeeds  //
        {
            {EGameSpeed::Worm, {"Worm", 0.3f}},       //
            {EGameSpeed::Snake, {"Snake", 0.1f}},     //
            {EGameSpeed::Python, {"Python", 0.05f}},  //
        };

    struct FGridData
    {
        FString Name;
        SnakeGame::Dim Value;
    };
    const TMap<EGridSize, FGridData> GridSizes  //
        {
            {EGridSize::Size_30x10, {"30x10", SnakeGame::Dim{30, 10}}},  //
            {EGridSize::Size_50x15, {"50x15", SnakeGame::Dim{50, 15}}},  //
            {EGridSize::Size_80x20, {"80x20", SnakeGame::Dim{80, 20}}},  //
        };

    FSpeedData CurrentSpeed{GameSpeeds[EGameSpeed::Snake]};
    FGridData CurrentGridSize{GridSizes[EGridSize::Size_50x15]};

    template <typename MapType, typename EnumType>
    EnumType FindOptionByNameOrDefault(const MapType& Map, const FString& Name, EnumType Default) const
    {
        const auto* Founded = Algo::FindByPredicate(Map, [Name](const auto& Pair) { return Pair.Value.Name.Equals(Name); });
        return Founded ? Founded->Key : Default;
    }

    template <typename MapType>
    TArray<FString> OptionNames(const MapType& Map) const
    {
        TArray<FString> Names;
        Algo::Transform(Map, Names, [](const auto& Pair) { return Pair.Value.Name; });
        return Names;
    }
};
