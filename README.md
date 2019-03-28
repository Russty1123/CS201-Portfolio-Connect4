# CS201-Portfolio-Connect4
This project is designed to fulfill the portfolio project for the University of Alabama's CS 201 course: 
Spring Semester, 2019, taught by Dr. Anderson.
The option I am fulfilling is the Game - Connect 4 option

To run the game, all that is necessary is for the user to navigate into the 
Connect4 folder and run the makefile followed by the resulting Connect4.exe file

Upon executing the game, the user will be prompted to chose some settings:
- The player mode: the game is played by 2 players, either both human users or human v computer 
    - This also lets you decide if the computer will be going first or second
- The number of rows, followed by the number of columns. The user gets to decide the size of the board

After settings have been decided, the game immediately starts. If a computer goes first, it immediately make it's move.
The first player will place pieces that display as red R characters and the second player will have blue B characters. 
Upon it reaching a player's turn, an ASCII display of a Connect Four board will display to the user.
The user will recieve a prompt to pick a column to place a piece, where the piece will be placed at the lowest unfilled spot in that column.

The game will alternate between players until one of two conditions occur: someone connects four pieces in a row or the board is filled.
When the game ends, if there was a winner, the winner recieves one point on a Player 1 vs Player 2 basis. This is displayed to the screen.
Alongside the score, a printout of the winning board state is also visible on screen at this time. 

After the games completion, the user will be prompted if they want to play again. 
If they select N, then the program will exit and scores will be reset. 
If they select Y, scores will be preserved and the program will run from the settings prompts through the entire game again. 
