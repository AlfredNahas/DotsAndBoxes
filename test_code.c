#include <assert.h>
#include <stdio.h>
#include "game.h"
#include "bot.h"

//test functions for the game logic and bot moves
void test_isValidLine() {

    char grid[ROWS][COLUMNS];

    
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            grid[i][j] = ' ';  
        }
    }

    printf("Testing valid horizontal line (0,0) to (0,1)...\n");
    assert(isValidLine(0, 0, 0, 1, grid) == true);  
    printf("Test 1 Passed: Valid horizontal line.\n");

    printf("Testing valid vertical line (0,0) to (1,0)...\n");
    assert(isValidLine(0, 0, 1, 0, grid) == true);  
    printf("Test 2 Passed: Valid vertical line.\n");

    
    printf("Testing invalid diagonal line (0,0) to (1,1)...\n");
    assert(isValidLine(0, 0, 1, 1, grid) == false);
    printf("Test 3 Passed: Invalid diagonal line.\n");

    
    printf("Testing line out of bounds (0,0) to (10,0)...\n");
    assert(isValidLine(0, 0, 10, 0, grid) == false);  
    printf("Test 4 Passed: Line out of bounds.\n");

    
    printf("Testing duplicate line (0,0) to (0,0)...\n");
    assert(isValidLine(0, 0, 0, 0, grid) == false); 
    printf("Test 5 Passed: Duplicate line.\n");

    grid[0][1] = 1;
    printf("Testing line already drawn (0,0) to (0,1)...\n");
    assert(isValidLine(0, 0, 0, 1, grid) == false);  
    printf("Test 6 Passed: Line already drawn.\n");
}

void test_makeBotMove() {
    State game;
    initializeGame(&game, true, 'A', 'e');   

    int initialLines = game.drawnLines;     
    makeBotMove(&game);    
    displayGrid(&game);                 
    assert(game.drawnLines == initialLines + 1);   // the easy bot should be expected just to draw a line.
    printf("Easy Bot Move Test Passed!\n");
   
}

void test_makeMediumBotMove() {
    State game;
    initializeGame(&game, true, 'A', 'm');
    placeLine(&game,0,0,0,1);
    placeLine(&game,0,1,1,1);
    placeLine(&game,0,0,1,0);
    int initialScore = game.scoreA;
    makeMediumBotMove(&game);
    displayGrid(&game);
    assert(game.scoreA > initialScore);
    printf("Medium Bot Box Capture Test Passed!\n");// medium bot should be expected to get a point.
}
 

void test_HardBot() {
    State game;
    initializeGame(&game, true, 'A', 'h');  

    while (!isGameOver(&game)) {
        if (game.currentPlayer == 'A') {
            makeHardBotMove(&game);  
        } else if (game.currentPlayer == 'B') {
            makeBotMove(&game);     
        }
    }

    displayGrid(&game);  

    printf("Final Score:\n");
    printf("Hard Bot (A): %d\n", game.scoreA);
    printf("Easy Bot (B): %d\n", game.scoreB);

    assert(game.scoreA > game.scoreB);  // Hard bot should win
    printf("Test Passed: Hard Bot defeated Easy Bot!\n");
}


int main() {
    test_isValidLine();
    test_makeBotMove();
    test_makeMediumBotMove();
    test_HardBot ();
    printf("All Bot Tests Passed!\n");
    return 0;
}
