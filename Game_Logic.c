#include<stdio.h>
#include<stdbool.h>

#define ROWS 5
#define COLUMNS 6

bool isValidLine(int x1, int y1, int x2, int y2, char grid[ROWS][COLUMNS]) { 
    int row1 = 2 * x1, col1 = 2 * y1;
    int row2 = 2 * x2, col2 = 2 * y2;

    if (row1 < 0 || row1 >= ROWS || col1 < 0 || col1 >= COLUMNS || 
        row2 < 0 || row2 >= ROWS || col2 < 0 || col2 >= COLUMNS) {

     return false; 
    }
    if ((row1 == row2 && col2 == col1 + 2) ||  
        (row1 == row2 && col2 == col1 - 2) ||  
        (col1 == col2 && row2 == row1 + 2) ||  
        (col1 == col2 && row2 == row1 - 2)) {


        if (row1 != row2 && col1 != col2) {
            return false; 
        }
        int midRow = (row1 + row2) / 2;
        int midCol = (col1 + col2) / 2;

        if (grid[midRow][midCol] == ' ') {
            return true;  
        }
    }

    return false;  
}
