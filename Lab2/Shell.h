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
int containsSpecial(char* command_token);

//Command Processor
void processCommand(vector<vector<char*> * > parsed_input);

//Some helper commands defined in Shell.cpp
void getUsername(char* username);

//Built In Commands.
void cd(char* path);
void clr();
//void dir();
//void dir(string path);
//void environ();
//void echo(string arg);
//void help();
//void pause();
//void quit();

void printStatus();
