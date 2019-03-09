#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <unistd.h>
#include <bits/stdc++.h>
#define PATH_SIZE 100

void do_export(std::vector<std::string> words, std::map<std::string, std::string>& paths);

void do_pwd();

void do_history(std::list<std::string> history);

void do_cd(std::vector<std::string> words);

void do_exit(std::list<std::string> history);

