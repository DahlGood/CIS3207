#include "Shell.h"

/*
    This parser ensures the user input will always be in the following order:
    |       0     |        1     |      2      | .... |      n      |
    | command arg | Bash Command | command arg | .... | command arg | 

    ex:
        {
            {ls, NULL},        <----- 0, command
            {|, NULL},         <----- 1, bash command
            {grep, item, NULL} <----- 2, command arg
        }

    This allows us to easily account for multiple redirections or pipes in the future.
*/

//List of bash commands entered by the user.
vector<char*> bashCommandsIncluded;

void parseInput(string rawCommand) {

    //Vector of  vector pointers
    vector<vector<char*> * > parsed_input;

    //Converting from string to char* in order to easily tokenize.
    char command[rawCommand.size() + 1];
    strcpy(command, rawCommand.c_str());

    //If the user doesnt enter anything and just hits enter, go back to getting user input.
    if(command[0] == '\0') {
        printStatus();
        return;
    }

    //Tokenizing the raw input by spaces.
    char* command_token = strtok(command, " ");
    
    //Check to make sure the first argument of user input isnt an illegal character.
    if(containsBashCommand(command_token)) {
        //Might need to include an else.
        throw(INVALID_COMMAND);
    }

    //If everything has gone fine so far, add the first element to the vector.
    parsed_input.push_back(new vector<char*>);

    
    int i = 0;
    while(command_token) {
        if(containsBashCommand(command_token)) {

            addBashCommand(command_token, &bashCommandsIncluded);

            //Ensuring the command group before this symbol appeared is NULL terminated.
            parsed_input.at(i)->push_back(NULL);
            
            //Incrementing because this special command cant exist in the same command group as other commands and arguments.
            i++;
            
            //Creating a new command group within our parsed vector.
            parsed_input.push_back(new vector<char*>);
            //Adding the commands / argument to the command group.
            parsed_input.at(i)->push_back(command_token);
            parsed_input.at(i)->push_back(NULL);

            //Two of these special commands cant exist back-to-back so incrementing again to get to a new command group.
            i++;
        }
        else {
            //If we're in the first command group we know we dont have a special character, so we can add to the current command group.
            //Or if the last command isnt special then we can also add to the current command group.
            if(i == 0 || containsBashCommand(parsed_input.at(i-1)->at(0)) == 0) {
                parsed_input.at(i)->push_back(command_token);
            }
            else {
                //If the previous command was special, then create a new command group.
                parsed_input.push_back(new vector<char*>);
                parsed_input.at(i)->push_back(command_token);
            }
            
        }

        //Tokenize the input again to get to the next item in the raw input.
        command_token = strtok(NULL, " ");

    }

    //Ensures the last command group will be NULL terminated.
    parsed_input.at(i)->push_back(NULL);

    processCommand(parsed_input);

    parsed_input.clear();
    bashCommandsIncluded.clear();

    printStatus();

}

bool containsBashCommand(char* command_token) {
    string shell_commands[6] = {"<","<<",">",">>","|","&"};
    for(auto x : shell_commands) {
        if(command_token == x) {
            return true;
        }
    }
    return false;
}

void addBashCommand(char* command, vector<char*> *bashCommandsIncluded) {
    bashCommandsIncluded->push_back(command);
    return;
}

vector<char*> getBashCommand() {
    return bashCommandsIncluded;
}