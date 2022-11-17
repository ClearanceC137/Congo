#include<iostream>
#include<vector>
#include <ctype.h>
#include <vector>
#include<bits/stdc++.h>
using namespace std;

class square{
	public:
		string color;             //black , white
		char type;                //lion , giraffe etc
		int rank;
		char column;
		square(string color,char type , int rank , char column){
			this->type = type;                //lion , giraffe etc
			this->rank = rank;
			this->column = column;
			this->color = color;			
		}
	
};

class board{
	public:
		string fen;
		string current_state[7];                     // from rank 1 to 7
		string number_of_moves;
		string side_to_play;		
		vector<vector<square>> positions;
		board(string fen){
			this->fen = fen;
			this->process_string();
			this->set_positions();
		}
		void process_string(){
				// processess string into separate ranks + side to play and number of moves				
			string str = fen;
		    string word = "";
		    vector<string> line;		    
		    // split by space
		    for (auto x : str)
		    {			
		        if (x == ' ')
		        {
		            line.push_back(word);
		            word = "";
		        }
		        else {
		            word = word + x;
		        }		       
		    }
		    line.push_back(word);
			
			word = "";
			str = line[0];
		    vector<string> rank;
		    // split by forward slash to obtain rank
		    for (auto x : str)
		    {
		        if (x == '/')
		        {
		            rank.push_back(word);
		            word = "";
		        }
		        else {
		            word = word + x;
		        }		       
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
			this->set_positions();         // reset current state of the game
		}
		/*  format blocks position of type 'type' */		
		void set_square(char type){
			char col[] = {'a','b','c','d','e','f','g'};
			vector<square>p;
			for(int i = 0 ; i < 7 ; i++){
					string curr = current_state[i];
					int strSize  = curr.size() , index = 0;
					for(int j = 0 ; j < strSize ; j++){
						if(isdigit(curr[j])){
							index+=(int)curr[j]-'0';
						}
						else{
							index++;
						}
						if(toupper(type) == curr[j]){
							square* block = new square("white",type, 7-i , col[index-1]);
							p.push_back(*block);
						}
						if(type == curr[j]){
							square* block = new square("black",type, 7-i , col[index-1]);
							p.push_back(*block);
						}						
					}	
			}
			positions.push_back(p);
			
		}
		/* sets all postions for each type */
		void set_positions(){
			char type[] = {'p','s','g','m','e','l','c','z'};
			for(int i = 0 ; i < 8 ; i++){
				set_square(type[i]);	
			}	
		}
		

};

string getString(char x)
{

    string s(1, x);
 
    return s;  
}
bool mycomp(string a, string b){
	//returns 1 if string a is alphabetically 
	//less than string b
	//quite similar to strcmp operation
	return a<b;
}
vector<string> alphabaticallySort(vector<string> a){
	int n=a.size();
	//mycomp function is the defined function which 
	//sorts the strings in alphabatical order
	sort(a.begin(),a.end(),mycomp);
	return a;
}
void print_results(string text){
     board *b = new board(text);
	string pieces[] = {"white pawn:","black pawn:","white superpawn:","black superpawn:","white giraffe:","black giraffe:","white monkey:","black monkey:","white elephant:","black elephant:","white lion:","black lion:","white crocodile:","black crocodile:","white zebra:","black zebra:","side to play:"};  
	char column[] = {'a','b','c','d','e','f','g'};
	int size = b->positions.size(),index = 0;	
	for(int i = 0 ; i< size ; i++){
		vector<square> each = b->positions[i];
		int sizej = each.size();
			vector<string> white;
			vector<string>black;
		for(int j = 0 ; j < sizej ; j++ ){

			if(each[j].color =="white"){
				white.push_back(each[j].column+to_string(each[j].rank));
			}
			else{
				black.push_back(each[j].column+to_string(each[j].rank));		
			}
		}
		white = alphabaticallySort(white);
		black = alphabaticallySort(black);
		for(string pos : white){
			pieces[index] = pieces[index] +" "+ pos;
		}
		for(string pos : black){
			pieces[index+1] = pieces[index+1] +" "+ pos;			
		}

		index+=2;
	}
	if(b->side_to_play == "w"){
		pieces[16] = pieces[16]+" white";
	}
	else{
		pieces[16] = pieces[16]+" black";		
	}
    for(int i = 0 ; i < 17 ; i++){
    	cout<<pieces[i]<<endl;
	}  
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


