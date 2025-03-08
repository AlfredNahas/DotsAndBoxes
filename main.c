#include <stdio.h>
#define Rows 5
#define Cols 6

void Gridbuilder(char grid[Rows][Cols])
{
    for (int i = 0; i < Rows; i++)
    {
        for (int j = 0; j < Cols; j++)
        {
            grid[i][j] = '.';
        }
    }
}

void printGrid(char grid[Rows][Cols])
{
    printf("   ");
    for (int j = 0; j < Cols; j++)
    {
        printf(" %d ", j);
    }
    printf("\n");

    for (int i = 0; i < Rows; i++)
    {
        printf(" %d ", i);
        for (int j = 0; j < Cols; j++)
        {
            printf(" %c ", grid[i][j]);
        }
        printf("\n\n");
    }
}

int main()
{
    char grid[Rows][Cols];
    Gridbuilder(grid);
    printGrid(grid);
    isValid();
    return 0;
}
