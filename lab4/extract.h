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
#include <bits/stdc++.h> 

#define SMALL_BUFF 100
#define LARGE_BUFF 2048
void extract(char* file, char* path, int num);
