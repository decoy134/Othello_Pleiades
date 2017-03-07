#include "player.hpp"

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
double Player::scoreMove(Move *myMove, Board b, Side side)
{
    // Default score initialization
    double score = 0.;

    // Create a copy of the board to make the test move
    Board * scorer = b.copy();

    if(myMove != nullptr)
        scorer->doMove(myMove, side);

    // Actual score adjustment
    score += scorer->count(mySide);
    score -= scorer->count(oppSide);

    // Clean-up the copy
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
    //time_t start, now;
    //time(&start);

    board.doMove(opponentsMove, oppSide);

    // right now we are just returning minimax since it is the best algorithm so far

    //if(testingMinimax)
        return testMinimax();

    Move * temp, * bestMove = nullptr;
    bool valid, firstMove = true;
    double bestScore, thisScore;

    // Current implementation of a brute force move checking algorithm
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            // Make a new move and check if it's allowed
            temp = new Move(i, j);
            valid = board.checkMove(temp, mySide);

            if(valid)
            {
                thisScore = scoreMove(temp, board, mySide);

                if(firstMove)
                {
                    bestMove = temp;
                    bestScore = thisScore;
                    firstMove = false;
                }
                else if(thisScore > bestScore)
                {
                    bestScore = thisScore;

                    delete bestMove;
                    bestMove = temp;
                }
            }
            else
                // Not a valid move. Delete it.
                delete temp;
        }
        //time(&now);
        //if(difftime(now, start) >= msLeft)
        //    return nullptr;
    }

    if(bestMove != nullptr)
    {
        board.doMove(bestMove, mySide);
        return bestMove;
    }

    return nullptr;
}

// Manually sets the board
void Player::manualBoard(char boardData[])
{
    board.setBoard(boardData);
}
