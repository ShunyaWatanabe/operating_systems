#include <bits/stdc++.h> 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/shm.h>
#define NUMARGS 7

class Client
{ 
	// Access specifier 
	public: 

	// Data Members 
	int itemId;
	int eatTime;
	int shmid;

	Client(int iId, int eTime, int id){
		itemId = iId;
		eatTime = eTime;
		shmid = id;
	}

	// Member Functions() 
	void function() { 
		std::cout << "itemId: "<<itemId<<std::endl;
		std::cout << "eatTime: "<<eatTime<<std::endl;
		std::cout << "shmid: "<<shmid<<std::endl;
	} 
}; 

