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

void perform_action(string word, list<string>&history, vector<string>& words, map<string, string> &paths){
	// perform action
	if (word == "exit") do_exit(history);
	else if (word == "history") do_history(history);
	else if (word == "pwd") do_pwd();
	else if (word == "export") do_export(words, paths);
	else if (word == "cd") do_cd(words);
	else do_external(words, paths.at("PATH"));
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
		execute_history(history, &input);
		append_history(history, input);
		
		vector<string> words;
		get_words(words, input);
		if (words.size() == 0){
			continue;
		}
		perform_action(words.at(0), history, words, paths);
	}
	return 0;
}
