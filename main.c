#include <stdio.h>
#include "game.h"
#include "bot.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// Structure to pass arguments to the bot thread
typedef struct {
    State *game;
} BotThreadArgs;

// Function executed by the bot thread
DWORD WINAPI botMoveThread(LPVOID args) {
    BotThreadArgs *botArgs = (BotThreadArgs *)args;
    State *game = botArgs->game;

    if (game->botDifficulty == 'e') {
        makeBotMove(game);
    } else if (game->botDifficulty == 'm') {
        makeMediumBotMove(game);
    } else {
        makeHardBotMove(game);
    }

    return 0;
}

int main() {
    State game;
    int gameMode;
    char botPlayer = ' ';
    bool botMode = false;
    char botDifficulty = 'e';

    srand(time(NULL));

    printf("=== DOTS AND BOXES ===\n");
    printf("1. Human vs. Human\n");
    printf("2. Human vs. Bot\n");
    printf("Select game mode (1-2): ");
    scanf("%d", &gameMode);

    if (gameMode == 2) {
        botMode = true;
        int choice;

        printf("Choose bot difficulty:\n");
        printf("1. Easy\n");
        printf("2. Medium\n");
        printf("3. Hard\n");
        printf("Select (1-3): ");
        scanf("%d", &choice);

        if (choice == 1)
            botDifficulty = 'e';
        else if (choice == 2)
            botDifficulty = 'm';
        else
            botDifficulty = 'h';

        printf("Choose who plays first:\n");
        printf("1. You play as Player A\n");
        printf("2. Bot plays as Player A\n");
        printf("Select (1-2): ");
        scanf("%d", &choice);
        botPlayer = (choice == 1) ? 'B' : 'A';
    }

    initializeGame(&game, botMode, botPlayer, botDifficulty);

    int x1, y1, x2, y2;
    bool gameRunning = true;

    HANDLE botThread;
    DWORD threadId;

    while (gameRunning) {
        displayGrid(&game);

        if (game.botMode && game.currentPlayer == game.botPlayer) {
            printf("Bot's turn (Player %c)\n", game.currentPlayer);

            BotThreadArgs botArgs = {&game};
            botThread = CreateThread(NULL, 0, botMoveThread, &botArgs, 0, &threadId);

            // Wait for the bot to complete its move
            WaitForSingleObject(botThread, INFINITE);
            CloseHandle(botThread);
        } else {
            printf("Player %c's turn\n", game.currentPlayer);
            printf("Enter coordinates (x1 y1 x2 y2): ");
            scanf("%d %d %d %d", &x1, &y1, &x2, &y2);

            if (isValidLine(x1, y1, x2, y2, game.grid)) {
                placeLine(&game, x1, y1, x2, y2);

                bool boxCompleted = checkForCompletedBoxes(&game, x1, y1, x2, y2);

                if (!boxCompleted) {
                    switchPlayer(&game);
                }
            } else {
                printf("Invalid move. Try again.\n");
            }
        }

        if (isGameOver(&game)) {
            displayGrid(&game);
            printf("=== GAME OVER ===\n");
            printf("Final Score: Player A: %d, Player B: %d\n", game.scoreA, game.scoreB);

            if (game.scoreA > game.scoreB) {
                printf("Player A wins!\n\n");
            } else if (game.scoreB > game.scoreA) {
                printf("Player B wins!\n\n");
            } else {
                printf("It's a tie!\n\n");
            }

            gameRunning = false;
        }
    }

    return 0;
}