#include <semaphore.h>

struct Item {
	int itemId;
	char description[30];
	float price;
	int minTime;
	int maxTime;
	void init (int a, char* b, int c, int d, int e){
		itemId = a;
		std::copy(b, b+30, description);
		price = c;
		minTime = d;
		maxTime = e;
	}
};

struct Stats {
	// shared information
	
	// keeps the queueSize in check
	sem_t door;
	int maxPeople;
	int numClients;
	int queueSize;

	// wait until cashier is ready
	sem_t cashiers;
	
	// semaphores for the client cashier interaction
	sem_t cashierKey;
	sem_t clientKey;
	sem_t cashierToClient;
	sem_t clientToCashier;

	// variables for the client cashier interaction
	int serviceTime;
	int itemId;
	int clientId;
	
	// semaphoes for the client server interaction
	sem_t clientToServer;
	sem_t serverToClient;
	sem_t serve;

	// variables for the client server interaction
	int orderNo;
	
	// to access database
	sem_t database;
	
	Item menu[20];
	
	// signal close 
	bool isClosed;
};
