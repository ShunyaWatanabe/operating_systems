#include "variables.h"
#include "meta.h"
using namespace std;

void meta(char* file, char* path){
	cout<<"meta"<<endl;
	// open .ad file
	int f = open(file, O_RDONLY);
	if (f == -1){
		cerr<<"Error opening a file: "<<file<<endl;
	}

	// read the header file
	char buff[SMALL_BUFF];
	int n;
	n = read(f, buff, SMALL_BUFF);
	int data_size = atoi(buff);

	// move to the back and get meta data
	off_t new_pos = lseek(f, SMALL_BUFF+data_size, SEEK_SET);
	int i = 0;
	cout << "filename, size, owner, group, rights, type of file" << endl;
	while ((n = read(f, buff, SMALL_BUFF)) > 0){
		i++;
		if (i == 5){
			cout << ((atoi(buff) & S_IRUSR) ? "r" : "-");
			cout << ((atoi(buff) & S_IWUSR) ? "w" : "-");
			cout << ((atoi(buff) & S_IXUSR) ? "x" : "-");
			cout << ((atoi(buff) & S_IRGRP) ? "r" : "-");
			cout << ((atoi(buff) & S_IWGRP) ? "w" : "-");
			cout << ((atoi(buff) & S_IXGRP) ? "x" : "-");
			cout << ((atoi(buff) & S_IROTH) ? "r" : "-");
			cout << ((atoi(buff) & S_IWOTH) ? "w" : "-");
			cout << ((atoi(buff) & S_IXOTH) ? "x" : "-");
		}
		else if (i == 6){
			cout << (atoi(buff) & S_IFMT);
		}
		else {
			cout << buff;
		}

		if (i == 6){
			cout<<endl;
			i = 0;
		} else {
			cout<<",  ";
		}
	}
	


// path
// string tmp = paths.at(i);
// char *info = &tmp[0u];
// write(to, info, SMALL_BUFF);
// // filesize
// tmp = to_string(sizes.at(i));
// info = &tmp[0u];
// write(to, info, SMALL_BUFF);
// // owner, group, rights, type of file
// struct stat statbuf;
// tmp = paths.at(i);
// info = &tmp[0u];
// if (stat(info, &statbuf) == -1){
//         cerr << "Error: cannot get stats: " << paths.at(i) << endl;
//         }
//         tmp = to_string(statbuf.st_uid);
//         info = &tmp[0u];
//         write(to, info, SMALL_BUFF);
//         tmp = to_string(statbuf.st_gid);
//         info = &tmp[0u];
//         write(to, info, SMALL_BUFF);
//         tmp = to_string(statbuf.st_mode & 111111111);
//         info = &tmp[0u];
//         write(to, info, SMALL_BUFF);
//         tmp = to_string(statbuf.st_mode & S_IFMT);
//         info = &tmp[0u];
//         write(to, info, SMALL_BUFF);

}
