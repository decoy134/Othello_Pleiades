#ifndef __COMMON_H__
#define __COMMON_H__

#include<ctime>
#include<vector>

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

    Tracer(Move * m, Side s, Tracer * p = nullptr) : move(m), side(s), parent(p) {}

    // Destroy the move contained, but not the parent
    ~Tracer(){delete move;}

    // Retrieves top level move
    Move * trace()
    {
        Tracer * temp = this;

        if(temp->parent == nullptr)
            return temp->move;
        else
            temp = temp->parent;
    }
    
};

#endif
