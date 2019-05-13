#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <vector>
#include <fcntl.h>
#include <unistd.h>

void iterate(std::vector<std::string>* paths, char* path);
void store(char* file, char* path);
