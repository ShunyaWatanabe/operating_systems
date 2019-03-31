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

void perform_pipe(vector<string>commands, list<string>& history, map<string, string>& paths){
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
			perform_action(words.at(0), history, words, paths);
			exit(0);
		}
		// parent. close pipe and wait for child process to die
		if (i != 0){
			close(pipefd[in-2]);
			close(pipefd[out-2]);
		}
		waitpid(pid, &status, 0);
	}
}

void perform(string input, list<string>history, map<string, string>& paths){
	// 1: basic operation
	if (input.find("|") == -1) {
		vector<string> words = split(input, " ");
		perform_action(words.at(0), history, words, paths);
	}
	// 2: pipe operation 
	else {
		vector<string> commands = split(input, "|");
		perform_pipe(commands, history, paths);
	}
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


		// output redirection 
		if (input.find(">") != -1){
			cout<<"detedcted >"<<endl;
			vector<string> IOs = split(input, ">");
			if (IOs.size() != 2){
				cerr<<"wrong format for operator >."<<endl;
				continue;
			}
			string input = IOs.at(0);
			string filename = IOs.at(1);
			filename.erase(remove(filename.begin(), filename.end(), ' '), filename.end());
			cout <<"input: "<< input << endl;
			cout <<"filename: "<< filename << endl;
			int pipefd[2];
			if (pipe(pipefd) == -1){
				cerr<<"error piping"<<endl;
				exit(0);
			}
			int pid = fork();
			if (pid < 0){
				cerr<<"error forking"<<endl;
			}
			if (pid == 0){
				// child process. perform and write to pipe
				close(pipefd[0]);
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);

				perform(input, history, paths);
				exit(0);
			}
			else {
				int pid = fork();
				if (pid < 0){
					cerr<<"error forking"<<endl;
				}
				if (pid == 0){
					// child process two. receive output
					close(pipefd[1]);
					dup2(pipefd[0], STDIN_FILENO);
					close(pipefd[0]);
					wait(NULL);

					// write to file
					char data[255];
					memset(data, 0, 255);
					ofstream file;
					file.open(filename);
					read(STDIN_FILENO, data, 255);
					file << data;
					file.close();
					exit(0);
				}
				// parent process
				close(pipefd[0]);
				close(pipefd[1]);
				wait(NULL);
				wait(NULL);
			}
		}
		// input redirection
		else if (input.find("<") != -1){
			cout<<"detedcted <"<<endl;

		}
		// no IO redirection
		else {
			cout<<"nothing detedcted"<<endl;
			perform(input, history, paths);	
		}
	}
	return 0;
}
