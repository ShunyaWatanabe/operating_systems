#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "store.h"
#include "append.h"
#include "extract.h"
#include "meta.h"
#include "display.h"
using namespace std;

int main(int argc, char* argv[]){
	if (strcmp(argv[1], "-c") == 0){
		store(argv[2], argv[3]);
	}
	else if (strcmp(argv[1], "-a") == 0){
		append(argv[2], argv[3]);
	}
	else if (strcmp(argv[1], "-x") == 0){
		if (argc == 6){
			extract(argv[2], argv[3], atoi(argv[5]));
		}
		else {
			extract(argv[2], argv[3], -1);
		}
	}
	else if (strcmp(argv[1], "-m") == 0){
		meta(argv[2], argv[3]);
	}
	else if (strcmp(argv[1], "-p") == 0){
		display(argv[2], argv[3]);
	}
		
	return 0;
}
