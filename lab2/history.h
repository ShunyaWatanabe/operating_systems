#include <iostream>
#include <string>
#include <list>
#include <bits/stdc++.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

std::list<std::string> make_history();
void append_history(std::list<std::string>& history, std::string input);
std::string search(std::list<std::string> history, int num);
void execute_history(std::list<std::string> history, std::string* input);
