#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
using namespace std;
#define NUM_CASHIERS 4
#define NUM_CLIENTS 100


int main(){
	
	srand(time(NULL));

	string shmid;
	cout<<"input shmid: ";
	cin>>shmid;

	// create customers
	for (int i=0; i<NUM_CLIENTS; i++){
		sleep(rand() % 2);
		int itemId = rand() % 20 + 1;
		int eatTime = rand() % 10 + 1;
		//spawn child
		int pid = fork();

		if (pid < 0){
			cerr<<"error forking child"<<endl;
			close(0);
		}

		else if (pid == 0){
			// child process
			string command = "./client -i " + to_string(itemId) + " -e " + to_string(eatTime) + " -m " + shmid;
			system(command.c_str());
			return 0;
		}
	}

	// for parent
	for (int i=0; i<NUM_CLIENTS; i++){
		wait(NULL);
	}	

	cout<<"done"<<endl;

	return 0;
}
