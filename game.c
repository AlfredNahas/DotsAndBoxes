#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

