#include "coordinator.h"
using namespace std;

char descriptions [][30] = { 
	"BBQ-Chicken-Salad",
	"Spinach-Power",
	"Garden-Salad",
	"Steak-Blue-Cheese",
	"Ceasars-Salad",
	"Chicken-Salad",
	"Mongolican-BBQ-Plate",
	"Club-Sandwich",
	"Belgian-Cheese-Sub",
	"Rio-Grande-Beef-Sub",
	"Argentine-Asado-Club",
	"Sierra-Sub",
	"Avocado-BLT",
	"Soup-de-Egion",
	"Soup-de-Sur",
	"Coffee",
	"Hot-Tea",
	"Hot-Chocolate",
	"Mocha",
	"Cafe-Late"
};

float prices [20] = {
	8.95, 9.15, 4.75, 7.25, 6.75,
	9.15, 9.75, 6.35, 10.25, 9.35, 
	11.75, 10.38, 8.05, 3.20, 2.75, 
	1.25, 1.05, 2.15, 3.25, 3.75
};

int minTimes [20] = {
	18, 12, 10, 12, 13, 15, 21, 13, 15, 18,
	23, 12, 12, 11, 6, 2, 1, 1, 2, 5
};

int maxTimes [20] = {
	24, 16, 13, 15, 15, 21, 31, 18, 19, 20,
	30, 15, 13, 15, 9, 4, 4, 2, 3, 7
};

int getMaxNumCashiers(int argc, char** argv){
	// get args
	if (argc != 3) {
		cerr<<"wrong format"<<endl;
		exit(0);
	}
	try {	
		if (strcmp(argv[1], "-n") != 0){
			cerr<<"Use -n to specify the max number of cashiers"<<endl;
			exit(0);
		}
		return atoi(argv[2]);
	} catch (...) {
		cerr<<"Error converting the max number of cashiers to an integer value"<<endl;
		exit(0);
	}
}

int main(int argc, char** argv) {
	srand(time(NULL));
	int maxNumCashiers = getMaxNumCashiers(argc, argv);
	Stats* stats;

	int id = shmget(IPC_PRIVATE, MEMORY_SIZE, 0666); 	
	if (id == -1){
		cerr<<"Failed to create shared memory"<<endl;	
	}

	cout<<"id is "<<id<<endl;

	// attach the segment
	stats = (Stats*)shmat(id, (void*)0, 0);

	if (*(int*) stats == -1){
		cerr<<"Failed to attach shared memory"<<endl;
	}

	// initialize the variables
	(*stats).maxPeople = MAX_PEOPLE;
	for (int i=0; i<20; i++){
		(*stats).menu[i].init(i+1, descriptions[i], prices[i], minTimes[i], maxTimes[i]);
	}
	sem_init(&(*stats).door, 1, 1);
	sem_init(&(*stats).cashiers, 1, 0);
	sem_init(&(*stats).cashierKey, 1, 1);
	sem_init(&(*stats).clientKey, 1, 1);
	sem_init(&(*stats).cashierToClient, 1, 0);
	sem_init(&(*stats).clientToCashier, 1, 0);
	sem_init(&(*stats).clientToServer, 1, 0);
	sem_init(&(*stats).serverToClient, 1, 0);
	sem_init(&(*stats).serve, 1, 1);
	sem_init(&(*stats).database, 1, 1);
	//sem_init(&(*stats).close, 1, 0);

	// hire cashiers
        for (int i=0; i<maxNumCashiers; i++){
		int serveTime = rand() % 20 + 1;
		int breakTime = rand() % 20 + 1;
		// spawn child
		int pid = fork();
		if (pid < 0){
	    		cerr<<"error forking child"<<endl;
	  		close(0);
		}
		else if (pid == 0){
			// child process
			string command = "./cashier -s " + to_string(serveTime) + " -b "+ to_string (breakTime) +" -m " + to_string(id) + " -i " + to_string(i+1);
			system(command.c_str());
			return 0;
		}
	}

	// hire a server
	int pid = fork();
	if (pid < 0){
		cerr<<"error forking child"<<endl;
		close(0);
	}
	else if (pid == 0){
		// child process
		string command = "./server -m " + to_string(id);
		system(command.c_str());
		return 0;
	}
	
	for (int i=0; i< maxNumCashiers + 1; i++){
		wait(NULL);
	}

	// the child processes are dead
	cout <<"child processes all died"<<endl;

	// record the data and output a result
	ifstream infile("database.txt");
	ofstream outfile;
	outfile.open("result.txt");
	int itemId, clientId;
	float price;
	int freqs[20] = {0};
	float revenue = 0.0;
	int numCustomers = 0;
	while (infile >> itemId >> price >> clientId){
		freqs[itemId-1] += 1;
		numCustomers += 1;
		revenue += price;
		outfile << "client ID: " << clientId << ", money: " << price << "\n";	
	}
	outfile << "\n";

	for (int i=0; i<5; i++){
		int max = 0;
		int index = -1;
		for (int j=0; j<20; j++){
			if (max < freqs[j]){
				max = freqs[j];
				index = j;
			}
		}
		outfile << "top " << i+1 << ": " << (*stats).menu[index].description << ", selling " << max*(*stats).menu[index].price << "\n";
		freqs[index] = -1;
	}

	outfile << "\n";
	outfile << "number of customers: " << numCustomers <<", total revenue: " << revenue << "\n";
	infile.close();
	outfile.close();

	sem_destroy(&(*stats).door);
	sem_destroy(&(*stats).cashiers);
	sem_destroy(&(*stats).cashierKey);
	sem_destroy(&(*stats).clientKey);
	sem_destroy(&(*stats).cashierToClient);
	sem_destroy(&(*stats).clientToCashier);
	sem_destroy(&(*stats).clientToServer);
	sem_destroy(&(*stats).serverToClient);
	sem_destroy(&(*stats).serve);
	sem_destroy(&(*stats).database);
	//sem_destroy(&(*stats).close);

	// remove segment
	if (shmctl(id, IPC_RMID, 0) == -1){
		cerr<<"Failed to detach memory segment"<<endl;
	}
	
	cout << "all clean!" << endl;
	return 0;
}
