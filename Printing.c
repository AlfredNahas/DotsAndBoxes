#include <stdio.h>
#define ROWS 4
#define COLS 5

void displayGrid(State *game) {
    printf("\nScore: Player A: %d, Player B: %d\n\n", game->scoreA, game->scoreB);

    printf("  ");
    for (int j = 0; j < COLUMNS; j++) {
        if (j % 2 == 0) {
            printf("%d ", j / 2);
        } else {
            printf("  ");
        }
    }
    printf("\n");

    for (int i = 0; i < ROWS; i++) {
        if (i % 2 == 0) {
            printf("%d ", i / 2);
        } else {
            printf("  ");
        }
        for (int j = 0; j < COLUMNS; j++) {
            printf("%c ", game->grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
void checkBoxes(int hLines[ROWS][COLS - 1], int vLines[ROWS - 1][COLS], char boxes[ROWS - 1][COLS - 1], char player)
{
    for (int i = 0; i < ROWS - 1; i++)
    {
        for (int j = 0; j < COLS - 1; j++)
        {
            if (hLines[i][j] && hLines[i + 1][j] && vLines[i][j] && vLines[i][j + 1] && boxes[i][j] == ' ')
            {
                boxes[i][j] = player;
            }
        }
    }
}
void placeLine(int hLines[ROWS][COLS - 1], int vLines[ROWS - 1][COLS], char boxes[ROWS - 1][COLS - 1], char player)
{
    int type, row, col;
    printf("Player %c, enter line type (1 for horizontal, 2 for vertical): ", player);
    scanf("%d", &type);

    if (type == 1)
    {
        printf("Enter row (0-%d) and column (0-%d): ", ROWS - 1, COLS - 2);
        scanf("%d %d", &row, &col);
        if (row >= 0 && row < ROWS && col >= 0 && col < COLS - 1 && !hLines[row][col])
        {
            hLines[row][col] = 1;
        }
        else
        {
            printf("Invalid move! Try again.\n");
            placeLine(hLines, vLines, boxes, player);
        }
    }
    else if (type == 2)
    {
        printf("Enter row (0-%d) and column (0-%d): ", ROWS - 2, COLS - 1);
        scanf("%d %d", &row, &col);
        if (row >= 0 && row < ROWS - 1 && col >= 0 && col < COLS && !vLines[row][col])
        {
            vLines[row][col] = 1;
        }
        else
        {
            printf("Invalid move! Try again.\n");
            placeLine(hLines, vLines, boxes, player);
        }
    }
    else
    {
        printf("Invalid choice! Try again.\n");
        placeLine(hLines, vLines, boxes, player);
    }
    checkBoxes(hLines, vLines, boxes, player);
}