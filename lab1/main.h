#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <bits/stdc++.h>
#include <unistd.h>
//#include <iostream>
//#include <fstream>

#define PATH_SIZE 100 // 100 is an arbitrary number

extern std::map<std::string, std::string> paths;
extern std::string input;
extern std::list<std::string> history;
extern std::fstream file;

std::vector<std::string> get_words(std::string input);

void do_export(std::vector<std::string> words);

void do_pwd();

void do_history(std::list<std::string> history);

void do_cd(std::vector<std::string> words);

//std::list<std::string> make_history(std::fstream file);

