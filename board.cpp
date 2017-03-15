#include "board.hpp"

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() {
    taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

bool Board::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

void Board::set(Side side, int x, int y) {
    taken.set(x + 8*y);
    black.set(x + 8*y, side == BLACK);
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}


/*
 * Returns true if the game is finished; false otherwise. The game is finished
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side side) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side side) {
    // Passing is only legal if you have no moves.
    if (m == nullptr) return !hasMoves(side);

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }
    return false;
}

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, Side side) {
    // A nullptr move means pass.
    if (m == nullptr) return;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;

    int X = m->getX();
    int Y = m->getY();
    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
}

/*
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    return black.count();
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}

/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    taken.reset();
    black.reset();
    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            taken.set(i);
            black.set(i);
        } if (data[i] == 'w') {
            taken.set(i);
        }
    }
}

int Board::countAll()
{
    return taken.count();
}

// Does sequence of suggested moves
void Board::doMoves(Tracer * t)
{
     vector<Tracer*> path; // the path taken by the tracer

     // No error checking, moves MUST be correct sequentially
     while(t != nullptr)
     {
         path.push_back(t);
         t = t->parent;
     }

     for(auto it = path.rbegin(); it != path.rend(); it++)
     {
         this->doMove( (*it)->move, (*it)->side );
     }
}

pair<int, int> Board::frontierSquares()
{
    // Order is BLACK and then WHITE
    pair<int, int> score = make_pair(0, 0);

    // North, West, South, East, Center
    int N, W, S, E, C;
    int valN, valW, valS, valE;

    int weight = 1;

    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
        {
            C = i + 8 * j;

            W = i - 1;
            valW = (W >= 0);

            N = j - 1;
            valN = (N >= 0);

            E = i + 1;
            valE = (E < 8);

            S = j + 1;
            valS = (S < 8);

            if(!taken[C])
            {
                if(valN)
                {
                    if(taken[C - 8] && black[C - 8])
                        score.first += weight;
                    else if(taken[C - 8])
                        score.second += weight;
                }
                if(valW)
                {
                    if(taken[C - 1] && black[C - 1])
                        score.first += weight;
                    else if(taken[C - 8])
                        score.second += weight;
                }
                if(valS)
                {
                    if(taken[C + 8] && black[C + 8])
                        score.first += weight;
                    else if(taken[C + 8])
                        score.second += weight;
                }
                if(valE)
                {
                    if(taken[C + 1] && black[C + 1])
                        score.first += weight;
                    else if(taken[C + 1])
                        score.second += weight;
                }
            }
        }

    return score;
}

pair<int, int> Board::scoreEdge()
{
    // Order is BLACK, and then WHITE
    pair<int, int> ret = make_pair(0, 0);
    
    if(countAll() > 50)
        return ret;

    int weight;

    for(int i = 1; i < 7; i++)
    {
        weight = 6;
        if(i == 1 || i == 7)
            weight *= -1;

        // Top row
        if(taken[i])
        {
            if(black[i])
                ret.first += weight;
            else
                ret.second += weight;
        }

        // Left col
        if(taken[i * 8])
        {
            if(black[i * 8])
                ret.first += weight;
            else
                ret.second += weight;
        }

        // Bottom row
        if(taken[i + 56])
        {
            if(black[i + 56])
                ret.first += weight;
            else
                ret.second += weight;
        }

        // Right col
        if(taken[i * 8 + 7])
        {
            if(black[i * 8 + 7])
                ret.first += weight;
            else
                ret.second += weight;
        }
    }

    return ret;
}

pair<int, int> Board::scoreCorner()
{
    // BLACK, and then WHITE
    pair<int, int> ret = make_pair(0, 0);

    int weight = 12;

    if(countAll() > 50)
        return ret;

    // Top left corners
    if(taken[0])
    {
        if(black[0])
            ret.first += weight;
        else
            ret.second += weight;
    }
    if(taken[9])
    {
        if(black[9])
            ret.first -= weight;
        else
            ret.second -= weight;
    }

    // Top right corners
    if(taken[7])
    {
        if(black[7])
            ret.first += weight;
        else
            ret.second += weight;
    }
    if(taken[14])
    {
        if(black[14])
            ret.first -= weight;
        else
            ret.second -= weight;
    }

    // Bottom left corners
    if(taken[56])
    {
        if(black[56])
            ret.first += weight;
        else
            ret.second += weight;
    }
    if(taken[49])
    {
        if(black[49])
            ret.first -= weight;
        else
            ret.second -= weight;
    }

    // Bottom right corners
    if(taken[63])
    {
        if(black[63])
            ret.first += weight;
        else
            ret.second += weight;
    }
    if(taken[54])
    {
        if(black[54])
            ret.first -= weight;
        else
            ret.second -= weight;
    }

    return ret;
}

vector<Tracer*> Board::getPosMoves(Side side, Tracer * parent){
    vector<Tracer*> ret; // vector to return
    Move * temp; // Move holding variable

    for(int i = 0; i < 8; i++)
        for(int j = 0; j < 8; j++)
        {
            temp = new Move(i, j);
            // check if we can do this move
            if(checkMove(temp, side))
                ret.push_back(new Tracer(temp, side, parent));
            else
                delete temp;
        }

    // The only valid move is nullptr
    if(ret.size() == 0)
        ret.push_back(new Tracer(nullptr, side, parent));

    return ret;
}
