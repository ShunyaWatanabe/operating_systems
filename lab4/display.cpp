#include "variables.h"
#include "display.h"
using namespace std;

void display(char* file, char* path){
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
	while ((n = read(f, buff, SMALL_BUFF)) > 0){
		cout << buff << endl;
		for (int i=0; i<5; i++){
			n = read(f, buff, SMALL_BUFF);
		}
	} 
}	
