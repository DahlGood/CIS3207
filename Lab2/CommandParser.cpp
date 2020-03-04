#include "Shell.h"


void parseInput(string rawCommand) {

    vector<vector<char*> * > parsed_input;

    //Converting from string to char* in order to easily tokenize.
    char command[rawCommand.size() + 1];
    strcpy(command, rawCommand.c_str());

    //Tokenizing the raw input by spaces.
    char* command_token = strtok(command, " ");
    //Check to make sure the first argument of user input isnt an illegal character.
    if(containsSpecial(command_token)) {
        //Might need to include an else.
        throw(INVALID_COMMAND);
    }

    

    //If everything has gone fine so far, add the first element to the vector.
    parsed_input.push_back(new vector<char*>);
    
    /* Example of the vector structure.
    {
        {
            wow, hello
        },
        {
            |
        },
        {
            yeet
        }

    }
    */
    
    int i = 0;
    
    while(command_token) {
        if(containsSpecial(command_token)) {
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
            if(i == 0 || containsSpecial(parsed_input.at(i-1)->at(0)) == 0) {
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
    /*
    cout << "START" << endl;
    cout << parsed_input.at(0)->at(0) << endl;
    cout << parsed_input.at(0)->at(1) << endl;
    cout << parsed_input.at(1)->at(0) << endl;
    cout << parsed_input.at(2)->at(0) << endl;
    cout << "END" << endl;
    cout << parsed_input.size() << endl;
    cout << parsed_input.at(2)->size() << endl;
    */
    for(unsigned int a = 0; a < parsed_input.size(); a++) {
        for(unsigned int b = 0; b < parsed_input.at(a)->size()-1; b++) {
            cout << parsed_input.at(a)->at(b) << endl;
        }
    }

    //processCommand(parsed_input);

}

int containsSpecial(char* command_token) {
    string shell_commands[6] = {"<","<<",">",">>","|","&"};
    for(auto x : shell_commands) {
        if(command_token == x) {
            return 1;
        }
    }
    return 0;
}