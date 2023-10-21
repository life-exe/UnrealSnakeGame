// Snake Game, Copyright LifeEXE. All Rights Reserved.

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "SnakeGame/Core/Snake.h"
#include "SnakeGame/Core/Types.h"

using namespace SnakeGame;

BEGIN_DEFINE_SPEC(FSnake, "Snake",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
TUniquePtr<Snake> CoreSnake;
Settings::Snake Config;
END_DEFINE_SPEC(FSnake)

void FSnake::Define()
{
    Describe("Core.Snake",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    Config.defaultSize = 10;
                    Config.startPosition = Position(100, 0);
                    CoreSnake = MakeUnique<Snake>(Config);
                });

            It("ShouldBeInitCorrectly",
                [this]()
                {
                    Position LinkPos = Config.startPosition;

                    TestTrueExpr(CoreSnake->head() == LinkPos);
                    TestTrueExpr(CoreSnake->links().Num() == Config.defaultSize);

                    auto* Link = CoreSnake->links().GetHead();
                    while (Link)
                    {
                        TestTrueExpr(Link->GetValue() == LinkPos);
                        Link = Link->GetNextNode();
                        --LinkPos.x;
                    }
                });

            It("Movement.CanBeMovedRight",
                [this]()
                {
                    Position LinkPos = Config.startPosition;
                    TestTrueExpr(CoreSnake->head() == LinkPos);
                    CoreSnake->move(Input{1, 0});

                    auto* Link = CoreSnake->links().GetHead();
                    while (Link)
                    {
                        TestTrueExpr(Link->GetValue() == Position(LinkPos.x + 1, LinkPos.y));
                        Link = Link->GetNextNode();
                        --LinkPos.x;
                    }
                });

            It("Movement.CannotBeMovedLeftByDefault",
                [this]()
                {
                    Position LinkPos = Config.startPosition;
                    TestTrueExpr(CoreSnake->head() == LinkPos);
                    CoreSnake->move(Input{-1, 0});

                    auto* Link = CoreSnake->links().GetHead();
                    while (Link)
                    {
                        TestTrueExpr(Link->GetValue() == Position(LinkPos.x + 1, LinkPos.y));
                        Link = Link->GetNextNode();
                        --LinkPos.x;
                    }
                });

            It("Movement.CanBeMovedUp",
                [this]()
                {
                    Position LinkPos = Config.startPosition;
                    TestTrueExpr(CoreSnake->head() == LinkPos);
                    CoreSnake->move(Input{0, 1});

                    auto* Link = CoreSnake->links().GetHead();
                    TestTrueExpr(Link->GetValue() == Position(LinkPos.x, LinkPos.y + 1));

                    Link = Link->GetNextNode();
                    while (Link)
                    {
                        TestTrueExpr(Link->GetValue() == LinkPos);
                        Link = Link->GetNextNode();
                        --LinkPos.x;
                    }
                });

            It("Movement.CanBeMovedDown",
                [this]()
                {
                    Position LinkPos = Config.startPosition;
                    TestTrueExpr(CoreSnake->head() == LinkPos);
                    CoreSnake->move(Input{0, -1});

                    auto* Link = CoreSnake->links().GetHead();
                    TestTrueExpr(Link->GetValue() == Position(LinkPos.x, LinkPos.y - 1));

                    Link = Link->GetNextNode();
                    while (Link)
                    {
                        TestTrueExpr(Link->GetValue() == LinkPos);
                        Link = Link->GetNextNode();
                        --LinkPos.x;
                    }
                });

            It("Size.CanBeIncreased",
                [this]()
                {
                    CoreSnake->increase();
                    TestTrueExpr(CoreSnake->links().Num() == Config.defaultSize + 1);
                    CoreSnake->increase();
                    TestTrueExpr(CoreSnake->links().Num() == Config.defaultSize + 2);
                    CoreSnake->increase();
                    TestTrueExpr(CoreSnake->links().Num() == Config.defaultSize + 3);
                    // etc.
                });
        });
}

#endif
