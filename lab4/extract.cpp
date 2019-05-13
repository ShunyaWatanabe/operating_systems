#include "variables.h"
#include "extract.h"
using namespace std;

void extract(char* file, char* path, int num){
	int f = open(file, O_RDONLY);
	if (f == -1){
		cerr << "Error opening the file: " << file <<endl;
		exit(0);
	}
	// read the header
	char buff[SMALL_BUFF];
	int n;
	n = read(f, buff, SMALL_BUFF);
	cout<<buff<<endl;
	n = atoi(buff);
	
	// seek the files info at the back
	vector <int> sizes;
	vector <string> files;
	off_t new_pos;
	new_pos = lseek(f, n+SMALL_BUFF, SEEK_SET);

	int m;
	while ((m = read(f, buff, SMALL_BUFF)) > 0){
		files.push_back(buff);
		m = read(f, buff, SMALL_BUFF);
		sizes.push_back(atoi(buff));
	}

	// find the position for start of files
	new_pos = lseek(f, SMALL_BUFF, SEEK_SET);

	// make directory
	if (mkdir("files", 0777) == -1) {
        	cerr << "Error:  " << strerror(errno) << endl;
	} 

	// create files in directory
	for (int i=0; i<files.size(); i++){
		int nf = creat(("files/"+files.at(i)).c_str() ,0644);
		if (nf == -1){
			cerr << "Error : " << strerror(errno) << endl;
		}
		int remainder = sizes.at(i);
		int write_size = SMALL_BUFF;
		while (remainder > 0){
			if (remainder < SMALL_BUFF){
				write_size = remainder;
			}
			// read from .ad file
			read(f, buff, write_size);
			// write to a new file
			write(nf, buff, write_size);
			remainder -= SMALL_BUFF;
		}
		close(nf);
	}
}
