#include "cashier.h"
#include "stats.h"

using namespace std; 

void getArgs(int argc, char** argv, int* sTime, int* bTime, int* id, int*cId){
	if (argc != NUMARGS){
		cerr<<"wrong number of arguments. it has to be " << NUMARGS <<endl;
		exit(0);
	}
	try {
		int count = 0;
		for (int i=1; i<NUMARGS; i+=2){
			if (strcmp(argv[i], "-s") == 0){
				*sTime = atoi(argv[i+1]);
				count += 1;
			}
			else if (strcmp(argv[i], "-b") == 0){
				*bTime = atoi(argv[i+1]);
				count += 2;
			}
			else if (strcmp(argv[i], "-m") == 0){
				*id = atoi(argv[i+1]);
				count += 3;
			}
			else if (strcmp(argv[i], "-i") == 0){
				*cId = atoi(argv[i+1]);
				count += 4;
			}
		}
		if (count != 10){
			cerr << "wrong format" << endl;
			exit(0); 
		}
	} catch (...){
		cerr <<"cannot convert the arguments"<<endl;
		exit(0);
	}
}

int main(int argc, char** argv) {
	int sTime, bTime, id, cId;

	// init
	getArgs(argc, argv, &sTime, &bTime, &id, &cId);
	srand (time(NULL));

	// attach
	Stats* stats;
	stats = (Stats*) shmat(id, (void*) 0, 0);

	while (1) {
		// if closed, break
		if ((*stats).isClosed){
			break;
		}

		// check if there is customer, sleep if no
		cout<<"         queuesize = "<<(*stats).queueSize<<endl;
		if ((*stats).queueSize == 0){
			cout<<"cashier " << cId << ": no clients. sleep for "<< bTime<<endl;
			sleep(bTime);
			continue;
		}

		// get a key to start working
		sem_wait(&(*stats).cashierKey);
		cout<<"cashier " << cId << ": got cashierKey"<<endl;

		// signal client that you are available
		cout<<"cashier " << cId << ": call client"<<endl;
		sem_post(&(*stats).cashiers);

		// work and signal after you are done
		int serviceTime = rand() % sTime + 1; // get a random service time
		(*stats).serviceTime = serviceTime;
		cout<<"cashier " << cId <<": instructed client"<<endl;
		sem_post(&(*stats).cashierToClient);

		// check order after the client places it
		sem_wait(&(*stats).clientToCashier);
		cout<<"cashier " << cId <<": accepted order"<<endl;
		int itemId = (*stats).itemId;
		int clientId = (*stats).clientId;
		cout<<"cashier " << cId <<": order was " << itemId << " and the client was " << clientId << endl;

		// write the itemId to database		
		sem_wait(&(*stats).database);
		cout<<"cashier " << cId <<": got permission for database"<<endl;
		int index = itemId-1;
		ofstream outfile;
		outfile.open("database.txt", ios_base::app);
		outfile << itemId << " " << (*stats).menu[index].price << " " <<  clientId << endl;
		outfile.close();
		cout<<"cashier " << cId <<": release database semaphore"<<endl;
		sem_post(&(*stats).database);
	
		// done serving. release key
		cout<<"cashier " << cId <<": release cashierKey"<<endl;
		sem_post(&(*stats).cashierKey);

		cout<<endl;
		sleep(sTime);
	}
	// detach
	if (shmdt((void*) stats) == -1){
		cerr <<"error detaching"<<endl;
		exit(0);
	}
	return 0; 
} 

