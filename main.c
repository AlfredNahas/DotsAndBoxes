#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 9
#define COLUMNS 11
#define DOT_ROWS ((ROWS / 2) + 1)
#define DOT_COLS ((COLUMNS / 2) + 1)
#define BOX_ROWS (DOT_ROWS - 1)
#define BOX_COLS (DOT_COLS - 1)
#define TOTAL_LINES ((BOX_ROWS * (BOX_COLS + 1)) + (BOX_COLS * (BOX_ROWS + 1)))

typedef struct
{
    char grid[ROWS][COLUMNS];
    char currentPlayer;
    int completedBoxes;
    int scoreA;
    int scoreB;
    int drawnLines;
    bool botMode;
    char botPlayer;
    char botDifficulty;
} State;

void copyGrid(char source[ROWS][COLUMNS], char dest[ROWS][COLUMNS])
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            dest[i][j] = source[i][j];
        }
    }
}

void initializeGame(State *game, bool botMode, char botPlayer, char botDifficulty)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLUMNS; j++)
        {
            game->grid[i][j] = ' ';
        }
    }

    for (int i = 0; i < ROWS; i += 2)
    {
        for (int j = 0; j < COLUMNS; j += 2)
        {
            game->grid[i][j] = '.';
        }
    }

    game->currentPlayer = 'A';
    game->scoreA = 0;
    game->scoreB = 0;
    game->completedBoxes = 0;
    game->drawnLines = 0;
    game->botMode = botMode;
    game->botPlayer = botPlayer;
    game->botDifficulty = botDifficulty;
}

bool isValidLine(int x1, int y1, int x2, int y2, char grid[ROWS][COLUMNS])
{
    int row1 = 2 * x1, col1 = 2 * y1;
    int row2 = 2 * x2, col2 = 2 * y2;

    if (row1 < 0 || row1 >= ROWS || col1 < 0 || col1 >= COLUMNS ||
        row2 < 0 || row2 >= ROWS || col2 < 0 || col2 >= COLUMNS)
    {
        return false;
    }

    if ((row1 == row2 && abs(col1 - col2) == 2) ||
        (col1 == col2 && abs(row1 - row2) == 2))
    {
        int midRow = (row1 + row2) / 2;
        int midCol = (col1 + col2) / 2;

        return grid[midRow][midCol] == ' ';
    }
    return false;
}

void switchPlayer(State *game)
{
    game->currentPlayer = (game->currentPlayer == 'A') ? 'B' : 'A';
}

bool isGameOver(State *game)
{
    return game->drawnLines >= TOTAL_LINES;
}

bool checkForCompletedBoxes(State *game, int x1, int y1, int x2, int y2)
{
    int row1 = 2 * x1, col1 = 2 * y1;
    int row2 = 2 * x2, col2 = 2 * y2;
    bool boxCompleted = false;

    bool isHorizontal = (row1 == row2);

    if (isHorizontal)
    {
        int midRow = row1 / 2;
        int midCol = (col1 < col2 ? col1 : col2) / 2;

        if (midRow > 0)
        {
            int boxRow = (midRow - 1) * 2 + 1;
            int boxCol = midCol * 2 + 1;

            if (game->grid[boxRow - 1][boxCol] == '-' &&
                game->grid[boxRow][boxCol - 1] == '|' &&
                game->grid[boxRow][boxCol + 1] == '|')
            {

                game->grid[boxRow][boxCol] = game->currentPlayer;
                boxCompleted = true;
                game->completedBoxes++;

                if (game->currentPlayer == 'A')
                {
                    game->scoreA++;
                }
                else
                {
                    game->scoreB++;
                }
            }
        }

        if (midRow < BOX_ROWS)
        {
            int boxRow = midRow * 2 + 1;
            int boxCol = midCol * 2 + 1;

            if (game->grid[boxRow + 1][boxCol] == '-' &&
                game->grid[boxRow][boxCol - 1] == '|' &&
                game->grid[boxRow][boxCol + 1] == '|')
            {

                game->grid[boxRow][boxCol] = game->currentPlayer;
                boxCompleted = true;
                game->completedBoxes++;

                if (game->currentPlayer == 'A')
                {
                    game->scoreA++;
                }
                else
                {
                    game->scoreB++;
                }
            }
        }
    }
    else
    {
        int midCol = col1 / 2;
        int midRow = (row1 < row2 ? row1 : row2) / 2;

        if (midCol > 0)
        {
            int boxRow = midRow * 2 + 1;
            int boxCol = (midCol - 1) * 2 + 1;

            if (game->grid[boxRow - 1][boxCol] == '-' &&
                game->grid[boxRow + 1][boxCol] == '-' &&
                game->grid[boxRow][boxCol - 1] == '|')
            {

                game->grid[boxRow][boxCol] = game->currentPlayer;
                boxCompleted = true;
                game->completedBoxes++;

                if (game->currentPlayer == 'A')
                {
                    game->scoreA++;
                }
                else
                {
                    game->scoreB++;
                }
            }
        }

        if (midCol < BOX_COLS)
        {
            int boxRow = midRow * 2 + 1;
            int boxCol = midCol * 2 + 1;

            if (game->grid[boxRow - 1][boxCol] == '-' &&
                game->grid[boxRow + 1][boxCol] == '-' &&
                game->grid[boxRow][boxCol + 1] == '|')
            {

                game->grid[boxRow][boxCol] = game->currentPlayer;
                boxCompleted = true;
                game->completedBoxes++;

                if (game->currentPlayer == 'A')
                {
                    game->scoreA++;
                }
                else
                {
                    game->scoreB++;
                }
            }
        }
    }

    return boxCompleted;
}

void placeLine(State *game, int x1, int y1, int x2, int y2)
{
    int row1 = 2 * x1, col1 = 2 * y1;
    int row2 = 2 * x2, col2 = 2 * y2;

    int midRow = (row1 + row2) / 2;
    int midCol = (col1 + col2) / 2;

    if (row1 == row2)
    {
        game->grid[midRow][midCol] = '-';
    }
    else
    {
        game->grid[midRow][midCol] = '|';
    }

    game->drawnLines++;
}

void displayGrid(State *game)
{
    printf("\nScore: Player A: %d, Player B: %d\n\n", game->scoreA, game->scoreB);

    printf("  ");
    for (int j = 0; j < COLUMNS; j++)
    {
        if (j % 2 == 0)
        {
            printf("%d ", j / 2);
        }
        else
        {
            printf("  ");
        }
    }
    printf("\n");

    for (int i = 0; i < ROWS; i++)
    {
        if (i % 2 == 0)
        {
            printf("%d ", i / 2);
        }
        else
        {
            printf("  ");
        }
        for (int j = 0; j < COLUMNS; j++)
        {
            printf("%c ", game->grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

bool makeBotMove(State *game)
{
    int maxPossibleMoves = TOTAL_LINES;
    int (*validMoves)[4] = malloc(maxPossibleMoves * sizeof(*validMoves));
    int validMoveCount = 0;

    for (int x1 = 0; x1 < DOT_ROWS; x1++)
    {
        for (int y1 = 0; y1 < DOT_COLS; y1++)
        {
            if (y1 < DOT_COLS - 1)
            {
                int x2 = x1;
                int y2 = y1 + 1;
                if (isValidLine(x1, y1, x2, y2, game->grid))
                {
                    validMoves[validMoveCount][0] = x1;
                    validMoves[validMoveCount][1] = y1;
                    validMoves[validMoveCount][2] = x2;
                    validMoves[validMoveCount][3] = y2;
                    validMoveCount++;
                }
            }

            if (x1 < DOT_ROWS - 1)
            {
                int x2 = x1 + 1;
                int y2 = y1;
                if (isValidLine(x1, y1, x2, y2, game->grid))
                {
                    validMoves[validMoveCount][0] = x1;
                    validMoves[validMoveCount][1] = y1;
                    validMoves[validMoveCount][2] = x2;
                    validMoves[validMoveCount][3] = y2;
                    validMoveCount++;
                }
            }
        }
    }

    if (validMoveCount > 0)
    {
        int randomIndex = rand() % validMoveCount;
        int x1 = validMoves[randomIndex][0];
        int y1 = validMoves[randomIndex][1];
        int x2 = validMoves[randomIndex][2];
        int y2 = validMoves[randomIndex][3];

        printf("Bot (%c) chooses: %d %d %d %d\n", game->currentPlayer, x1, y1, x2, y2);

        placeLine(game, x1, y1, x2, y2);

        bool boxCompleted = checkForCompletedBoxes(game, x1, y1, x2, y2);

        if (!boxCompleted)
        {
            switchPlayer(game);
        }

        free(validMoves);
        return true;
    }

    free(validMoves);
    return false;
}

bool makeMediumBotMove(State *game)
{
    int maxPossibleMoves = TOTAL_LINES;
    int (*validMoves)[4] = malloc(maxPossibleMoves * sizeof(*validMoves));
    int validMoveCount = 0;

    for (int x1 = 0; x1 < DOT_ROWS; x1++)
    {
        for (int y1 = 0; y1 < DOT_COLS; y1++)
        {
            if (y1 < DOT_COLS - 1)
            {
                int x2 = x1, y2 = y1 + 1;
                if (isValidLine(x1, y1, x2, y2, game->grid))
                {
                    validMoves[validMoveCount][0] = x1;
                    validMoves[validMoveCount][1] = y1;
                    validMoves[validMoveCount][2] = x2;
                    validMoves[validMoveCount][3] = y2;
                    validMoveCount++;
                }
            }
            if (x1 < DOT_ROWS - 1)
            {
                int x2 = x1 + 1, y2 = y1;
                if (isValidLine(x1, y1, x2, y2, game->grid))
                {
                    validMoves[validMoveCount][0] = x1;
                    validMoves[validMoveCount][1] = y1;
                    validMoves[validMoveCount][2] = x2;
                    validMoves[validMoveCount][3] = y2;
                    validMoveCount++;
                }
            }
        }
    }

    if (validMoveCount == 0)
    {
        free(validMoves);
        return false;
    }

    int *scores = malloc(validMoveCount * sizeof(int));
    int maxScore = 0;
    for (int i = 0; i < validMoveCount; i++)
    {
        State copy;
        copyGrid(game->grid, copy.grid);
        copy.completedBoxes = game->completedBoxes;
        copy.drawnLines = game->drawnLines;

        int x1 = validMoves[i][0], y1 = validMoves[i][1];
        int x2 = validMoves[i][2], y2 = validMoves[i][3];
        placeLine(&copy, x1, y1, x2, y2);
        int before = copy.completedBoxes;
        checkForCompletedBoxes(&copy, x1, y1, x2, y2);
        scores[i] = copy.completedBoxes - before;
        if (scores[i] > maxScore)
            maxScore = scores[i];
    }

    int bestCount = 0;
    int *bestIndices = malloc(validMoveCount * sizeof(int));
    for (int i = 0; i < validMoveCount; i++)
    {
        if (scores[i] == maxScore)
            bestIndices[bestCount++] = i;
    }

    int selected = bestCount > 0 ? bestIndices[rand() % bestCount] : rand() % validMoveCount;
    int *move = validMoves[selected];
    printf("Bot (%c) chooses: %d %d %d %d\n", game->currentPlayer, move[0], move[1], move[2], move[3]);

    placeLine(game, move[0], move[1], move[2], move[3]);
    bool completed = checkForCompletedBoxes(game, move[0], move[1], move[2], move[3]);
    if (!completed)
        switchPlayer(game);

    free(validMoves);
    free(scores);
    free(bestIndices);
    return true;
}

int evaluateBoard(State *game, char player)
{
    if (player == 'A')
    {
        return game->scoreA - game->scoreB;
    }
    else
    {
        return game->scoreB - game->scoreA;
    }
}

int minimax(State *game, int depth, bool isMaximizing, char player, int alpha, int beta)
{
    if (depth == 0 || game->drawnLines >= TOTAL_LINES)
    {
        return evaluateBoard(game, player);
    }

    int maxPossibleMoves = TOTAL_LINES;
    int (*validMoves)[4] = malloc(maxPossibleMoves * sizeof(*validMoves));
    int validMoveCount = 0;

    for (int x1 = 0; x1 < DOT_ROWS; x1++)
    {
        for (int y1 = 0; y1 < DOT_COLS; y1++)
        {
            if (y1 < DOT_COLS - 1)
            {
                int x2 = x1, y2 = y1 + 1;
                if (isValidLine(x1, y1, x2, y2, game->grid))
                {
                    validMoves[validMoveCount][0] = x1;
                    validMoves[validMoveCount][1] = y1;
                    validMoves[validMoveCount][2] = x2;
                    validMoves[validMoveCount][3] = y2;
                    validMoveCount++;
                }
            }
            if (x1 < DOT_ROWS - 1)
            {
                int x2 = x1 + 1, y2 = y1;
                if (isValidLine(x1, y1, x2, y2, game->grid))
                {
                    validMoves[validMoveCount][0] = x1;
                    validMoves[validMoveCount][1] = y1;
                    validMoves[validMoveCount][2] = x2;
                    validMoves[validMoveCount][3] = y2;
                    validMoveCount++;
                }
            }
        }
    }

    if (validMoveCount == 0)
    {
        free(validMoves);
        return evaluateBoard(game, player);
    }

    if (isMaximizing)
    {
        int maxEval = -1000;
        for (int i = 0; i < validMoveCount; i++)
        {
            State copy;
            copy.currentPlayer = game->currentPlayer;
            copy.scoreA = game->scoreA;
            copy.scoreB = game->scoreB;
            copy.completedBoxes = game->completedBoxes;
            copy.drawnLines = game->drawnLines;
            copyGrid(game->grid, copy.grid);

            int x1 = validMoves[i][0], y1 = validMoves[i][1];
            int x2 = validMoves[i][2], y2 = validMoves[i][3];
            placeLine(&copy, x1, y1, x2, y2);

            bool boxCompleted = checkForCompletedBoxes(&copy, x1, y1, x2, y2);

            if (!boxCompleted)
            {
                switchPlayer(&copy);
            }

            int eval = minimax(&copy, depth - 1, boxCompleted ? isMaximizing : !isMaximizing, player, alpha, beta);
            maxEval = (eval > maxEval) ? eval : maxEval;

            alpha = (alpha > maxEval) ? alpha : maxEval;
            if (beta <= alpha)
            {
                break;
            }
        }
        free(validMoves);
        return maxEval;
    }
    else
    {
        int minEval = 1000;
        for (int i = 0; i < validMoveCount; i++)
        {
            State copy;
            copy.currentPlayer = game->currentPlayer;
            copy.scoreA = game->scoreA;
            copy.scoreB = game->scoreB;
            copy.completedBoxes = game->completedBoxes;
            copy.drawnLines = game->drawnLines;
            copyGrid(game->grid, copy.grid);

            int x1 = validMoves[i][0], y1 = validMoves[i][1];
            int x2 = validMoves[i][2], y2 = validMoves[i][3];
            placeLine(&copy, x1, y1, x2, y2);

            bool boxCompleted = checkForCompletedBoxes(&copy, x1, y1, x2, y2);

            if (!boxCompleted)
            {
                switchPlayer(&copy);
            }

            int eval = minimax(&copy, depth - 1, boxCompleted ? isMaximizing : !isMaximizing, player, alpha, beta);
            minEval = (eval < minEval) ? eval : minEval;

            beta = (beta < minEval) ? beta : minEval;
            if (beta <= alpha)
            {
                break;
            }
        }
        free(validMoves);
        return minEval;
    }
}

bool makeHardBotMove(State *game)
{
    int maxPossibleMoves = TOTAL_LINES;
    int (*validMoves)[4] = malloc(maxPossibleMoves * sizeof(*validMoves));
    int validMoveCount = 0;

    for (int x1 = 0; x1 < DOT_ROWS; x1++)
    {
        for (int y1 = 0; y1 < DOT_COLS; y1++)
        {
            if (y1 < DOT_COLS - 1)
            {
                int x2 = x1, y2 = y1 + 1;
                if (isValidLine(x1, y1, x2, y2, game->grid))
                {
                    validMoves[validMoveCount][0] = x1;
                    validMoves[validMoveCount][1] = y1;
                    validMoves[validMoveCount][2] = x2;
                    validMoves[validMoveCount][3] = y2;
                    validMoveCount++;
                }
            }
            if (x1 < DOT_ROWS - 1)
            {
                int x2 = x1 + 1, y2 = y1;
                if (isValidLine(x1, y1, x2, y2, game->grid))
                {
                    validMoves[validMoveCount][0] = x1;
                    validMoves[validMoveCount][1] = y1;
                    validMoves[validMoveCount][2] = x2;
                    validMoves[validMoveCount][3] = y2;
                    validMoveCount++;
                }
            }
        }
    }

    if (validMoveCount == 0)
    {
        free(validMoves);
        return false;
    }

    int *scores = malloc(validMoveCount * sizeof(int));
    int bestScore = -1000;
    int bestMoveIndex = 0;

    int depth = 3;
    if (game->drawnLines > TOTAL_LINES / 2)
    {
        depth = 4;
    }
    if (game->drawnLines > 3 * TOTAL_LINES / 4)
    {
        depth = 5;
    }

    for (int i = 0; i < validMoveCount; i++)
    {
        State copy;
        copy.currentPlayer = game->currentPlayer;
        copy.scoreA = game->scoreA;
        copy.scoreB = game->scoreB;
        copy.completedBoxes = game->completedBoxes;
        copy.drawnLines = game->drawnLines;
        copyGrid(game->grid, copy.grid);

        int x1 = validMoves[i][0], y1 = validMoves[i][1];
        int x2 = validMoves[i][2], y2 = validMoves[i][3];
        placeLine(&copy, x1, y1, x2, y2);

        bool boxCompleted = checkForCompletedBoxes(&copy, x1, y1, x2, y2);

        if (!boxCompleted)
        {
            switchPlayer(&copy);
        }

        scores[i] = minimax(&copy, depth - 1, boxCompleted, game->currentPlayer, -1000, 1000);

        if (scores[i] > bestScore)
        {
            bestScore = scores[i];
            bestMoveIndex = i;
        }
    }

    int x1 = validMoves[bestMoveIndex][0], y1 = validMoves[bestMoveIndex][1];
    int x2 = validMoves[bestMoveIndex][2], y2 = validMoves[bestMoveIndex][3];

    printf("Hard Bot (%c) chooses: %d %d %d %d\n", game->currentPlayer, x1, y1, x2, y2);

    placeLine(game, x1, y1, x2, y2);
    bool completed = checkForCompletedBoxes(game, x1, y1, x2, y2);
    if (!completed)
    {
        switchPlayer(game);
    }

    free(validMoves);
    free(scores);
    return true;
}

int main()
{
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

    if (gameMode == 2)
    {
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

    while (gameRunning)
    {
        displayGrid(&game);

        if (game.botMode && game.currentPlayer == game.botPlayer)
        {
            printf("Bot's turn (Player %c)\n", game.currentPlayer);
            if (game.botDifficulty == 'e')
            {
                makeBotMove(&game);
            }
            else if (game.botDifficulty == 'm')
            {
                makeMediumBotMove(&game);
            }
            else
            {
                makeHardBotMove(&game);
            }
        }
        else
        {
            printf("Player %c's turn\n", game.currentPlayer);
            printf("Enter coordinates (x1 y1 x2 y2): ");
            scanf("%d %d %d %d", &x1, &y1, &x2, &y2);

            if (isValidLine(x1, y1, x2, y2, game.grid))
            {
                placeLine(&game, x1, y1, x2, y2);

                bool boxCompleted = checkForCompletedBoxes(&game, x1, y1, x2, y2);

                if (!boxCompleted)
                {
                    switchPlayer(&game);
                }
            }
            else
            {
                printf("Invalid move. Try again.\n");
            }
        }

        if (isGameOver(&game))
        {
            displayGrid(&game);
            printf("=== GAME OVER ===\n");
            printf("Final Score: Player A: %d, Player B: %d\n", game.scoreA, game.scoreB);

            if (game.scoreA > game.scoreB)
            {
                printf("Player A wins!\n\n");
            }
            else if (game.scoreB > game.scoreA)
            {
                printf("Player B wins!\n\n");
            }
            else
            {
                printf("It's a tie!\n\n");
            }

            gameRunning = false;
        }
    }

    return 0;
}