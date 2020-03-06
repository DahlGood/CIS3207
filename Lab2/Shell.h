#include <string.h>
#include <iostream>
#include <unistd.h>
#include <vector>


using namespace std;

enum error_codes {
    INVALID_COMMAND = 0,
};

//Command Parser
void parseInput(string rawCommand);
bool containsBashCommand(char* command_token);
void addBashCommand(char* command, vector<char*> *bashCommandsIncluded);
vector<char*> getBashCommand();

//Command Processor
void processCommand(vector<vector<char*> * > parsed_input);
int isBuiltIn(string command);
void chooseDir(char* argument);
void builtInRedirection(vector<char*> bashCommandsIncluded, vector<vector<char*> * > parsed_input);
void processBuiltIn(vector<vector<char*> * > parsed_input);

//Some helper commands defined in Shell.cpp
void getUsername(char* username);

//Built In Commands.
void cd(char* path);
void clr();
void dir();
void dir(char* path);
void myenviron();
void myecho(char* arg);
void mypause();
void myquit();
void help();

void printStatus();

