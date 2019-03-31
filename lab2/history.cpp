#include "history.h"
#include "built_in.h"
using namespace std;

list<string> make_history(){
	ifstream file;
	file.open("history.txt");
	list<string> history;
	string entry;
	if (file.is_open()){
		while (getline(file, entry)){
			history.push_back(entry);
		}
	}
	return history;
}

void append_history(list<string>& history, string input){
	if (history.size() == 100){
		history.pop_front();
	}
	history.push_back(input);
}

string search(list<string> history, int num){
	list<string>::iterator it = history.begin();
	advance(it, num-1);
	return *it;
}

void execute_history(list<string> history, string* input){
	if ((*input).at(0)== '!'){
		(*input).erase(0, 1);
		try {
			int num = stoi((*input));
			if (num <= 100 && num > 0){
				*input = search(history, num);
			} 
		}
		catch(invalid_argument& e){
			// if no conversion could be performed
			cerr<<"history: no conversion could be performed"<<endl;
		}
		catch(...){
			cerr<<"history: unknown error"<<endl; // maybe introduce better error message
			do_exit(history);  // TODO ask nabil if this is reachable
		}
	}
}

