#include "game.h"
#include <stdlib.h>
#include <stdio.h>

//Makes a random move for the easy bot, finds all valid moves and select one randomly
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

//Makes a smarter move for the medium bot, prioritizes moves that complete boxes and selects the best one
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
//Evaluates board state for minimax algorithm, returns score based on player to optimize the next move
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

//Minimax algorithm with alpha-beta pruning to find the best move for the hard bot by looking ahead by several moves
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

//Makes the best move for the hard bot using the minimax algorithm to evaluate the best possible move and places it on the grid
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
