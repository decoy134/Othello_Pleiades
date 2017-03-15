#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include "common.hpp"
using namespace std;

class Board {

private:
    bitset<64> black;
    bitset<64> taken;

    bool occupied(int x, int y);
    bool get(Side side, int x, int y);
    bool onBoard(int x, int y);

public:
    Board();
    ~Board();
    Board *copy();

    void set(Side side, int x, int y);

    bool isDone();
    bool hasMoves(Side side);
    bool checkMove(Move *m, Side side);
    void doMove(Move *m, Side side);
    int count(Side side);
    int countBlack();
    int countWhite();

    void setBoard(char data[]);

    // Returns all the possible moves on the board as a vector
    vector<Tracer*> getPosMoves(Side side, Tracer * parent);

    int countAll();
    pair<int, int> frontierSquares();
    //void undoMoves(Tracer * t);

    pair<int, int> scoreEdge();
    pair<int, int> scoreCorner();

    void doMoves(Tracer * t);
};

#endif
