#include "main.h"
#include <iostream>
#include <fstream>
using namespace std;

map<string, string> paths;
string input;

void get_words(vector<string>& words, string input){
	// split line into words
	char char_array[PATH_SIZE];
	strcpy(char_array, input.c_str());
	char* token = strtok(char_array, " ");
	while (token != NULL){
		words.push_back(token);
		token = strtok(NULL, " ");
	}
	for (auto i = words.begin(); i!=words.end(); ++i){
		cout<< *i << ' ';
	}

}


void do_export(vector<string> words){
	if (words.size() >= 2){
		string word2 = words.at(1);
		if (word2.find("=") != -1){
			string key = word2.substr(0, word2.find("="));
			string val = word2.substr(word2.find("=")+1, word2.length());
			if (key == "PATH"){
				paths["PATH"] = val;
			} else {
				paths.insert({key, val});
			}
		}
	}
	map<string, string>::iterator itr;
	for (itr = paths.begin(); itr != paths.end(); ++itr){
		cout << itr->first << "=" << itr->second << endl;
	}
}

void do_pwd(){
	char wd[PATH_SIZE];
	getcwd(wd, PATH_SIZE); 
	cout<< wd << endl;
}

void do_history(list<string> history){
	list<string>::iterator itr;
	int i = 1;
	for (itr = history.begin(); itr != history.end(); ++itr){
		cout << i << " " << itr->c_str() << endl;
		i++;
	}
}

void do_cd(vector<string> words){
	if (words.size() < 2) {
		return; // if path is not given, don't do anything
	}
	string word2 = words.at(1);
	char wd[PATH_SIZE];
	strcpy(wd, word2.c_str());
	if (chdir(wd) == -1){
		cout << "error occured" << endl;
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

void do_exit(list<string> history){
	if (history.size() > 0){
		ofstream file;
		file.open("history.txt");
		if (file.is_open()){
			list<string>::iterator itr;
			for (itr = history.begin(); itr != history.end(); ++itr){
				file << itr->c_str() << endl;
			}
			file.close();
		}	
	}
}

void append_history(list<string> history, string input){
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
	paths.insert({"PATH", ""});
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
			do_export(words);
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
