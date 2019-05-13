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

void store(char* file, char* path);
void append(char* file, char* path);
void extract(char* file, char* path, int num);
void meta(char* file, char* path);
void print(char* file, char* path);
