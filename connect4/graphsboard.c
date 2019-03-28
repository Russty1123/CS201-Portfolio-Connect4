/* I'm Russell McKenzie, and this is my Connect 4 Program for UA's CS201. 
 *
 * This program prompts for a number of players (1-2), turn order, and size of
 * a Connect 4 board. It then proceeds to print to the command line a representation
 * of the board, and prompts the user(s) for moves based on the displayed columns.
 * If there is only 1 player, then a computer will make moves based on a Depth
 * First algorithm to determine a value outlined in code and play the move with
 * the highest perceived value. This computer is a strong player, but was not
 * designed to be unbeatable, particularly because it cannot evaluate the move
 * that he unlocks for the other player above the move it is making.
 *
 * Any and all options may be changed mid-session, but during the time that the
 * program is active, it will keep score based on whether the player going first
 * or second won, awarding no score to a draw. This count will reset with the program,
 * and will not track if players decide to switch their turn order. 
 */

// standard c libraries only
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>

void size_prompt(); //Function designed to handle IO regarding board size. 
int play_game(char computer_active); //Function designed to handle turn order, especially if a computer is in play.
int replay_query(); //Function designed to handle IO regarding the player's intend to continue playing
char computer_query(); //Function designed to handle IO regarding if there will be a computer
void printBoard(char *board); //Function used to print the resulting board after every move and at game's end
int takeTurn(char *board, int player, char*); //Function used to handle IO and placement of a human's turn.


//The following functions all handle various parts of the computer's turn, especially in determining what's valuable
int comp_best_move(char *board, int, char*); 
int calc_move(char *board, int, char*, int);
void computer_takeTurn(char *board, int, char*);
int evaluation(int evaluate[], int);

//The following functions are used to figure out if either player has connected 4 and thereby won the game
int checkWin(char *board);
int checkFour(char *board, int, int, int, int);
int horizontalCheck(char *board);
int verticalCheck(char *board);
int diagonalCheck(char *board);

//Essentially every function needs to know how big the board is, and since it is configurable I made this and only this global.
int num_rows = 0;
int num_cols = 0;


//The main function initializes some variables, handles the introduction and exit messages, and calls all the IO/config functions to setup the game before releasing the play_game function
int main(int argc, char *argv[]){
	
	int finished = 0;           //is the game done?
	int one_score = 0;			//Track the score for player 1 
	int two_score = 0;			//Track the score for player 2
	int result = 0;				//Used to call play game and hold the result of the game for determining score
	char computer_active = '0'; //Flag for is the computer is going to be active, as well as where it will be in the turn order
	
	printf("Welcome to Connect 4! \nYou can play as two humans, or vs a computer going first or second. \nYou may resize the board before each game as small as 4x4 and as large as you wish, though sizes larger than 40 may have visual errors. \nScore will be kept track throughout the session by a Player 1 or 2 basis, where Player 1 went first.\n"); 
	
	while(finished != 1){ 							//Loop games until the player says they are done
		computer_active = computer_query();         //check if they want a computer
		size_prompt();								//check what board size they want
		result = play_game(computer_active);		//play the game
		if(result == 1) one_score++;				//check for score
		if(result == 2) two_score++;
		
		printf("Current scores: \nPlayer 1 - %d\nPlayer 2 - %d\n",one_score,two_score);
		printf("Play again? Y/N \n");
		finished = replay_query();					//Do they want to play again?
	}
	printf("Thanks for playing, shutting down.\n");	//Exit message
	return 0;										//End main, ending the program
}


//This function handles the IO for picking the presence of AI
char computer_query(){
	char computer_mode = '3';
	printf("Please pick a mode:\nPlayer 1 V Player 2\n0: Human V Human\n1: Human V Computer\n2: Computer V Human\n");
	while( !(computer_mode == '0' || computer_mode == '1' || computer_mode == '2') ){  
		computer_mode = getchar();
		if( !(computer_mode == '0' || computer_mode == '1' || computer_mode == '2' || computer_mode == '\n') ){
			printf("Please enter mode: 0,1,2\n");
		}
	}
	return computer_mode; //return the flag to main so main can tell play game what's up
}

//This function handles the IO for deciding to be done with the game or to continue. 
int replay_query(){
	char play_again='x';
	char buffer[50];
	int finished = 0;
	while( !(play_again == 'Y' || play_again == 'y' || play_again == 'n' || play_again == 'N') ){
		play_again = getchar();
		if( !(play_again == 'Y' || play_again == 'y' || play_again == 'n' || play_again == 'N' || play_again == '\n') ){
			printf("Please enter Y to continue or N to quit\n");
		}
	}
	if(play_again == 'n' || play_again == 'N') finished = 1;
	gets(buffer);
return finished; //Return whether they decided to play again or not. 
}


//Begin the game! It needs to know the computer's state in order to handle the turns. 
int play_game(char computer_active){
	
	char *piece_select = "RB";				//This char* is useful because I can use it with the current turn to determine what to put on the board
	char board[num_rows * num_cols];		//I'm using an array of characters to keep track of the pieces and empty spaces in the game.
	memset(board, ' ', num_rows * num_cols);//Gotta allocate that memory.

	int turn, done = 0;  //I need a turn tracker and a flag as to whether or not the game has ended.

	for(turn = 0; turn < num_rows * num_cols && !done; turn++){    	//While the game has not ended and we have not completed filled the board(and drawn) we need to keep taking turns
		printBoard(board);   
		if( (computer_active == '1' && turn%2 == 1) || (computer_active == '2' && turn%2 == 0) ){
			computer_takeTurn(board, turn%2, piece_select);			//If the computer is active and it's turn is now, take the computer's turn
		}else{
			while(!takeTurn(board, turn % 2, piece_select)){		//Else the human needs to take their turn. Note that if there is no computer it always falls into this else.
				printBoard(board);   
				puts("**Column full!**\n");							//This whole !takeTurn thing is so that if the player makes an illegal move I can tell them and reprompt+reprint
			}
		}
		done = checkWin(board);		//if someone has reached a win condition, then stop looping
	} 
   printBoard(board);	//print the winning board so they can examine it as they are re-prompted for new game stuff

   if(turn == num_rows * num_cols && !done){ //this if loop handles who won (or draw) and return values that are used for scoring
      puts("Tie Game!");
	  return 0;
   } else {
		turn--;
		if(piece_select[turn % 2] == 'R'){
			printf("Player %d (\x1b[31m%c\x1b[0m) wins!\n", turn % 2 + 1, piece_select[turn % 2]);
			return 1;
		}
		if(piece_select[turn % 2] == 'B'){
			printf("Player %d (\x1b[34m%c\x1b[0m) wins!\n", turn % 2 + 1, piece_select[turn % 2]);
			return 2;
		}
   }
   return 0;
}

// This function handles the IO for setting the board size. This is the only function that can interact with my 2 global variables
void size_prompt(){
	int correct = 0; 
	char buffer[50]; //useful so that I can get rid of garabage
	num_rows = 0; //reset to 0, so that new input is REQUIRED, and bad input gets looped
	num_cols = 0;
	while(correct != 1){ //loop for correct input
		printf("What number of rows do you want to play with?\n");
		scanf("%1000d",&num_rows);
		if(num_rows < 4){
			printf("Please choose rows equal to or greater than 4\n");
		}
		if(num_rows > 40){
			printf("There may be formatting errors with too large of dimensions\n");
		}
		if (num_rows >= 4){
			correct = 1;
		}
		gets(buffer);
	}
		correct = 0;
	while(correct != 1){
		printf("What number of columns do you want to play with?\n");
		scanf("%1000d",&num_cols);
		if(num_cols < 4){
			printf("Please choose columns equal to or greater than 4\n");
		}
		if(num_cols > 40){
			printf("There may be formatting errors with too large of dimensions\n");
		}
		if (num_cols >= 4){
			correct = 1;
		}
		gets(buffer);
	}
	return; //No return value, this whole thing is just adjusting globals
}

//This is my function for printing out the characters into the board in an orderly fashion
//It is called between every move so that the player can see every board change
//This function introduces the math of num_cols*row+col, that's essentially how I get to that row and column in the board[] array
//Most of this function is just printf statements to make my board look "nice" (or, at least consistent)
void printBoard(char *board){
	int row, col; //counters named to represent where on the board the loop is

	puts("\n    ****Connect Four****\n");
	for(row = 0; row < num_rows; row++){
		printf("\n");
		for(col = 0; col < num_cols; col++){
			if(board[num_cols * row + col] == 'R'){
				printf("|\x1b[31m%c\x1b[0m ",  board[num_cols * row + col]); //\x1b[??m  is just a way to print in color, where ?? is an integer that determines the color
			}
			else{
				if(board[num_cols * row + col] == 'B'){
					printf("|\x1b[34m%c\x1b[0m ",  board[num_cols * row + col]);
				}else{ 
				printf("|%c ",  board[num_cols * row + col]);
				}
			}
		}
		puts("|");
		for(col = 0; col < num_cols; col++){
			printf("---");
		}
		printf("-");
	}
	printf("\n");
	for(col = 1; col < num_cols+1; col++){
		printf(" %02d",col);  //This 02 helps a lot I can space around every board column being 2 wide, even if I'm only printing a single digit value
	}
	printf("\n");
}

//This function handles the io for human turns, as well as turns the input of a column into a real space on the board
int takeTurn(char *board, int player, char *piece_select){
	int row, col = 0;
	printf("Player %d (%c):\nEnter column number to place your piece: ", player + 1, piece_select[player]);

	while(1){ 
		if(1 != scanf("%10000d", &col) || col < 1 || col > num_cols ){
			while(getchar() != '\n');
			puts("Number out of bounds! Try again.");
		} else { 
			break;
		}
	}
	col--;
//After we got a column from them, we need to turn it into a real space in my board[]
	for(row = num_rows - 1; row >= 0; row--){ 
		if(board[num_cols * row + col] == ' '){ //"find the lowest row"
			board[num_cols * row + col] = piece_select[player]; //This is the same "position calculation" math that I've done before and will do again
			return 1; //good return
		}
	}
	return 0; //bad return
}

//This function is made for convenience, I only have to call this function in main and it checks all the possible win conditions. If any of them are true, return a flag that ends the game
int checkWin(char *board){
	return (horizontalCheck(board) || verticalCheck(board) || diagonalCheck(board));
}

// This function is real simple, any of my win checkers will pass this function 4 positions, so this just checks to see if they are all the same, and non-empty
int checkFour(char *board, int a, int b, int c, int d){
	return (board[a] == board[b] && board[b] == board[c] && board[c] == board[d] && board[a] != ' ');
}

// This function checks all possible positions on the board that can have 4 in a row horizontally, and passes them to checkFour to see if they are a win
int horizontalCheck(char *board){
	int row, col, index;

	for(row = 0; row < num_rows; row++){
		for(col = 0; col < num_cols - 3; col++){ // I only go col > num_cols-3 as I don't want this check to "wrap around" to a new row, which would be an illegal 4 in a row.
			index = num_cols * row + col; //The same position calculation again
			if(checkFour(board,index,index+1,index+2,index+3)){ //This is the first case that I want to check the spots around a particular position. Horizontal is easy, +1,+2,+3 finds the spaces immediately "to the right" in my board[]
				return 1; //Obviously return 1 if we found a win
			}
		}
    }
    return 0; //Return 0 if the game continues
}

//This function is very similar to horizontal check, it just needs to check above the spots above rather than below
int verticalCheck(char *board){
	int row, col, index;

	for(row = 0; row < num_rows - 3; row++){ //rather than stopping based on row wrap around, now I want to stop before I start reading junk that exists "above" the board
		for(col = 0; col < num_cols; col++){
			index = num_cols * row + col; //calculate position
			if(checkFour(board,index,index+num_cols,index+2*num_cols,index+3*num_cols)){ //It turns out that the item immediately above a spot is that same spot's index + the number of columns
				return 1; //1 if win
			}
		}
	}
   return 0; //0 if continue
}

//The most complex of win checking, it needs to do the diagonals, which presents a unique challenge as far as preventing "wrap around reading"
int diagonalCheck(char *board){
	int row, col, index = 0;
	int DIAG_RGT = num_cols+1, DIAG_LFT = num_cols-1;

	for(row = 0; row < num_rows - 3; row++){ //num_rows - 3 continues to be important so we don't go above the board[]	
		for(col = 0; col < num_cols; col++){
			index = num_cols * row + col;
//This if is hard. when checking for diagonal lefts, it is important that we don't look too close to the left wall (col >= 3), and vice versa, diagonal rights should stay away from the right wall (col <= num_cols-3)
			if( (col >= 3 && checkFour(board,index,index+DIAG_LFT,index+DIAG_LFT*2,index+DIAG_LFT*3) )|| (col <= num_cols-3 && checkFour(board,index,index+DIAG_RGT,index+DIAG_RGT*2,index+DIAG_RGT*3)) ){
				return 1; //1 if win
			}
		}
	}
	return 0; //0 if continue
}
//With this we are done with the basic connect 4. everything following is about making a computer play and play smart


//This is the function that gets called from main when it is the computer's turn. It just calls comp_best_move with the board[]. This is what finally inserts the computer's best move into board[].
void computer_takeTurn(char *board, int player, char *piece_select){
	printf("~~Computer's Turn~~\n"); //Notify the player that the computer is thinking
	board[comp_best_move(board, player, piece_select)] = piece_select[player];
} //no return, all it had to do was edit the board

//This function marks the start of my depth first search. Essentially this serve the purpose of calling subsequent functions, 1 for every column, that will determine where the computer will move
int comp_best_move (char *board, int player, char *piece_select){
	int x = 0;
	int y = -1; //this -1 initialization helps so that if a column is full, subsequent functions get an illegal value and not 0, which is legal in my board[] array but in most cases an illegal connect4 move
	int row = 0;
	int best_score=0;
	int this_score=0;
	int move=num_cols/2;
	
	while(x<num_cols){ //This while loop is how I look at 1 move from every columns
		for(row = 0; row <= num_rows; row++){ //This for loop is how I look at the first open position in each column
			if(board[num_cols * row + x] == ' '){
				y = num_cols * row + x; //when I find the open row in each column, save the position
			}
		}
		//this column's score is calculated by a calc_move based on the y found above  
		this_score = calc_move(board,player,piece_select,y); //note if y=-1, then this will return 0, which is the lowest possible score, so my computer will never make that move
		
		if(this_score > best_score){ //if this columns score is the best one yet, save it in move
			move = y;
			best_score = this_score;
			this_score = 0; //make sure to put variables back in order for the next loop
			y=-1; //-1 is important for column full detection, as talked about above
		}
		x=x+1;
	}
	printf("The computer decided to move in the %d column\n",move%num_cols+1); //After this is all said and done seraching, here is where the computer knows it's move for the first time, so inform the player where the computer is going
	return move;
}


//This function continues my depth first search by looking at the stuff around the position of the potential move passed to it from above
int calc_move(char *board, int player, char *piece_select, int position){
	printf("\x1b[31m\ncalc_move received:%d\x1b[0m",position);
	int score = 0;
	int x;
	int add;
	int evaluate[6];
	
	x = 0;      //setup for horizontal evaluation
	while (x<7){ //I want to create a temporary array of size 6 to represent what's around the computer's potential move
//In order to see what's is around it horizontally (left and right), I want to look at board[]-3 through board+3, so my loop is designed to assign those to an array called evaluate in positions 0 through 6, respectively
//I wanted to work with integers in evaluate, so I needed a switch statement to look at what is in my board and assign an integer to evaluate
	if ( (position+x-3) > num_rows*num_cols-1 || (position+x-3) < 0){ 
		evaluate[x] = -1; 		//this if checks for out of bounds above or below my board[] so we aren't reading garbage
		}else{
			switch (board[position+x-3]){ 
				case 'R':
					evaluate[x] = 1;
					break;
				case 'B':
					evaluate[x] = 2;
					break;
				case ' ':
					evaluate[x] = 0;
					break;
				default:
					printf("horizfail"); //I need a default, so I made an error message that would warn me if something bad was trying to be evaluated. 
					//in theory, the computer should never fall into the default case
					break;
			}
			if((position+x-3)%num_cols == num_cols-1 || (position+x-3)%num_cols == 0){
			evaluate[x] = evaluate[x] + 3; //If a value is close to a wall, we should "mark" it, so I can be sure not to evaluate a wrap around 
			}
		}	
		x=x+1;		
	}
	add = evaluation(evaluate, player); //Now that we created an evaluate array, pass that to the evaluation function, which will return a score
	score = score + add; //start summing up all the scores to get a composite score for horizontal, vertical, and both diagonal evaluations

	
	x = 0;      //setup for vertical evaluation
	while (x<7){//This is extremely similar to horizontal, it just has the *num_cols factor rather than 1,2,3 because we need to go up the entire row rather than left or right
		if ( (position+(x-3)*num_cols) > num_rows*num_cols-1 || (position+(x-3)*num_cols) < 0){
			evaluate[x] = -1;
		}else{
			switch (board[position+(x-3)*num_cols]){
				case 'R':
					evaluate[x] = 1;
					break;
				case 'B':
					evaluate[x] = 2;
					break;
				case ' ':
					evaluate[x] = 0;
					break;
				default:
					printf("vertfail");
					break;
			}
		}		//Note that this is the only evalutation setup that does not worry about wrap around, because it does not move left or right
		x=x+1;		
	}
	add = evaluation(evaluate, player);
	score = score + add;
	
	x = 0;      //setup for lower left to upper right evaluation
	while (x<7){//This copies both elements from above together to move both vertically and horizontally to create my evaluate[]
		if( ( (position+(x-3)+(x-3)*num_cols) ) > num_rows*num_cols-1 || (position+(x-3)+(x-3)*num_cols) < 0){
			evaluate[x] = -1;
		}else{
			switch (board[position+(x-3)+(x-3)*num_cols]){
				case 'R':
					evaluate[x] = 1;
					break;
				case 'B':
					evaluate[x] = 2;
					break;
				case ' ':
					evaluate[x] = 0;
					break;
				default:
					printf("diagonefail");
					break;
			}
			if( (position+(x-3)+(x-3)*num_cols)%num_cols == num_cols-1 || (position+(x-3)+(x-3)*num_cols)%num_cols == 0){
			evaluate[x] = evaluate[x] + 3;
			}
		}
		x=x+1;		
	}
	add = evaluation(evaluate, player);
	score = score + add;
	
	x = 0;      //setup for lower right to upper left evaluation
	while (x<7){//This simply flips the sign of the "left to right" factor so we get the other diagonal 
		if ( ( (position-(x-3)+(x-3)*num_cols)) > num_rows*num_cols-1 || (position-(x-3)+(x-3)*num_cols) < 0){
			evaluate[x] = -1;
		}else{
			switch (board[position-(x-3)+(x-3)*num_cols]){
				case 'R':
					evaluate[x] = 1;
					break;
				case 'B':
					evaluate[x] = 2;
					break;
				case ' ':
					evaluate[x] = 0;
					break;
				default:
					printf("diagtwofail");
					break;
			}
			if( (position-(x-3)+(x-3)*num_cols)%num_cols == num_cols-1 || (position-(x-3)+(x-3)*num_cols)%num_cols == 0){
			evaluate[x] = evaluate[x] + 3;
			}
		}
		x=x+1;		
	}
	add = evaluation(evaluate, player);
	score = score + add; //After this point, the move has been considered from all directions, and we now have a composite score for this particular move
	
//	printf("\x1b[33mcalc_move returns:%d\x1b[0m",score);  //This was my most useful debugging line, so I'm leaving it in commented out
														  //It allows the user or developer to see what the computer thinks about every move it considers
	return score; //Return the score of the move so that calc_move can decide if this is the best move or not
}

//This function receives one direction of the potential move, and will return how "strong" the move is with an int, where higher is stronger
int evaluation (int evaluate[],int player){
	
	int score = 0;
	if(evaluate[3] == -1){  //if the move itself is invalid, score 0;
		printf("eval failed");
		return score;
	}
	
	int x = 0;
	int y = 7;
	while(x<7){	//This while loop is how I avoid wrap around
		if(evaluate[x] > 2){ //evaluate will be >2 IFF the spot it is representing was a border piece
			evaluate[x] = evaluate[x]-3; //so reset it's value to be consistent with the other values
			if (x>3){ //If it is to the right of the middle value
				y = x;
				x = x + 1;
				while(x<7){
					evaluate[x] = -1; //make everything to the right of the border spot illegal
					x = x + 1;
				}
				x = y;
				y = 0;
			}	
			if (x<3){ //If it is to the left of the middle value 
				y = x;
				x = x - 1;
				while(x > -1){
				evaluate[x] = -1; //make everything to the left of the border spot illegal
				x = x - 1;
				}
				x = y;
				y = 0;
			}
		}
	x = x + 1;
	}
	
	int e0 = evaluate[0];  //I wanted to try to make my logic more readable, so I made some short variable names
	int e1 = evaluate[1];
	int e2 = evaluate[2];
	int e4 = evaluate[4];
	int e5 = evaluate[5];
	int e6 = evaluate[6];
	
	//is the move next to a group of 3 in a row?
	if( ( ( (e0==e1&&e1==e2) || (e1==e2&&e2==e4) || (e2==e4&&e4==e5) ) && e2>0 ) || ( (e4==e5&&e5==e6)&& e4>0 ) ){
		score = score + 1000; //it's better than anything other than winning
		if(e2==(player+4)%3||e4==(player+4)%3){ //If the pieces are the computer's
			score = score + 1000; //it's winning, highest possible score
		}
	}
	//is the move next to a group of 2 in a row? (with growth potential due to 0's)
	if( (e1==e2 && (e1>0) && (e0==0 || e4==0) ) || (e2==e4 && (e2>0) && (e1==0 || e5==0) ) || (e4==e5 && (e4>0) && (e2==0||e6==0) ) ){
		score = score + 100; //not as important as winning, next best thing, color less important
	}
	//does the move make a 2 chain?
	if( (e2>0 && ( (e0==e1 && e0==0) || (e1==e4 && e1==0) || (e4==e5 && e4==0) ) )||(e4>0  && ( (e0==e1 && e0==0) || (e1==e4 && e1==0) || (e4==e5 && e4==0) ) ) ){
		score = score + 10; //not as good as making a 3 chain
	}
	
	x = 0;
	while(x<7){
		if(evaluate[x] != -1){
			score = score + 1; //Placing pieces near other pieces and empties is better than placing pieces against a wall
		}
		x=x+1;
	}	
	return score; //Return the score, this score will be summed with other direction's scores which will return to calc_move and be compared to decide if it is the best move.
}