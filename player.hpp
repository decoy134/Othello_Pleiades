#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
using namespace std;

class Player {
private:
    Board board;
    Side mySide, oppSide; // Stores relevant sides for the board 

public:
    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);

    // Function exclusively used for testing the minimax on naive heuristic
    Move *testMinimax();

    double scoreMove(Move *myMove, Board b, Side side);
    void manualBoard(char boardData[]); // A size 64 char array for testing purposes

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
