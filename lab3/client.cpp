#include "client.h"
#include "stats.h"
using namespace std; 

void getArgs(int argc, char** argv, int* iId, int* eTime, int* id){
	if (argc != NUMARGS){
		cerr<<"wrong number of arguments. it has to be" << NUMARGS <<endl;
		exit(0);
	}
	try {
		int count = 0;
		for (int i=1; i<NUMARGS; i+=2){
			if (strcmp(argv[i], "-i") == 0){
				*iId = atoi(argv[i+1]);
				count += 1;
			}
			else if (strcmp(argv[i], "-e") == 0){
				*eTime = atoi(argv[i+1]);
				count += 2;
			}
			else if (strcmp(argv[i], "-m") == 0){
				*id = atoi(argv[i+1]);
				count += 3;
			}
		}
		if (count != 6){
			cerr << "wrong format" << endl;
			exit(0); 
		}
	} catch (...){
		cerr <<"cannot convert the arguments"<<endl;
		exit(0);
	}
}

int main(int argc, char** argv) {
	int iId, eTime, id;
	srand(time(NULL));

	// construct
	getArgs(argc, argv, &iId, &eTime, &id);

	// attach
	Stats* stats;
	stats = (Stats*) shmat(id, (void*) 0, 0);

	// go through a door
	sem_wait(&(*stats).door);
	if ((*stats).queueSize >= (*stats).maxPeople){
		cout << "   new client: leaves since too many people" << endl;
		sem_post(&(*stats).door);
		exit(0);
	}
	(*stats).queueSize += 1;
	(*stats).numClients += 1;
	int clientId = (*stats).numClients;
	sem_post(&(*stats).door);

	// the time starts after passing through the door
	time_t start = time(NULL);

	// wait for cashiers to accept orders
	sem_wait(&(*stats).cashiers);
	cout<<" client " << clientId<< ": was called by cashier"<<endl;

	// get key for atomicity
	sem_wait(&(*stats).clientKey);
	cout<<" client "<< clientId<< ": got clientKey"<<endl;
	
	// wait for cashier to send stuff
	sem_wait(&(*stats).cashierToClient);
	cout<<" client "<< clientId<< ": received instruction from cashier"<<endl;
	int serviceTime = (*stats).serviceTime;

	// send itemId and signal
	(*stats).itemId = iId;
	(*stats).clientId = clientId;

	// get out of queue
	sem_wait(&(*stats).door);
	(*stats).queueSize -= 1;
	sem_post(&(*stats).door);

	// let the cashier know you are done
	cout<<" client "<< clientId<<": order a menu to cashier"<<endl;
	sem_post(&(*stats).clientToCashier);

	// finish interaction. give key to the next client
	cout<<" client "<< clientId<<": released clientKey"<<endl;
	cout<<endl;
	sem_post(&(*stats).clientKey);

	// sleep for a while
	sleep(serviceTime);

	// calculate serving time
	int maxTime = (*stats).menu[iId].maxTime;
	int minTime = (*stats).menu[iId].minTime;
	int serveTime = rand() % (maxTime-minTime) + minTime;

	// sleep until getting served
	sleep(serveTime);

	// get key for atomicity
	sem_wait(&(*stats).serve);
	cout<<"    client "<< clientId<<": start getting served"<<endl;
	
	// let the server know which order you made
	(*stats).orderNo = clientId;
	//(*stats).serveTime = serveTime; //this is apparently needed according to the description, but it doesn't make sense

	// call server to serve for you
	cout<<"    client "<< clientId<<": call server"<<endl;
	sem_post(&(*stats).clientToServer);

	// wait for server to check the order, and pass you the food
	sem_wait(&(*stats).serverToClient);
	cout<<"    client "<< clientId<<": server reacted"<<endl;

	// release key
	cout<<"    client "<< clientId<<": finish getting served"<<endl;
	cout<<endl;
	sem_post(&(*stats).serve);

	// calculate eatTime and sleep for that much	
	int eatTime = rand() % (eTime) + 1;
	cout<<endl;
	cout<<"        client "<< clientId<<": sleep for "<<eatTime<<endl;
	cout<<endl;
	sleep(eatTime);

	// if you are the last customer, let them know it's ok to close
	if ((*stats).queueSize == 0){
		(*stats).isClosed = 1;
	}
	cout<<endl;
	cout<<"            client "<< clientId<<": leaving the restaurant!"<<endl;
	cout<<endl;
/*
	// get out
	sem_wait(&(*stats).exit);
	time_t end = time(NULL);
	int duration = (int) (end-start);
	(*stats).duration = duration;
	sem_post(&(*stats).clientToCoordinator);
	sem_wait(&(*stats).coordinatorToClient);
	sem_post(&(*stats).exit);
*/
	// detach
	if (shmdt((void*) stats) == -1){
		cerr <<"error detaching"<<endl;
		exit(0);
	}
	return 0; 
} 

