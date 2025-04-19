#ifndef BOT_H
#define BOT_H

#include "game.h"

bool makeBotMove(State* game);
bool makeMediumBotMove(State* game);
void makeHardBotMove(State* game);
int evaluateBoard(State* game, char player);
int minimax(State* game, int depth, bool isMaximizing, char player, int alpha, int beta);

#endif
