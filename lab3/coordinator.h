#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "stats.h"
#include <unistd.h>
#define MEMORY_SIZE 4096
#define MAX_PEOPLE 50
#define MAX_QUEUE_SIZE 10
