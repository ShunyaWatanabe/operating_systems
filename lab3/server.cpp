#include "server.h"
using namespace std;

int getShmid(int argc, char** argv){
	// get args
	if (argc != 3) {
		cerr<<"wrong format"<<endl;
		exit(0);
	}
	try {	
		if (strcmp(argv[1], "-m")!= 0){
			cerr<<"Use -m to specify the shmid"<<endl;
			exit(0);
		}
		return atoi(argv[2]);
	} catch (...) {
		cerr<<"Error converting the max number of cashiers to an integer value"<<endl;
		exit(0);
	}
}

int main(int argc, char** argv) {
	int shmid = getShmid(argc, argv);
	Stats* stats;

	// attach the segment
	stats = (Stats*)shmat(shmid, (void*)0, 0);

	if (*(int*) stats == -1){
		cerr<<"Failed to attach shared memory"<<endl;
	}
	
	int numServes = 0;
	cout<<"server created"<<endl;
	while (1){

		if ((*stats).isClosed == 1){
			break;
		}
		// wait until client asks for food
		cout<<endl;
		cout<< "   server: start waiting for client"<<endl;
		cout<<endl;
		sem_wait(&(*stats).clientToServer);
		cout<<endl;
		cout<< "   server: called by client"<<endl;
		// idk what to do here
		/*
		// read the order, check which order to process
		ifstream database ("database.txt");
		string line;
		int itemId, clientId; 
		float price;
		while (database >> itemId >> price >> clientId){
			if (clientId == (*stats).orderNo){
				// found client id for the order
				
			}
		database.close();	
		}*/
		
		// acknowledge the order
		cout<< "   server: done. call client"<<endl;
		cout<<endl;
		sem_post(&(*stats).serverToClient);

	}	

	// detach
	 if (shmdt((void*) stats) == -1){
		cerr <<"error detaching"<<endl;
		exit(0);
	}
	return 0;
}
