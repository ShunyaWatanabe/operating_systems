#include "main.h"
#include "built_in.h" // functions for built in commands
#include "external.h"
#include "history.h"
using namespace std;

vector<string> split(string input, const char* delimitter){
	// split line into words
	vector<string> words;
	char char_array[PATH_SIZE];
	strcpy(char_array, input.c_str());
	char* token = strtok(char_array, delimitter);
	while (token != NULL){
		words.push_back(token);
		token = strtok(NULL, delimitter);
	}
	return words;
}

void perform_action(string word, list<string>&history, vector<string>& words, map<string, string> &paths){
	// perform action
	if (word == "exit") do_exit(history);
	else if (word == "history") do_history(history);
	else if (word == "pwd") do_pwd();
	else if (word == "export") do_export(words, paths);
	else if (word == "cd") do_cd(words);
	else do_external(words, paths.at("PATH"));
}

void perform_action2(string word, list<string>&history, vector<string>& words, map<string, string> &paths){
	// perform action
	if (word == "exit") do_exit(history);
	else if (word == "history") do_history(history);
	else if (word == "pwd") do_pwd();
	else if (word == "export") do_export(words, paths);
	else if (word == "cd") do_cd(words);
	else do_external2(words, paths.at("PATH"));
}

int main(){

	// initializatoin process
	list<string> history = make_history();

	map<string, string> paths;
	paths.insert({"PATH", ""});

	string input = "";

	// accept commands
	while (1){
		cout << ">>";
		getline(cin, input);
		execute_history(history, &input);
		append_history(history, input);
		
		vector<string> commands = split(input, "|");

		if (commands.size() == 1){
			vector<string> words = split(commands.at(0), " ");	
				perform_action(words.at(0), history, words, paths);
			continue;
		}

		int numPipes = commands.size()-1;
		int pipefd[numPipes*2];
		for (int i=0; i<numPipes; i++){
			if (pipe(pipefd+i*2)<0){
				cerr<<"error piping"<<endl;
				exit(0);		
			}
		}
		for (int i=0; i<commands.size(); i++){
			vector<string> words = split(commands.at(i), " ");
			int in = i*2;
			int out = i*2+1;
			if (words.size() == 0){
				exit(0);
			}
			int status;
			int pid = fork();
			if (pid < 0){
				cerr<<"error forking"<<endl;
				exit(0);
			}
			if (pid == 0){
				if (i==0){
					// first child. only sending
					dup2(pipefd[out], STDOUT_FILENO);
				}
				else if (i==commands.size()-1){
					// last child. only receiving
					dup2(pipefd[in-2], STDIN_FILENO);
				}
				else {
					// intermediary child. sending and receiving
					dup2(pipefd[out], STDOUT_FILENO);
					dup2(pipefd[in-2], STDIN_FILENO);
				}
				for (int j=0; j<numPipes; j++){
					close(pipefd[j*2]);
					close(pipefd[j*2+1]);
				}
				perform_action2(words.at(0), history, words, paths);
				exit(0);
			}
			// parent. wait for child process to die
			if (i != 0){
				close(pipefd[in-2]);
				close(pipefd[out-2]);
			}
			waitpid(pid, &status, 0);
		}
	/*	
		for (int i=0; i<commands.size(); i++){
			close(pipefd[i*2]);
			close(pipefd[i*2+1]);
		}
		*/
	}
	return 0;
}
