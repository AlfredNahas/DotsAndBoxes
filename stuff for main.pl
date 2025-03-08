stuff for main
    int hLines[ROWS][COLS - 1] = {0};  // Stores horizontal lines
    int vLines[ROWS - 1][COLS] = {0};  // Stores vertical lines
    char boxes[ROWS - 1][COLS - 1];    // Stores completed boxes
    for (int i = 0; i < ROWS - 1; i++) {
        for (int j = 0; j < COLS - 1; j++) {
            boxes[i][j] = ' '; // Initialize empty boxes
        }
    }

    char player = 'A';

    while (1) {  // Infinite loop (can be modified to check for game end)
        printf("\nCurrent Grid:\n");
        printGrid(hLines, vLines, boxes);
        
        placeLine(hLines, vLines, boxes, player);
        
        // Switch player
        player = (player == 'A') ? 'B' : 'A';
    }


this is needed in the main for my part of code to work else it won't