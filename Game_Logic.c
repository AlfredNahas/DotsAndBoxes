#include <stdio.h>
#include <stdbool.h>
#define ROWS 9
#define COLUMNS 12
#define DOT_ROWS ((ROWS / 2) + 1)  
#define DOT_COLS ((COLUMNS / 2) + 1)  
#define BOX_ROWS (DOT_ROWS - 1)  
#define BOX_COLS (DOT_COLS - 1)  


typedef struct
{
    char grid[ROWS][COLUMNS];
    char currentPlayer;
    int completedBoxes;
    int scoreA;
    int scoreB;

} State;

bool isValidLine(int x1, int y1, int x2, int y2, char grid[ROWS][COLUMNS])
{
    int row1 = 2 * x1, col1 = 2 * y1;
    int row2 = 2 * x2, col2 = 2 * y2;

    if (row1 < 0 || row1 >= ROWS || col1 < 0 || col1 >= COLUMNS ||
        row2 < 0 || row2 >= ROWS || col2 < 0 || col2 >= COLUMNS)
    {

        return false;
    }
    if ((row1 == row2 && col2 == col1 + 2) ||
        (row1 == row2 && col2 == col1 - 2) ||
        (col1 == col2 && row2 == row1 + 2) ||
        (col1 == col2 && row2 == row1 - 2))
    {

        if (row1 != row2 && col1 != col2)
        {
            return false;
        }
        int midRow = (row1 + row2) / 2;
        int midCol = (col1 + col2) / 2;

        if (grid[midRow][midCol] == ' ')
        {
            return true;
        }
    }
    return false;
}

void switchPlayer(State *game) {
    if (game->currentPlayer == 'A')
    {
        game->currentPlayer = 'B';
    }
    else
    {
        game->currentPlayer = 'A';
    }
}

bool isGameOver(State *game) {
    return game->completedBoxes >= BOX_ROWS * BOX_COLS;
}

bool checkForCompletedBoxes(State *game, int x1, int y1, int x2, int y2) {
    int row1 = 2 * x1, col1 = 2 * y1;
    int row2 = 2 * x2, col2 = 2 * y2;
    bool boxCompleted = false;
    
    bool isHorizontal = (row1 == row2);
    
    if (isHorizontal) {
        int midRow = row1 / 2;
        int midCol = (col1 < col2 ? col1 : col2) / 2;
        
        if (midRow > 0) {
            int boxRow = (midRow - 1) * 2 + 1;
            int boxCol = midCol * 2 + 1;
            
            if (game->grid[boxRow-1][boxCol] == '-' &&  
                game->grid[boxRow][boxCol-1] == '|' &&  
                game->grid[boxRow][boxCol+1] == '|') {  
                
                game->grid[boxRow][boxCol] = game->currentPlayer;
                boxCompleted = true;
                game->completedBoxes++;
                
                if (game->currentPlayer == 'A') {
                    game->scoreA++;
                } else {
                    game->scoreB++;
                }
            }
        }
        
        if (midRow < ROWS) {
            int boxRow = midRow * 2 + 1;
            int boxCol = midCol * 2 + 1;
            
            if (game->grid[boxRow+1][boxCol] == '-' &&  
                game->grid[boxRow][boxCol-1] == '|' &&  
                game->grid[boxRow][boxCol+1] == '|') {  
                
                game->grid[boxRow][boxCol] = game->currentPlayer;
                boxCompleted = true;
                game->completedBoxes++;
                
                if (game->currentPlayer == 'A') {
                    game->scoreA++;
                } else {
                    game->scoreB++;
                }
            }
        }
    } else {
        int midCol = col1 / 2;
        int midRow = (row1 < row2 ? row1 : row2) / 2;
        
        if (midCol > 0) {
            int boxRow = midRow * 2 + 1;
            int boxCol = (midCol - 1) * 2 + 1;
            
            if (game->grid[boxRow-1][boxCol] == '-' &&  
                game->grid[boxRow+1][boxCol] == '-' &&  
                game->grid[boxRow][boxCol-1] == '|') {  
                
                game->grid[boxRow][boxCol] = game->currentPlayer;
                boxCompleted = true;
                game->completedBoxes++;
                
                if (game->currentPlayer == 'A') {
                    game->scoreA++;
                } else {
                    game->scoreB++;
                }
            }
        }
        
        if (midCol < BOX_COLS) {
            int boxRow = midRow * 2 + 1;
            int boxCol = midCol * 2 + 1;
            
            if (game->grid[boxRow-1][boxCol] == '-' &&  
                game->grid[boxRow+1][boxCol] == '-' &&  
                game->grid[boxRow][boxCol+1] == '|') {  
                
                game->grid[boxRow][boxCol] = game->currentPlayer;
                boxCompleted = true;
                game->completedBoxes++;
                
                if (game->currentPlayer == 'A') {
                    game->scoreA++;
                } else {
                    game->scoreB++;
                }
            }
        }
    }
    
    return boxCompleted;
}