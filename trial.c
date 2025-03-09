#include <stdio.h>
#include <stdbool.h>

#define ROWS 9
#define COLUMNS 12
#define DOT_ROWS ((ROWS / 2) + 1)  
#define DOT_COLS ((COLUMNS / 2) + 1)  
#define BOX_ROWS (DOT_ROWS - 1)  
#define BOX_COLS (DOT_COLS - 1)  

typedef struct {
    char grid[ROWS][COLUMNS];
    char currentPlayer;
    int scoreA;
    int scoreB;
    int completedBoxes;
} GameState;

// Initialize the game state
void initializeGame(GameState *game) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            game->grid[i][j] = ' ';
        }
    }
    
    for (int i = 0; i < ROWS; i += 2) {
        for (int j = 0; j < COLUMNS; j += 2) {
            game->grid[i][j] = 'o';
        }
    }
    
    game->currentPlayer = 'A';
    game->scoreA = 0;
    game->scoreB = 0;
    game->completedBoxes = 0;
}

bool isValidLine(int x1, int y1, int x2, int y2, char grid[ROWS][COLUMNS]) { 
    if (x1 < 0 || x1 >= DOT_ROWS || y1 < 0 || y1 >= DOT_COLS || 
        x2 < 0 || x2 >= DOT_ROWS || y2 < 0 || y2 >= DOT_COLS) {
        return false; 
    }

    int row1 = 2 * x1, col1 = 2 * y1;
    int row2 = 2 * x2, col2 = 2 * y2;

    if (!((row1 == row2 && abs(col1 - col2) == 2) ||  
          (col1 == col2 && abs(row1 - row2) == 2))) {
        return false;
    }

    int midRow = (row1 + row2) / 2;
    int midCol = (col1 + col2) / 2;

    return (grid[midRow][midCol] == ' ');
}

// Draw a line on the grid
void drawLine(GameState *game, int x1, int y1, int x2, int y2) {
    int row1 = 2 * x1, col1 = 2 * y1;
    int row2 = 2 * x2, col2 = 2 * y2;

    int midRow = (row1 + row2) / 2;
    int midCol = (col1 + col2) / 2;

    if (row1 == row2) {
        game->grid[midRow][midCol] = '-';
    } else {
        game->grid[midRow][midCol] = '|';
    }
}

// Switch the current player
void switchPlayer(GameState *game) {
    game->currentPlayer = (game->currentPlayer == 'A') ? 'B' : 'A';
}

// Check if the game is over
bool isGameOver(GameState *game) {
    return game->completedBoxes >= BOX_ROWS * BOX_COLS;
}

// Check if a move completes any boxes and update the game state
bool checkForCompletedBoxes(GameState *game, int x1, int y1, int x2, int y2) {
    int row1 = 2 * x1, col1 = 2 * y1;
    int row2 = 2 * x2, col2 = 2 * y2;
    bool boxCompleted = false;

    int midRow = (row1 + row2) / 2;
    int midCol = (col1 + col2) / 2;

    for (int r = -1; r <= 1; r += 2) {
        int checkRow = midRow + r;
        if (checkRow >= 1 && checkRow < ROWS - 1) {
            if (game->grid[checkRow - 1][midCol] == '-' &&
                game->grid[checkRow + 1][midCol] == '-' &&
                game->grid[checkRow][midCol - 1] == '|' &&
                game->grid[checkRow][midCol + 1] == '|') {

                game->grid[checkRow][midCol] = game->currentPlayer;
                game->completedBoxes++;

                if (game->currentPlayer == 'A') {
                    game->scoreA++;
                } else {
                    game->scoreB++;
                }
                boxCompleted = true;
            }
        }
    }

    return boxCompleted;
}

// Display the current grid
void displayGrid(GameState *game) {
    printf("\nScore: Player A: %d, Player B: %d\n\n", game->scoreA, game->scoreB);

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            printf("%c", game->grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    GameState game;
    initializeGame(&game);

    int x1, y1, x2, y2;
    bool gameRunning = true;

    printf("=== DOTS AND BOXES ===\n");

    while (gameRunning) {
        displayGrid(&game);

        printf("Player %c's turn\n", game.currentPlayer);
        printf("Enter coordinates (x1 y1 x2 y2): ");

        if (scanf("%d %d %d %d", &x1, &y1, &x2, &y2) != 4) {
            printf("Invalid input. Please enter four integers.\n");
            while (getchar() != '\n');  // Clear input buffer
            continue;
        }

        if (isValidLine(x1, y1, x2, y2, game.grid)) {
            drawLine(&game, x1, y1, x2, y2);

            if (!checkForCompletedBoxes(&game, x1, y1, x2, y2)) {
                switchPlayer(&game);
            }

            if (isGameOver(&game)) {
                displayGrid(&game);
                printf("=== GAME OVER ===\n");
                printf("Final Score: Player A: %d, Player B: %d\n", game.scoreA, game.scoreB);

                if (game.scoreA > game.scoreB) {
                    printf("Player A wins!\n");
                } else if (game.scoreB > game.scoreA) {
                    printf("Player B wins!\n");
                } else {
                    printf("It's a tie!\n");
                }

                gameRunning = false;
            }
        } else {
            printf("Invalid move. Try again.\n");
        }
    }

    return 0;
}
