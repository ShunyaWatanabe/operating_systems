#include "variables.h"
#include "store.h"
using namespace std;

void iterate(vector<string>* paths, char* path){
	struct stat statbuf;
	struct dirent* dirent_ptr;
	DIR* catalog;
	if (stat(path, &statbuf) == -1){
		cerr<<"Failed to get file status"<<endl;
		exit(0);
	}	
	// check if path is a file or directory
	if ((statbuf.st_mode & S_IFMT) == S_IFDIR){
		// it's a directory
		if ((catalog = opendir(path)) == NULL){
			cerr << "error opening a directory: " << path << endl;
			return;
		}
		while ((dirent_ptr = readdir(catalog)) != NULL){
			char* entry = dirent_ptr->d_name;
			if (strcmp(entry,"..") == 0 || strcmp(entry,".") == 0){
				//cout << entry << endl;
			} else {
				//add previous path
				/*if (strcmp(path,".") == 0 || strcmp(path,"./") == 0 || strcmp(path,"..") == 0|| strcmp(path,"../")==0  ){
					memset(path, 0, sizeof(path));
				}*/
				string buf(path);
				if (buf.rfind(string(1,'/')) == string::npos || buf.rfind(string(1,'/')) != buf.length()-1){	
					char tmp[] = "/";
					char *slash = &tmp[0u];
					buf.append("/");
				}
				buf.append(entry);
				char *new_path = &buf[0u];
				iterate(paths, new_path);
				
				//iterate(paths, entry);
			}
		}
	}
	else if ((statbuf.st_mode & S_IFMT) == S_IFREG){
		// it's a file
		// open a file, append it to .ad file and close it
		string p(path);
		//paths->push_back(p.substr(p.find('/') + 1));
		paths->push_back(p);
	}
	else{
		cerr<<"Detected a path that is not a directory or a file"<<endl;
	}
}

void store(char* file, char* path){	
	vector<string> paths;
	vector<int> sizes;
	iterate(&paths, path);
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	int to = creat(file, 00644);
	// 1: header	
	char empty_buff[SMALL_BUFF] = {0};
	write(to, empty_buff, SMALL_BUFF);
	int total = 0;
	// 2: files
	for (int i=0; i<paths.size();i++){
		int fr = open(paths.at(i).c_str(), O_RDONLY);
		if (fr == -1){
			cerr<<"Error opening the file: "<< paths.at(i)<<endl;
		}
		int n = 0;
		int filesize = 0;
		char buffer[LARGE_BUFF];
		while ((n = read(fr, buffer, LARGE_BUFF)) > 0){
			write(to, buffer, n);
			total += n;
			filesize += n;
		}
		sizes.push_back(filesize);
		close(fr);
	}
	// 3: file info
	for (int i=0; i<sizes.size(); i++){
		// path
		string tmp = paths.at(i);
		char *info = &tmp[0u];
		write(to, info, SMALL_BUFF);
		// filesize
		tmp = to_string(sizes.at(i));
		info = &tmp[0u];
		write(to, info, SMALL_BUFF);
		// owner, group, rights, type of file
		struct stat statbuf;
		tmp = paths.at(i);
		info = &tmp[0u];
		if (stat(info, &statbuf) == -1){
			cerr << "Error: cannot get stats: " << paths.at(i) << endl;
		}	
		tmp = to_string(statbuf.st_uid);
		info = &tmp[0u];
		write(to, info, SMALL_BUFF);
		tmp = to_string(statbuf.st_gid);
		info = &tmp[0u];
		write(to, info, SMALL_BUFF);
		tmp = to_string(statbuf.st_mode & 111111111);
		info = &tmp[0u];
		write(to, info, SMALL_BUFF);
		tmp = to_string(statbuf.st_mode & S_IFMT);
		info = &tmp[0u];
		write(to, info, SMALL_BUFF);
		
	}
	// write back to header the total size of files
	off_t new_pos;
	new_pos = lseek(to, 0, SEEK_SET);
	string tmp = to_string(total);
	char *t = &tmp[0u];
	write(to, t, 2048); 	
	// done
	close(to);
}
