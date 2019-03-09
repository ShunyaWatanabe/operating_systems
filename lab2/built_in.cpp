#include "built_in.h"
using namespace std;

void do_export(vector<string> words, map<string, string>& paths){
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

void do_exit(list<string> history){
	if (history.size() > 0){
		ofstream file;
		file.open("history.txt"); // TODO truncate the original text file
		if (file.is_open()){
			list<string>::iterator itr;
			for (itr = history.begin(); itr != history.end(); ++itr){
				file << itr->c_str() << endl;
			}
			file.close();
		}	
	}
	exit(0);
}

