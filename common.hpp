#ifndef __COMMON_H__
#define __COMMON_H__

#include<ctime>
#include<vector>
#include<iostream>

enum Side { 
    WHITE, BLACK
};

class Move {
   
public:
    int x, y;
    Move(int x, int y) {
        this->x = x;
        this->y = y;        
    }
    ~Move() {}

    int getX() { return x; }
    int getY() { return y; }

    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }
};

// Structure used to trace up to 
struct Tracer {
    Move * move;
    Side side;
    Tracer * parent;
    double score;

    Tracer(Move * m, Side s, Tracer * p = nullptr) : move(m), side(s), parent(p), score(0) {}

    // Destroy the move contained, but not the parent
    ~Tracer(){
        if(move != nullptr)
            delete move;
    }

    // Retrieves top level move
    Move * trace()
    {
        Tracer * temp = this;

        while(temp->parent != nullptr)
        {
            temp = temp->parent;
        }

        return temp->move;
    }
    
};

#endif
