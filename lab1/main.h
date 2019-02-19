#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <bits/stdc++.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdexcept>

#define PATH_SIZE 100 // 100 is an arbitrary number
#define F_OK 0 // file exists

std::vector<std::string> get_words(std::string input);

std::list<std::string> make_history(std::fstream file);

std::string search(std::list<std::string> history, int num);
