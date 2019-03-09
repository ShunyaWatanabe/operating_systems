#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdexcept>
#include <sys/types.h>
#include <sys/wait.h>

void get_actual_paths(std::vector<std::string>&actual_paths, std::string paths);
char* convert(const std::string &s);
void do_external(std::vector<std::string> words, std::string path);

