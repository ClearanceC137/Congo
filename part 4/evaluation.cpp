#include<iostream>
#include<vector>
#include <ctype.h>
#include <vector>
#include<bits/stdc++.h>
using namespace std;

class square{
	public:
		string color;             //black , white
		char type,column;                //lion , giraffe etc
		int rank,col;
		int value;
		square(string color,char type , int rank , char column,int col,int value){
			this->type = type;                //lion , giraffe etc
			this->rank = rank;
			this->column = column;
			this->color = color;
			this->col = col;
			this->value = value;			
		}
	
};


class board{
	public:
		
		string fen ,number_of_moves,side_to_play;
		string grid[7][7] ,current_state[7];		
		vector<vector<square>> positions;
		board(string fen){
			this->fen = fen;
			this->process_string();
			this->set_positions();
			this->set_grid();

		}
		void process_string(){
				/* processess string into separate ranks + side to play and number of moves  */				
			string str = fen;
		    string word = "";
		    vector<string> line;		     
		    for (auto x : str)                           // split by space
		    {			
		        if (x == ' ')
		        {
		            line.push_back(word);
		            word = "";
		        }
		        else {word = word + x;}		       
		    }
		    line.push_back(word);
			
			word = "";
			str = line[0];
		    vector<string> rank;
		    for (auto x : str)              		    // split by forward slash to obtain rank
		    {
		        if (x == '/')
		        {
		            rank.push_back(word);
		            word = "";
		        }
		        else {word = word + x;}		       
		    }
		    rank.push_back(word);	
			int size = line.size();
			this->side_to_play = line[size-2];
			this->number_of_moves = line[size-1] ; 
			for(int i = 0 ; i < 7 ; i++){
				this->current_state[i] = rank[i];
			}							
		}		
		void set_EFN(string fen){
			this->fen = fen;
			this->process_string();
			this->set_positions();         //reset current state of the game
			this->set_grid();
			
		}
		/*        sets position for a specific type     */		
		void set_square(char type,int value){
			char col[] = {'a','b','c','d','e','f','g'};
			vector<square>p;
			for(int i = 0 ; i < 7 ; i++){
					string curr = current_state[i];
					int strSize  = curr.size() , index = 0;
					for(int j = 0 ; j < strSize ; j++){
						if(isdigit(curr[j])){index+=(int)curr[j]-'0';}
						else{index++;}
						if(toupper(type) == curr[j]){
							square* block = new square("white",type, 7-i , col[index-1],index-1,value);
							p.push_back(*block);
						}
						if(type == curr[j]){
							square* block = new square("black",type, 7-i , col[index-1],index-1,value);
							p.push_back(*block);
						}						
					}	
			}
			positions.push_back(p);
			
		}
		/* sets all postions for each type */
		void set_positions(){
			char type[] = {'p','s','g','l','z'};
			int value[] = {100,350,400,10000,300};
			for(int i = 0 ; i < 5 ; i++){
				set_square(type[i] ,value[i]);	
			}	
		}
		
		/*       set the 7x7 grid to allow moves         */
		void set_grid(){
			for(int i = 0 ; i < 5 ; i++){
				vector<square> state =positions[i];
				for(square s: state){
					if(s.color == "white"){	grid[7-s.rank][s.col] = toupper(s.type);}
					else{grid[7-s.rank][s.col] = s.type;}					
				}
			}
		}
		/*   evaluation function      */
		void evaluation(){
			
			if(positions[0].size() == 0 && positions[1].size() == 0 && positions[2].size() == 0 && positions[3].size() == 2 && positions[4].size() == 0){
				// only 2 lions left on the board
				cout<<0;
			}
			else if(positions[3].size() == 1){
				
				if(positions[3][0].color =="white" && side_to_play == "b" ){cout<<-positions[3][0].value;}
				else if(positions[3][0].color =="white" && side_to_play == "w" ){cout<<positions[3][0].value;}
				else if(side_to_play == "b"){cout<<positions[3][0].value;}
				else{
					cout<<-positions[3][0].value;	
				}
			}else{
				/* set value of lions to zero since they are not considered at this point  */
				positions[3][0].value = 0;
				positions[3][1].value = 0;
				int blackvalue = 0 ,whitevalue = 0,score = 0;
				for(int i = 0 ; i < 5 ; i++){
					for(square sq : positions[i]){
						if(sq.color =="white"){
							whitevalue+=sq.value;
						}else{
							blackvalue+=sq.value;
						}
					}
				}
				score = whitevalue - blackvalue;
				if(side_to_play == "w"){
					cout<<score;
				}else{
					cout<<-score;					
				}
			}
		}
		/*         -----------------------------------------------------------------------     */
		
		/* abstract methods for a specified piece on board  */
		virtual vector<string> lion() = 0 , zebra() = 0 ,giraffe() = 0 ,  pawn() = 0,superpawn() = 0;

};



string getString(char x){string s(1, x);return s;}

bool isvalid(int row , int col , int lowrow , int upperrow ,int lowcol , int uppercol){	
	if( row >= lowrow &&  row<= upperrow && col >= lowcol &&  col<= uppercol ){return true;}
	return false;
}
bool isteam(char type1 , char type2 ){
	if (isupper(type1) && isupper(type2)){return true;}
	if (!isupper(type1) && !isupper(type2)){return true;}	
	return false;
}

/*   ---------------- subclass for generating moves for every type of piece on board   ---------------*/
class movepiece : public board{
	public:
		string column[7] = {"a","b","c","d","e","f","g"};
		movepiece(string text):board(text){
			
		}
		vector<string> lion(){
			vector<string>moves;
			int row , col;
			for(square s : positions[3]){
				/* lion position  */
				if(getString(s.color[0]) ==side_to_play ){
					row = 7-s.rank;
					col =  s.col;
				}
			}

			if(isvalid(row+1,col,4,6,2,4) || isvalid(row+1,col,0,2,2,4) ){           //down

				if(grid[row+1][col]==""){moves.push_back(column[col]+to_string(7-(row+1)));}
				else{
					if(!isteam(grid[row][col][0] , grid[row+1][col][0] )){moves.push_back(column[col]+to_string(7-(row+1)));}
				}	
			}if(isvalid(row -1, col , 4 , 6 ,2 , 4) || isvalid(row -1, col , 0 , 2 ,2 , 4) ){       //up

				if(grid[row-1][col]==""){moves.push_back(column[col]+to_string(7-(row-1)));}
				else{
					if(!isteam(grid[row][col][0] , grid[row-1][col][0] )){moves.push_back(column[col]+to_string(7-(row-1)));}
				}				
			}			
			if(isvalid(row , col+1 , 4 , 6 ,2 , 4) || isvalid(row , col+1 , 0 , 2 ,2 , 4)){       //right

				if(grid[row][col+1]==""){moves.push_back(column[col+1]+to_string(7-row));}
				else{
					if(!isteam(grid[row][col][0] , grid[row][col+1][0])){moves.push_back(column[col+1]+to_string(7-row));}
				}				
			}	
			if(isvalid(row , col-1, 4 , 6 ,2 , 4) || isvalid(row , col-1, 0 , 2 ,2 , 4)){       // left
			
				if(grid[row][col-1]==""){moves.push_back(column[col-1]+to_string(7-row));}
				else{
					if(!isteam(grid[row][col][0] , grid[row][col-1][0] )){moves.push_back(column[col-1]+to_string(7-row));}
				}				
			}
			/* ----------------     diagonals   -------------------    */
			if(isvalid(row-1 , col-1, 4 , 6 ,2 , 4) || isvalid(row-1 , col-1, 0 , 2 ,2 , 4)){       

				if(grid[row-1][col-1]==""){moves.push_back(column[col-1]+to_string(7-(row-1)));}
				else{
					if(!isteam(grid[row][col][0] , grid[row-1][col-1][0] )){moves.push_back(column[col-1]+to_string(7-(row-1)));}
				}				
			}
			if(isvalid(row-1 , col+1, 4 , 6 ,2 , 4) || isvalid(row-1 , col+1, 0 , 2 ,2 , 4)){       

				if(grid[row-1][col+1]==""){moves.push_back(column[col+1]+to_string(7-(row-1)));}
				else{
					if(!isteam(grid[row][col][0] , grid[row-1][col+1][0] )){moves.push_back(column[col+1]+to_string(7-(row-1)));}
				}				
			}
			if(isvalid(row+1 , col-1, 4 , 6 ,2 , 4) || isvalid(row+1 , col-1, 0 , 2 ,2 , 4)){     

				if(grid[row+1][col-1]==""){moves.push_back(column[col-1]+to_string(7-(row+1)));}
				else{
					if(!isteam(grid[row][col][0] , grid[row+1][col-1][0] )){moves.push_back(column[col-1]+to_string(7-(row+1)));}
				}				
			}
			if(isvalid(row+1 , col+1, 4 , 6 ,2 , 4) || isvalid(row+1 , col+1, 0 , 2 ,2 , 4)){

				if(grid[row+1][col+1]==""){moves.push_back(column[col+1]+to_string(7-(row+1)));}
				else{if(!isteam(grid[row][col][0] , grid[row+1][col+1][0] )){moves.push_back(column[col+1]+to_string(7-(row+1)));}}				
			}
			/*   -------------- capturing ------------     */

			if(grid[2][2]=="l" && grid[4][4]=="L" &&  grid[3][3]==""){
				if(side_to_play =="w" ){moves.push_back(column[2]+to_string(5));}
				else{moves.push_back(column[4]+to_string(3));}
			}
			if(grid[2][4]=="l" && grid[4][2]=="L" &&  grid[3][3]==""){
				if(side_to_play =="w" ){moves.push_back(column[4]+to_string(5));}
				else{moves.push_back(column[2]+to_string(3));}
			}			
			if(side_to_play =="w"){
				for(int i = row-1 ; i > -1 ; i--){
					if( grid[i][col] !=""){
						if(grid[i][col] == "l"){moves.push_back(column[col]+to_string(7-i));}
						break;}					
				}
			}
			else{
				for(int i = row+1 ; i < 7 ; i++){
					if( grid[i][col] !=""){
						if(grid[i][col] == "L"){moves.push_back(column[col]+to_string(7-i));}
						break;}	
				}				
			}				
			return moves;
		}
		vector<string> zebra(){
			int row , col;
			vector<string>moves;
			for(square s : positions[4]){
				/* zebra position  */
				if(getString(s.color[0]) ==side_to_play ){
					row = 7-s.rank;
					col =  s.col;
				}
			}
			/* -------- two squares vertical one square horizontal  -------- */
			if(isvalid(row-2 , col-1, 0 , 6 ,0 , 6)){
				if(grid[row-2][col-1]==""){moves.push_back(column[col-1]+to_string(7-(row-2)));}
				else{if(!isteam(grid[row][col][0] , grid[row-2][col-1][0] )){moves.push_back(column[col-1]+to_string(7-(row-2)));}}					
			}
			if(isvalid(row-2 , col+1, 0 , 6 ,0 , 6)){
				if(grid[row-2][col+1]==""){moves.push_back(column[col+1]+to_string(7-(row-2)));}
				else{if(!isteam(grid[row][col][0] , grid[row-2][col+1][0] )){moves.push_back(column[col+1]+to_string(7-(row-2)));}}					
			}
			if(isvalid(row+2 , col-1, 0 , 6 ,0 , 6)){
				if(grid[row+2][col-1]==""){moves.push_back(column[col-1]+to_string(7-(row+2)));}
				else{if(!isteam(grid[row][col][0] , grid[row+2][col-1][0] )){moves.push_back(column[col-1]+to_string(7-(row+2)));}}					
			}
			if(isvalid(row+2 , col+1, 0 , 6 ,0 , 6)){
				if(grid[row+2][col+1]==""){moves.push_back(column[col+1]+to_string(7-(row+2)));}
				else{if(!isteam(grid[row][col][0] , grid[row+2][col+1][0] )){moves.push_back(column[col+1]+to_string(7-(row+2)));}}					
			}
			/* -------- two squares horizontal one square vertical  -------- */					
			if(isvalid(row-1 , col-2, 0 , 6 ,0 , 6)){
				if(grid[row-1][col-2]==""){moves.push_back(column[col-2]+to_string(7-(row-1)));}
				else{if(!isteam(grid[row][col][0] , grid[row-1][col-2][0] )){moves.push_back(column[col-2]+to_string(7-(row-1)));}}					
			}
			if(isvalid(row+1 , col-2, 0 , 6 ,0 , 6)){
				if(grid[row+1][col-2]==""){moves.push_back(column[col-2]+to_string(7-(row+1)));}
				else{if(!isteam(grid[row][col][0] , grid[row+1][col-2][0] )){moves.push_back(column[col-2]+to_string(7-(row+1)));}}					
			}
			if(isvalid(row-1 , col+2, 0 , 6 ,0 , 6)){
				if(grid[row-1][col+2]==""){moves.push_back(column[col+2]+to_string(7-(row-1)));}
				else{if(!isteam(grid[row][col][0] , grid[row-1][col+2][0] )){moves.push_back(column[col+2]+to_string(7-(row-1)));}}					
			}
			if(isvalid(row+1 , col+2, 0 , 6 ,0 , 6)){
				if(grid[row+1][col+2]==""){moves.push_back(column[col+2]+to_string(7-(row+1)));}
				else{if(!isteam(grid[row][col][0] , grid[row+1][col+2][0] )){moves.push_back(column[col+2]+to_string(7-(row+1)));}}					
			}
			return moves;
		}
		vector<string> giraffe(){
			int row , col;
			vector<string>moves;
			for(square s : positions[2]){
				if(getString(s.color[0]) ==side_to_play ){
					row = 7-s.rank;
					col =  s.col;
				}
			}
			/** down **/
			if(isvalid(row+1,col,0 , 6 ,0 , 6)){           			
				if(grid[row+1][col]==""){moves.push_back(column[col]+to_string(7-(row+1)));}
			}
			if(isvalid(row+2,col,0 , 6 ,0 , 6) ){  			
				if(grid[row+2][col]==""){moves.push_back(column[col]+to_string(7-(row+2)));}
				else{
					if(!isteam(grid[row][col][0] , grid[row+2][col][0] )){moves.push_back(column[col]+to_string(7-(row+2)));}
				}	
			}
			/** up  **/
			if(isvalid(row -1, col ,0 , 6 ,0 , 6)){  
				if(grid[row-1][col]==""){moves.push_back(column[col]+to_string(7-(row-1)));}			
			}
			if(isvalid(row -2, col , 0 , 6 ,0 , 6) ){   
				if(grid[row-2][col]==""){moves.push_back(column[col]+to_string(7-(row-2)));}
				else{
					if(!isteam(grid[row][col][0] , grid[row-2][col][0] )){moves.push_back(column[col]+to_string(7-(row-2)));}
				}				
			}
			/**  right  **/			
			if(isvalid(row , col+1 , 0 , 6 ,0 , 6)){     
				if(grid[row][col+1]==""){moves.push_back(column[col+1]+to_string(7-row));}				
			}
			if(isvalid(row , col+2 , 0 , 6 ,0 , 6)){    
				if(grid[row][col+2]==""){moves.push_back(column[col+2]+to_string(7-row));}
				else{
					if(!isteam(grid[row][col][0] , grid[row][col+2][0] )){moves.push_back(column[col+2]+to_string(7-row));}
				}				
			}
			/**  left  **/	
			if(isvalid(row , col-1, 0 , 6 ,0 , 6)){     
				if(grid[row][col-1]==""){moves.push_back(column[col-1]+to_string(7-row));}					
			}
			if(isvalid(row , col-2,0 , 6 ,0 , 6)){     
				if(grid[row][col-2]==""){moves.push_back(column[col-2]+to_string(7-row));}
				else{
					if(!isteam(grid[row][col][0] , grid[row][col-2][0] )){moves.push_back(column[col-2]+to_string(7-row));}
				}					
			}
			/** ------------------------- diagonals  ----------------------- **/ 
			if(isvalid(row-1 , col-1,0 , 6 ,0 , 6) ){       
				if(grid[row-1][col-1]==""){moves.push_back(column[col-1]+to_string(7-(row-1)));}			
			}
			if(isvalid(row-2 , col-2, 0 , 6 ,0 , 6)){       
				if(grid[row-2][col-2]==""){moves.push_back(column[col-2]+to_string(7-(row-2)));}
				else{
					if(!isteam(grid[row][col][0] , grid[row-2][col-2][0] )){moves.push_back(column[col-2]+to_string(7-(row-2)));}
				}				
			}
			
			if(isvalid(row-1 , col+1,0 , 6 ,0 , 6)){       
				if(grid[row-1][col+1]==""){moves.push_back(column[col+1]+to_string(7-(row-1)));}				
			}
			if(isvalid(row-2 , col+2, 0 , 6 ,0 , 6)){       
				if(grid[row-2][col+2]==""){moves.push_back(column[col+2]+to_string(7-(row-2)));}
				else{
					if(!isteam(grid[row][col][0] , grid[row-2][col+2][0] )){moves.push_back(column[col+2]+to_string(7-(row-2)));}
				}				
			}
			
			if(isvalid(row+1 , col-1,0 , 6 ,0 , 6)){       
				if(grid[row+1][col-1]==""){moves.push_back(column[col-1]+to_string(7-(row+1)));}			
			}
			if(isvalid(row+2 , col-2, 0 , 6 ,0 , 6)){       
				if(grid[row+2][col-2]==""){moves.push_back(column[col-2]+to_string(7-(row+2)));}
				else{
					if(!isteam(grid[row][col][0] , grid[row+2][col-2][0] )){moves.push_back(column[col-2]+to_string(7-(row+2)));}
				}				
			}
			
			if(isvalid(row+1 , col+1,0 , 6 ,0 , 6)){       
				if(grid[row+1][col+1]==""){moves.push_back(column[col+1]+to_string(7-(row+1)));}			
			}
			if(isvalid(row+2 , col+2,0 , 6 ,0 , 6)){       
				if(grid[row+2][col+2]==""){moves.push_back(column[col+2]+to_string(7-(row+2)));}
				else{
					if(!isteam(grid[row][col][0] , grid[row+2][col+2][0] )){moves.push_back(column[col+2]+to_string(7-(row+2)));}
				}				
			}
			return moves;
		}
		vector<string> pawn(){
			vector<string>moves;
			int row , col;
			for(square s : positions[0]){
				if(getString(s.color[0]) ==side_to_play ){
					row = 7-s.rank;
					col =  s.col;
					if(side_to_play=="b"){
						/* ------- forward -------- */
						if(isvalid(row+1,col,0 , 6 ,0 , 6)){           			
							if(grid[row+1][col]==""){moves.push_back(column[col]+to_string(7-(row+1)));}
							else{if(!isteam(grid[row][col][0] , grid[row+1][col][0] )){moves.push_back(column[col]+to_string(7-(row+1)));}}	
						}
						if(isvalid(row+1,col-1,0 , 6 ,0 , 6)){           			
							if(grid[row+1][col-1]==""){moves.push_back(column[col-1]+to_string(7-(row+1)));}
							else{if(!isteam(grid[row][col][0] , grid[row+1][col-1][0] )){moves.push_back(column[col-1]+to_string(7-(row+1)));}}	
						}
						if(isvalid(row+1,col+1,0 , 6 ,0 , 6)){           			
							if(grid[row+1][col+1]==""){moves.push_back(column[col+1]+to_string(7-(row+1)));}
							else{if(!isteam(grid[row][col][0] , grid[row+1][col+1][0] )){moves.push_back(column[col+1]+to_string(7-(row+1)));}}	
						}
						/* ------ other side ---- */
						if(row >= 4 && grid[row-1][col]==""){           			
							moves.push_back(column[col]+to_string(7-(row-1)));
							if(grid[row-2][col]==""){moves.push_back(column[col]+to_string(7-(row-2)));}					
						}				
					}else{
						/* ------- forward -------- */
						if(isvalid(row-1,col,0 , 6 ,0 , 6)){           			
							if(grid[row-1][col]==""){moves.push_back(column[col]+to_string(7-(row-1)));}
							else{if(!isteam(grid[row][col][0] , grid[row-1][col][0] )){moves.push_back(column[col]+to_string(7-(row-1)));}}	
						}
						if(isvalid(row-1,col-1,0 , 6 ,0 , 6)){           			
							if(grid[row-1][col-1]==""){moves.push_back(column[col-1]+to_string(7-(row-1)));}
							else{if(!isteam(grid[row][col][0] , grid[row-1][col-1][0] )){moves.push_back(column[col-1]+to_string(7-(row-1)));}}	
						}
						if(isvalid(row-1,col+1,0 , 6 ,0 , 6)){           			
							if(grid[row-1][col+1]==""){moves.push_back(column[col+1]+to_string(7-(row-1)));}
							else{if(!isteam(grid[row][col][0] , grid[row-1][col+1][0] )){moves.push_back(column[col+1]+to_string(7-(row-1)));}}	
						}
						/* ------ other side ---- */
						if(row <= 2 && grid[row+1][col]==""){           			
							moves.push_back(column[col]+to_string(7-(row+1)));
							if(grid[row+2][col]==""){moves.push_back(column[col]+to_string(7-(row+2)));}					
						}				
					}
				}
			}
			return moves;
		}
		vector<string> superpawn(){
			vector<string>moves;
			int row , col;
			for(square s : positions[1]){
				if(getString(s.color[0]) ==side_to_play ){
					row = 7-s.rank;
					col =  s.col;
					if(isvalid(row,col-1,0 , 6 ,0 , 6)){           			
						if(grid[row][col-1]==""){moves.push_back(column[col-1]+to_string(7-(row)));}
						else{if(!isteam(grid[row][col][0] , grid[row][col-1][0] )){moves.push_back(column[col-1]+to_string(7-(row)));}}	
					}
					if(isvalid(row,col+1,0 , 6 ,0 , 6)){           			
						if(grid[row][col+1]==""){moves.push_back(column[col+1]+to_string(7-(row)));}
						else{if(!isteam(grid[row][col][0] , grid[row][col+1][0] )){moves.push_back(column[col+1]+to_string(7-(row)));}}	
					}
					
					if(side_to_play=="b"){
						/* ------- forward -------- */
						if(isvalid(row+1,col,0 , 6 ,0 , 6)){           			
							if(grid[row+1][col]==""){moves.push_back(column[col]+to_string(7-(row+1)));}
							else{if(!isteam(grid[row][col][0] , grid[row+1][col][0] )){moves.push_back(column[col]+to_string(7-(row+1)));}}	
						}
						if(isvalid(row+1,col-1,0 , 6 ,0 , 6)){           			
							if(grid[row+1][col-1]==""){moves.push_back(column[col-1]+to_string(7-(row+1)));}
							else{if(!isteam(grid[row][col][0] , grid[row+1][col-1][0] )){moves.push_back(column[col-1]+to_string(7-(row+1)));}}	
						}
						if(isvalid(row+1,col+1,0 , 6 ,0 , 6)){           			
							if(grid[row+1][col+1]==""){moves.push_back(column[col+1]+to_string(7-(row+1)));}
							else{if(!isteam(grid[row][col][0] , grid[row+1][col+1][0] )){moves.push_back(column[col+1]+to_string(7-(row+1)));}}	
						}
						/* ------ retreating ---- */
						if(isvalid(row-1 , col,0 , 6 ,0 , 6) && grid[row-1][col]==""){           			
							moves.push_back(column[col]+to_string(7-(row-1)));
							if(isvalid(row-2 , col,0 , 6 ,0 , 6) && grid[row-2][col]=="" ){
								moves.push_back(column[col]+to_string(7-(row-2)));								
							}
						}
						if(isvalid(row-1 , col+1,0 , 6 ,0 , 6) && grid[row-1][col+1]==""){ 
							moves.push_back(column[col+1]+to_string(7-(row-1)));     
							if(isvalid(row-2 , col+2,0 , 6 ,0 , 6) && grid[row-2][col+2]==""){       
								moves.push_back(column[col+2]+to_string(7-(row-2)));				
							}			
						}
						if(isvalid(row-1 , col-1,0 , 6 ,0 , 6) && grid[row-1][col-1]==""){ 
							moves.push_back(column[col-1]+to_string(7-(row-1)));      
							if(isvalid(row-2 , col-2,0 , 6 ,0 , 6) && grid[row-2][col-2]==""){       
								moves.push_back(column[col-2]+to_string(7-(row-2)));				
							}			
						}				
					}else{
						/* ------- forward -------- */
						if(isvalid(row-1,col,0 , 6 ,0 , 6)){           			
							if(grid[row-1][col]==""){moves.push_back(column[col]+to_string(7-(row-1)));}
							else{if(!isteam(grid[row][col][0] , grid[row-1][col][0] )){moves.push_back(column[col]+to_string(7-(row-1)));}}	
						}
						if(isvalid(row-1,col-1,0 , 6 ,0 , 6)){           			
							if(grid[row-1][col-1]==""){moves.push_back(column[col-1]+to_string(7-(row-1)));}
							else{if(!isteam(grid[row][col][0] , grid[row-1][col-1][0] )){moves.push_back(column[col-1]+to_string(7-(row-1)));}}	
						}
						if(isvalid(row-1,col+1,0 , 6 ,0 , 6)){           			
							if(grid[row-1][col+1]==""){moves.push_back(column[col+1]+to_string(7-(row-1)));}
							else{if(!isteam(grid[row][col][0] , grid[row-1][col+1][0] )){moves.push_back(column[col+1]+to_string(7-(row-1)));}}	
						}
						/* ------ retreating ---- */
						if(isvalid(row+1 , col,0 , 6 ,0 , 6) && grid[row+1][col]==""){           			
							moves.push_back(column[col]+to_string(7-(row+1)));
							if(isvalid(row+2 , col,0 , 6 ,0 , 6) && grid[row+2][col]=="" ){
								moves.push_back(column[col]+to_string(7-(row+2)));								
							}
						}
						if(isvalid(row+1 , col+1,0 , 6 ,0 , 6) && grid[row+1][col+1]==""){
							moves.push_back(column[col+1]+to_string(7-(row+1)));      
							if(isvalid(row+2 , col+2,0 , 6 ,0 , 6) && grid[row+2][col+2]==""){       
								moves.push_back(column[col+2]+to_string(7-(row+2)));				
							}			
						}
						if(isvalid(row+1 , col-1,0 , 6 ,0 , 6) && grid[row+1][col-1]==""){ 
							moves.push_back(column[col-1]+to_string(7-(row+1)));      
							if(isvalid(row+2 , col-2,0 , 6 ,0 , 6) && grid[row+2][col-2]==""){       
								moves.push_back(column[col-2]+to_string(7-(row+2)));				
							}			
						}
				
					}
				}
			}
			return moves;
		}	
};
void print_results(string state){
	board *b = new movepiece(state);
	b->evaluation(); 	
}
int main(){
	vector<string>lines;
	int N;
	cin >> N;
	cin.ignore(); //NB!
	for (int i = 0; i < N; ++i) {
		string fen;
		getline(cin, fen);
		lines.push_back(fen);
	}
	for(int i = 0  ; i < N ; i++){
		print_results(lines[i]);
		if(i != N-1){
			cout<<endl;			
		}
	} 
	                                                                                     	
	return 0;
}


