#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
using namespace std;

class Player {
private:
    Board board;
    Side mySide, oppSide; // 

public:
    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);

    double scoreMove(Move *myMove);
    void manualBoard(char boardData[]); // A size 64 char array for testing purposes

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
