Caltech CS2 Assignment 9: Othello

Team Name - Pleiades

AI Name - CZ2128_Delta

Members - Max Zhan (just me)

Past Week Contributions:

I am the only team member.

I implemented a minimax algorithm with adjustable parameter N which would theoretically allow
it to search to whatever depth it wanted to via iterative deepening. However, I spent most of
my time struggling with the Heuristic. It seems that my heuristic is not very good despite my
implementation of frontier squares and long hours of testing for corner and edge values. This
ultimately means that higher depth searchs for my AI are not useful because it misjudges the
position of the board and ends up outthinking itself, so I have set N = 3.

A lot of time was spent on making sure that this would be capable of clean memory iterative
deepening. I first add N depths move layers and then compute the heuristic on the N-th layer,
which is then used to find the score of each move's parent based on whether we are minimizing
or maximizing. To do this, I created a structure called Tracer. Tracers contain a Move, a Side,
a double for its score, and a Tracer pointer to its parent. Using Tracers, I can retrieve the
path of a move extremely quickly by following up from the parent, allowing me to rapidly set
the board state. Tracers also allow me to use an iterative loop of a recursive one. I store a
vector of vectors which stores Tracer pointers, and by getting all possible moves as a vector
of Tracers which remember its parent, I can append the vector to whatever depth I want without
inflating the integer size of the top level vector past the depth limit. At the same time, I
can very easily reverse search from the bottom up by comparing parent pointers; when there is
a change, then I can set the evaluated the score for the previous parent node. This allows me
to have just one loop for each depth as well.

Given more time, I would spend most of it addressing the heuristic which is the limiting factor
of my decision tree. Preferably, I would have used a neural network or support vector machine
machine learning approach to set the heuristic parameters, but due to tightness of time, I had
to manually test it instead. I did not implement alpha-beta pruning because, although I had
initially done so, I found an issue with the tracing algorithm through the tree that required
me to rewrite part of it. From there, I did not implement alpha-beta pruning because it was
clear that my heuristic was not good enough to warrant higher depth searches.
