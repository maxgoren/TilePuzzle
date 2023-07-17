# TilePuzzle
Solving tile puzzles with searching algorithms

Presented are solvers for the sliding tile puzzle. Implemented are Depth Limited Depth First Search, Breadth First Search, 
Iterative Deepening Depth First Search, Priority First Search, and A* search.

The A* version is the only one that can solve 15-puzzles in a reasonable amount of time, thought it still takes roughly a minute. The others are limited to solving
8-puzzles, though they do so relatively quickly.

Priority first search was implemented using both a quaternary tree and an LCRS tree.
All other examples use Left Child Right Sibling trees. LCRS trees are a memory efficient alternative
to quaternary trees, whos structure lends themselves nicely to the this class of problem.

All code is distributed under the MIT License (c) 2023 Max Goren
