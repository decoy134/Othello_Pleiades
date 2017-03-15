#include "player.hpp"

using namespace std;
/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
    // Set the side variables for doing moves on the board
    mySide = side;
    if(side == WHITE)
        oppSide = BLACK;
    else
        oppSide = WHITE;
    board = Board();
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

// Returns the score of a game state
double Player::scoreMove(Move * myMove, Board b, Side side)
{
    // Default score initialization
    double score = 0.;

    // Make a copy of the board and do the move on the requested side
    Board * scorer = b.copy();
    scorer->doMove(myMove, side);

    Side other;

    // Actual score adjustment
    pair<int, int> edge_scr = scorer->scoreEdge();
    pair<int, int> corn_scr = scorer->scoreCorner();

    int myEdge, oppEdge, myCorn, oppCorn;
    if(mySide == BLACK)
    {
        myEdge = edge_scr.first;
        myCorn = corn_scr.first;

        oppEdge = edge_scr.second;
        oppCorn = corn_scr.second;

        other = WHITE;
    }
    else
    {
        myEdge = edge_scr.second;
        myCorn = corn_scr.second;

        oppEdge = edge_scr.first;
        oppCorn = corn_scr.first;

        other = BLACK;
    }

    score += (scorer->count(mySide));// + myEdge + myCorn);
    score -= (scorer->count(oppSide));// + oppEdge + oppCorn);

    // Delete the scorer
    delete scorer;

    // Return the score
    return score;
}

// Function for testing the minimax algorithm with naive heuristic.
Move *Player::testMinimax()
{
    std::vector<Move*> decisions;

    Move * temp; // placeholder move variable

    // Loop to compile all valid moves
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            // Compile all allowed moves
            temp = new Move(i, j);
            if(board.checkMove(temp, mySide))
                decisions.push_back(temp);
            else
                delete temp;
        }
    }

    if(decisions.size() == 0)
        return nullptr;

    // Keep track of all the scores
    std::vector<double> scores;

    bool firstMove; // First move we are checking?
    double thisScore, worstScore; // score we are checking

    for(auto it = decisions.begin(); it != decisions.end(); it++)
    {
        Board * predict = board.copy();
        predict->doMove(*it, mySide);

        // Set first move to check
        firstMove = true;

        for(int i = 0; i < 8; i++)
        {
            for(int j = 0; j < 8; j++)
            {
                // Make a new move and check if it's allowed
                temp = new Move(i, j);

                if(predict->checkMove(temp, oppSide))
                {
                    thisScore = scoreMove(temp, *predict, oppSide);
                    if(firstMove)
                    {
                        worstScore = thisScore;
                        firstMove = false;
                    }
                    else if(thisScore < worstScore)
                    {
                        worstScore = thisScore;
                    }
                }
                
                delete temp;
            }
        }

        if(firstMove)
            worstScore = scoreMove(nullptr, *predict, oppSide);
        
        scores.push_back(worstScore);

        delete predict;
    }

    Move * bestMove = decisions[0];
    double bestScore = scores[0];

    for(unsigned int i = 1; i < decisions.size(); i++)
    {
        if(scores[i] > bestScore)
        {
            bestScore = scores[i];
            temp = bestMove;
            bestMove = decisions[i];
            delete temp;
        }
        else
            delete decisions[i];
    }

    board.doMove(bestMove, mySide);
    return bestMove;
}

// Computes the best move from a vector of vectors for Tracers
Move *Player::traceBest(vector< vector<Tracer*> > dec)
{
    // The current parent of the nodes we are examining
    Tracer * parent = nullptr;
    Board * tester = nullptr;
    double score, move_scr; // running score value
    Move * move; // placeholder move variable
    Side side; // placeholder side variable

    bool first_iter = true; // first iteration

    // Start from the lowest depth and head to the top
    for(auto it = dec.rbegin(); it != dec.rend() - 1; it++)
    {
        // iterate over all possible decisions
        for(auto jt = it->begin(); jt != it->end(); jt++)
        {
            move = (*jt)->move;
            side = (*jt)->side;

            // Parent has changed, adjust board accordingly
            if( parent != (*jt)->parent )
            {
                if(parent != nullptr)
                    parent->score = move_scr;
                parent = (*jt)->parent; // set the new parent

                if(tester != nullptr)
                    delete tester;
                tester = board.copy(); // copy the new board
                tester->doMoves(parent); // implement the moves to the board

                move_scr = -100000;
                if(side == mySide)
                    move_scr *= -1;
            }

            if(first_iter)
                // score the board state
                score = scoreMove(move, *tester, side);
            else
                score = (*jt)->score;

            // Trying to maximize score
            if( (side == oppSide) && (score > move_scr) )
            {
                move_scr = score;
            }
            // Trying to minimize score
            else if( (side == mySide) && (score < move_scr) )
            {
                move_scr = score;
            }
        }
        if(parent != nullptr)
        {
            parent->score = move_scr;
            parent = nullptr;
        }
        if(tester != nullptr)
        {
            delete tester;
            tester = nullptr;
        }
        first_iter = false;
    }

    double best_scr = -100000;
    move = nullptr;

    for(auto it = dec[0].begin(); it != dec[0].end(); it++)
    {
        if((*it)->score > best_scr)
        {
            best_scr = (*it)->score;
            move = (*it)->move;
        }
    }

    return move;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    // Set the current time and declare the present time 
    time_t start, now;
    time(&start);

    // Update the board with the opposing player's move
    board.doMove(opponentsMove, oppSide);
    
    // For the purposes of testing the miniMax algorithm
    if(testingMinimax)
        return testMinimax();

    vector< vector<Tracer*> > dec; // decision tree depths
    vector<Tracer*> temp_vec; // temporary vector
    int i; // depth index

    Move * ret = nullptr; // move variable to return

    // Changing loop logic
    Side currSide = oppSide; // Side of the player whose move we are checking
    bool max_min = false; // true means we are maximizing, false means minimizing


    /* --------BEGIN: MINIMAX-------- */

    int N = 4; // Depth we are heading to

    dec.push_back(board.getPosMoves(mySide, nullptr));
   
    // First depth move augmentation 
    if(dec[0][0]->move == nullptr)
    {
        delete dec[0][0];
        return nullptr;
    }

    // Checking the decision tree to a minimum of depth 4
    for(i = 1; i < N; i++)
    {
        // Push back with an empty vector
        dec.push_back(vector<Tracer*>());

        // Iterate over last layer's possible moves
        for(auto it = dec[i - 1].begin(); it != dec[i - 1].end(); it++)
        {
            Board * tester = board.copy(); // copy the board for testing.
            tester->doMoves(*it); // make the board out of the tracer

            // Get the possible moves with the tracer as the parent
            temp_vec = tester->getPosMoves(currSide, *it);
            
            // Fast memory append
            dec[i].reserve( temp_vec.size() );
            dec[i].insert( dec[i].end(), temp_vec.begin(), temp_vec.end() );
            
            delete tester; // free the memory
        }
/*
        // Set the new return move from this depth
        if(ret != nullptr)
            delete ret;
        temp = best_mov->trace();
        ret = new Move(temp->getX(), temp->getY());

        // Check how much time we have left
        time(&now);

        // If we need to make a move now
        if( (msLeft > 0) && difftime(now, start) > (msLeft - 0.2) )
        {
            for(auto it = dec.begin(); it != dec.end(); it++)
            {
                for(auto jt = it->begin(); jt != it->end(); jt++)
                    delete *jt;
            }

            board.doMove(ret, mySide);

            return ret;
        }
*/
        if(max_min)
            currSide = oppSide;
        else
            currSide = mySide;
        max_min = !max_min;
    }

    Move * temp = traceBest(dec);
    ret = new Move(temp->getX(), temp->getY());

    /* ---------END: MINIMAX--------- */

    for(auto it = dec.begin(); it != dec.end(); it++)
    {
        for(auto jt = it->begin(); jt != it->end(); jt++)
            delete *jt;
    }

    board.doMove(ret, mySide);

    return ret;
}

// Manually sets the board
void Player::manualBoard(char boardData[])
{
    board.setBoard(boardData);
}
