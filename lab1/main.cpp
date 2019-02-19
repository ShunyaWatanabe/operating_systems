#include "main.h"
#include "built_in.h" // functions for built in commands
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
	while (getline(file, entry)){
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

string search(list<string> history, int num){
	list<string>::iterator it = history.begin();
	advance(it, num-1);
	return *it;
}

void get_actual_paths(vector<string>&actual_paths, string paths){ // almost identical to get_words()
	char char_array[PATH_SIZE];
	strcpy(char_array, paths.c_str());
	char* token = strtok(char_array, ":");
	while (token != NULL){
		actual_paths.push_back(token);
		token = strtok(NULL, ":");
	}
}

void do_external(vector<string> words, string path){
	// check external commands
	vector<string> actual_paths;
	get_actual_paths(actual_paths, path);
	bool is_found = false;
	for (vector<string>::iterator itr = actual_paths.begin(); itr != actual_paths.end(); itr++){
		const char* path = (*itr).append("/").append(words.at(0)).c_str();
		if (access(path, F_OK) == 0){
			// success
			cout << words.at(0) << " is an external command (" <<	*itr << ")" << endl;
			cout << "command arguments:" << endl;
			for (vector<string>::iterator itr = words.begin()+1; itr != words.end(); itr++){
				cout << *itr << endl;
			}
			is_found = true;
			break;
		}
	}	
	if (!is_found){
		cout<<"command not found"<<endl;	
	}
}

int main(){

	// initializatoin process
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

	// accept commands
	while (1){
		cout << ">>";
		getline(cin, input);

		// get commands from history if given '!'
		if (input.at(0) == '!'){
			input.erase(0, 1);
			try {
				int num = stoi(input);
				if (num <= 100 && num > 0){
					input = search(history, num);
				} 
			}
			catch(invalid_argument& e){
				// if no conversion could be performed
				continue;
 			}
			catch(...){
				cout<<"error"<<endl; // maybe introduce better error message
				do_exit(history);
			}
		}

		append_history(history, input);

		vector<string> words;
		get_words(words, input);
		// simple error check
		if (words.size() == 0){
			continue;
		}

		string word1 = words.at(0);

		// perform action
		if (word1 == "exit") do_exit(history);
		else if (word1 == "history") do_history(history);
		else if (word1 == "pwd") do_pwd();
		else if (word1 == "export") do_export(words, paths);
		else if (word1 == "cd") do_cd(words);
		else do_external(words, paths.at("PATH"));
	}

	return 0;
}
