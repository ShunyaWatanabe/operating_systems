#include "main.h"
#include "built_in.h"
using namespace std;

void get_words(vector<string>& words, string input){
	// split line into words
	char char_array[PATH_SIZE];
	strcpy(char_array, input.c_str());
	char* token = strtok(char_array, " ");
	while (token != NULL){
		words.push_back(token);
		token = strtok(NULL, " ");
	}
}

list<string> make_history(ifstream& file){
	list<string> history;
	string entry;
	while (!file.eof()){
		file >> entry;
		history.push_back(entry);
	}
	return history;
}

void append_history(list<string>& history, string input){
	if (history.size() == 100){
		history.pop_front();
	}
	history.push_back(input);
}

int main(){

	ifstream file;
	file.open("history.txt");
	list<string> history;
	if (file.is_open()){
		history = make_history(file);
		file.close();
	}

	map<string, string> paths;
	paths.insert({"PATH", ""});

	string input = "";
	while (1){
		cout << ">>";
		getline(cin, input);
		history.push_back(input);

		vector<string> words;
		get_words(words, input);
		if (words.size() == 0){
			continue;
		}
		string word1 = words.at(0);

		// perform action
		if (word1 == "exit"){
			do_exit(history);
			break;
		}
		else if (word1 == "history"){
			do_history(history);		
		}
		else if (word1 == "pwd"){
			do_pwd();
		}
		else if (word1 == "export"){	
			do_export(words, paths);
		}
		else if (word1 == "cd"){
			do_cd(words);
		}
		else {
			// check external commands
			
		}
		append_history(history, input);
	}

	return 0;
}
