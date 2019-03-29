#include "external.h"
#define PATH_SIZE 100
using namespace std;

void get_actual_paths(vector<string>&actual_paths, string paths){ // almost identical to get_words()
	char char_array[PATH_SIZE];
	strcpy(char_array, paths.c_str());
	char* token = strtok(char_array, ":");
	while (token != NULL){
		actual_paths.push_back(token);
		token = strtok(NULL, ":");
	}
}

// taken from the url below to convert string vector to char pointer array
// https://stackoverflow.com/questions/7048888/stdvectorstdstring-to-char-array
char* convert(const string &s){
	char *pc = new char[s.size()+1];
	strcpy(pc, s.c_str());
	return pc;
}

void do_external(vector<string> words, string path){
	// check external commands
	vector<string> actual_paths;
	get_actual_paths(actual_paths, path);
	vector<char*> char_array;
	transform(words.begin(), words.end(), back_inserter(char_array), convert);
	char_array.push_back((char*) 0);	

	bool is_found = false;
	for (vector<string>::iterator itr = actual_paths.begin(); itr != actual_paths.end(); itr++){
		const char* path = (*itr).append("/").append(words.at(0)).c_str();
		if (access(path, F_OK) == 0){
			// success
			// execute a file
			int pid=fork();
			int status;
			if (pid == 0){
				if (execv(path, &char_array[0]) == -1){
					cout << "Error in execv(): " << errno << ": " << strerror(errno)  << endl;				
				}	
				exit(0);
			}
			waitpid(pid, &status, 0); // wait for child process to end
			is_found=true;
			break;
		}
	}	
	if (!is_found){
		cout<<"command not found"<<endl;	
	}
}

void do_external2(vector<string> words, string path){
	// check external commands
	vector<string> actual_paths;
	get_actual_paths(actual_paths, path);
	vector<char*> char_array;
	transform(words.begin(), words.end(), back_inserter(char_array), convert);
	char_array.push_back((char*) 0);	

	bool is_found = false;
	for (vector<string>::iterator itr = actual_paths.begin(); itr != actual_paths.end(); itr++){
		const char* path = (*itr).append("/").append(words.at(0)).c_str();
		if (access(path, F_OK) == 0){
			// success
			// execute a file
			if (execv(path, &char_array[0]) == -1){
				cerr << "Error in execv(): " << errno << ": " << strerror(errno)  << endl;				
			}
		}
	}	
}
