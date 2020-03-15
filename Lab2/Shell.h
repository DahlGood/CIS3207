#include <string.h>
#include <iostream>
#include <unistd.h>
#include <vector>


using namespace std;

//Command Parser
void parseInput(string rawCommand);
bool containsBashCommand(char* command_token);
void addBashCommand(char* command, vector<char*> *bashCommandsIncluded);
vector<char*> getBashCommand();

//Command Processor
void processCommand(vector<vector<char*> * > parsed_input);
int isBuiltIn(string command);
void chooseDir(char* argument);
void chooseHelp(char* argument);
void builtInRedirection(vector<char*> bashCommandsIncluded, vector<vector<char*> * > parsed_input);
void processBuiltIn(vector<vector<char*> * > parsed_input);
void externalRedirection(vector<char*> bashCommandsIncluded, vector<vector<char*> * > parsed_input);
void externalPiping(vector<vector<char*> * > parsed_input);
void externalBE(vector<vector<char*> * > parsed_input);
void processExternal(vector<vector<char*> * > parsed_input);

//Some helper commands defined in Shell.cpp
void getUsername(char* username);

//Built In Commands.
void cd(char* path);
void clr();
void dir();
void dir(char* path);
void myenviron();
void myecho(vector<char *> cmdGrpOne);
void mypause();
void myquit();
void help();
void help(char* command);

void printStatus();