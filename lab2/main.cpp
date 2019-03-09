#include "main.h"
#include "built_in.h" // functions for built in commands
#include "external.h"
#include "history.h"
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

int main(){

	// initializatoin process
	list<string> history = make_history();

	map<string, string> paths;
	paths.insert({"PATH", ""});

	string input = "";

	// accept commands
	while (1){
		cout << ">>";
		getline(cin, input);

		// get commands from history if given '!'
		if (input.at(0) == '!'){
			if (execute_history(history, &input) == -1){
				cout<<"error"<<endl;
				continue;
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
