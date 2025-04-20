#ifndef GAME_H
#define GAME_H


#include <stdbool.h>
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

// Function declarations
void copyGrid(char source[ROWS][COLUMNS], char dest[ROWS][COLUMNS]);
void initializeGame(State *game, bool botMode, char botPlayer, char botDifficulty);
bool isValidLine(int x1, int y1, int x2, int y2, char grid[ROWS][COLUMNS]);
void switchPlayer(State *game);
bool isGameOver(State *game);
bool checkForCompletedBoxes(State *game, int x1, int y1, int x2, int y2);
void placeLine(State *game, int x1, int y1, int x2, int y2);
void displayGrid(State *game);


#endif
