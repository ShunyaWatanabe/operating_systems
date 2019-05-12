#include "func.h"
using namespace std;

void store(char* file, char* path){	
	// check if path is a file or directory
	struct stat statbuf;
	struct dirent* entry;
	DIR* catalog;
	if (stat(path, &statbuf) == -1){
		cerr<<"Failed to get file status"<<endl;
		exit(0);
	}	
	if ((statbuf.st_mode & S_IFMT) == S_IFDIR){
		// it's a directory
		if ((catalog = opendir(path)) == NULL){
			cerr << "error opening a directory: " << path << endl;
			return;
		}
		while ((entry = readdir(catalog)) != NULL){
			//store(file, entry->d_name);
			cout << entry->d_name << endl;
		}
	}
	else if ((statbuf.st_mode & S_IFMT) == S_IFREG){
		// it's a file
		cout << path << endl;
	}
	else{
		cerr<<"Detected a path that is not a directory or a file"<<endl;
	}
}
void append(char* file, char* path){
	cout<<"append"<<endl;
}
void extract(char* file, char* path, int num){
	cout<<"extract"<<endl;
}
void meta(char* file, char* path){
	cout<<"meta"<<endl;
}
void print(char* file, char* path){
	cout<<"print"<<endl;
}	
