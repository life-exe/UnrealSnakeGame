// Snake Game, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "SnakeGame/Core/Game.h"
#include "SnakeGame/Core/Grid.h"
#include "SnakeGame/Core/Utils.h"

using namespace SnakeGame;

BEGIN_DEFINE_SPEC(FSnakeGame, "Snake",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
TUniquePtr<Game> CoreGame;
Settings GS;
END_DEFINE_SPEC(FSnakeGame)

class MockPositionRandomizer : public IPositionRandomizer
{
public:
    virtual bool generatePosition(const Dim& dim, const TArray<CellType>& cells, Position& position) const override
    {
        position = m_positions[m_index++];
        return true;
    }

    void setPositions(const TArray<Position>& positions)
    {
        m_positions = positions;
        m_index = 0;
    }

private:
    TArray<Position> m_positions;
    mutable int32 m_index{0};
};

void FSnakeGame::Define()
{
    Describe("Core.Game", [this]() {  //
        BeforeEach(
            [this]()
            {
                GS.gridDims = Dim{10, 10};
                GS.snake.defaultSize = 4;
                GS.snake.startPosition = Grid::center(GS.gridDims.width, GS.gridDims.height);
                GS.gameSpeed = 1.0f;
                CoreGame = MakeUnique<Game>(GS);
            });

        It("GridMightExist", [this]() { TestTrueExpr(CoreGame->grid().IsValid()); });
        It("SnakeMightExist", [this]() { TestTrueExpr(CoreGame->snake().IsValid()); });
        It("FoodMightExist", [this]() { TestTrueExpr(CoreGame->food().IsValid()); });
        It("GameCanBeOver",
            [this]()
            {
                bool bGameOver{false};
                CoreGame->subscribeOnGameplayEvent(
                    [&bGameOver](GameplayEvent Event)
                    {
                        if (Event == GameplayEvent::GameOver)
                        {
                            bGameOver = true;
                        }
                    });

                const int32 Moves = FMath::RoundToInt(GS.gridDims.width / 2.0f) - 1;
                for (int32 i = 0; i < Moves; ++i)
                {
                    CoreGame->update(1.0f, Input::Default);
                    TestTrueExpr(!bGameOver);
                }

                CoreGame->update(1.0f, Input::Default);
                TestTrueExpr(bGameOver);
            });
        It("SnakeShouldMoveCorrectlyNextToItsTail",
            [this]()
            {
                bool bGameOver{false};
                CoreGame->subscribeOnGameplayEvent(
                    [&](GameplayEvent Event)
                    {
                        if (Event == GameplayEvent::GameOver)
                        {
                            bGameOver = true;
                        }
                    });

                CoreGame->update(GS.gameSpeed, {0, 1});   // move down
                CoreGame->update(GS.gameSpeed, {-1, 0});  // move left
                CoreGame->update(GS.gameSpeed, {0, -1});  // move up

                TestTrueExpr(!bGameOver);  // the snake shouldn't bite its tail
            });
    });

    Describe("Core.Game", [this]() {  //
        It("FoodCanBeTaken",
            [this]()
            {
                auto Randomizer = MakeShared<MockPositionRandomizer>();
                Randomizer->setPositions({Position{7, 6}, Position{9, 6}, Position::Zero});

                GS.gridDims = Dim{10, 10};
                GS.snake.startPosition = Grid::center(GS.gridDims.width, GS.gridDims.height);
                GS.gameSpeed = 1.0f;
                CoreGame = MakeUnique<Game>(GS, Randomizer);

                uint32 Score = 0;
                CoreGame->subscribeOnGameplayEvent(
                    [&Score](GameplayEvent Event)
                    {
                        if (Event == GameplayEvent::FoodTaken)
                        {
                            ++Score;
                        }
                    });

                TestTrueExpr(CoreGame->score() == 0);
                TestTrueExpr(Score == 0);

                CoreGame->update(1.0f, Input::Default);
                TestTrueExpr(CoreGame->score() == 1);
                TestTrueExpr(Score == 1);

                CoreGame->update(1.0f, Input::Default);
                TestTrueExpr(CoreGame->score() == 1);
                TestTrueExpr(Score == 1);

                CoreGame->update(1.0f, Input::Default);
                TestTrueExpr(CoreGame->score() == 2);
                TestTrueExpr(Score == 2);
            });

    });
}

#endif
