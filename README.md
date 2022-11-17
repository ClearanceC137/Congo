# Congo
This is a relaxed game version of chess. The project is written in C++ and in the project I use algorithms to try simulate a competitive environment with minimax approach!!
The project is split into % Parts

State of game : Forsyth–Edwards Notation (FEN) is a standard way of encoding chess positions as a string

Part1 - Setting the board
Input : The first line of input is N, the number of FEN strings that must be read in as input. N lines
follow, with each line consisting of a single FEN string.
Output :For each FEN string i, output the location of all pieces on the board, as well as the side to move

Part2 -Move Generation
Input : The first line of input is N, the number of FEN strings that must be read in as input.
Output:For each FEN string i, output the valid move of the piece which is of interest.

Part3 - Execute Moves
Input: The first line of input is N, the number of input positions given as FEN strings. 2N lines follow
consisting of FEN strings and the move to be executed
Output: For each FEN string and move, output two lines. The first line of output should be the resulting
position as a FEN string. The second line should specify whether the game is over.

Part4 - Minimax
4.1 Evaluation function
Input: The first line of input is N, the number of input positions given as FEN strings. N lines follow,
with each line containing a single FEN string.
Output:For each FEN string, output the evaluation of that position according to the above evaluation
function.
4.2 Implements minimax search
Input: The first line of input is N, the number of input positions given as FEN strings. N lines follow,
with each line containing a single FEN string
Output: For each FEN string, output the alpha-beta evaluation of that position for depth = 4 using the
aforementioned evalation function.

Part 5 Alpha beta pruning
5.1 Advanced Evaluation (improving 4.1)
Input :The first line of input is N, the number of input positions given as FEN strings. N lines follow,
with each line containing a single FEN string.
Output: For each FEN string, output the evaluation of that position according to the above evaluation
function.
5.2 Implements alpha beta prunning with minimax search
Input : The first line of input is N, the number of input positions given as FEN strings. N lines follow,
with each line containing a single FEN string.
Output : For each FEN string, output the alpha-beta evaluation of that position for depth = 4 using the
aforementioned evalation function.

