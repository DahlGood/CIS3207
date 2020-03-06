#include "Shell.h"
#include<bits/stdc++.h> 
//Needed for open()
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


void processCommand(vector<vector<char*> * > parsed_input) {

    /*
        Check if command is built in
        If command contains redirection
            run builtInRedirection
        Else
            Run command like normal in switch case.



        If command is not built in
        If command contains redirection
            run redirection
        If command contains piping
            run piping
        Else
            Run generalExec
    */

    //Since we're only worrying about single redirection / pipes for the most part, we only have to check the first command.
    int position = isBuiltIn(parsed_input.at(0)->at(0));
    if(position < 8) {
        //If a bash command was entered.
        if(getBashCommand().size() != 0) {
            //Pass builtInRedirection a list of all bash commands used, the output file specified, 
            builtInRedirection(getBashCommand(), parsed_input);
        }
        else {
            processBuiltIn(parsed_input);
        }
    }
    else {

    }
    /*
    for(unsigned int i = 0; i < parsed_input.size(); i+=2) {
        cout << parsed_input.at(i)->at(0) << endl;
        cout << isBuiltIn(parsed_input.at(i)->at(0)) << endl;
        switch (isBuiltIn(parsed_input.at(i)->at(0))) {
            case 0:
                cd(parsed_input.at(i)->at(1));
                break;
            case 1:
                clr();
                break;
            case 2:
                chooseDir(parsed_input.at(i)->at(1));
                break;
            case 3:
                myenviron();
                break;
            case 4:
                try
                {
                    myecho(parsed_input.at(i)->at(1));
                }
                catch(int x)
                {
                    cout << "Echo requires an argument to be entered. Ex: echo Hello" << endl;
                } 
                break;
            case 5:
                mypause();
                break;
            case 6:
                myquit();
                break;
            case 7:
                help();
                break;
                
            default:
                cout << "Default" << endl;
                break;
        }
    }
    */


}

int isBuiltIn(string command) {
    vector<string> built_ins{"cd", "clr", "dir", "environ", "echo", "pause", "quit", "help"};
    
    unsigned int position = find(built_ins.begin(), built_ins.end(), command) - built_ins.begin();

    if(position <= (unsigned)built_ins.size()) {
        return position;
    }
    
    return -1;

}

void chooseDir(char* argument) {
    if(argument != NULL) {
        dir(argument);
    }
    else {
        dir();
    }
}


//Project specifications say this only needs to handle output redirection and only for 4 built in commands.
void builtInRedirection(vector<char*> bashCommandsIncluded, vector<vector<char*> * > parsed_input) {
    
    int file;

    if(strcmp(bashCommandsIncluded.at(0), ">") == 0) {
        file = open(parsed_input.at(2)->at(0), O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    }
    else if(strcmp(bashCommandsIncluded.at(0), ">>") == 0) {
        file = open(parsed_input.at(2)->at(0), O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    }
    else {
        cout << "Built in commands only support > and >> redirection." << endl;
        return;
    }

    //Make file look like stdout.
    //Saving stdout.
    int stdout_saved = dup(STDOUT_FILENO);
    dup2(file, STDOUT_FILENO);
    close(file);
    processBuiltIn(parsed_input);
    dup2(stdout_saved, STDOUT_FILENO);
    close(stdout_saved);
    //dup2(STDOUT_FILENO, file);
    return;

}

void processBuiltIn(vector<vector<char*> * > parsed_input) {

    switch (isBuiltIn(parsed_input.at(0)->at(0))) {
            case 0:
                cd(parsed_input.at(0)->at(1));
                break;
            case 1:
                clr();
                break;
            case 2:
                chooseDir(parsed_input.at(0)->at(1));
                break;
            case 3:
                myenviron();
                break;
            case 4:
                try
                {
                    myecho(parsed_input.at(0)->at(1));
                }
                catch(int x)
                {
                    cout << "Echo requires an argument to be entered. Ex: echo Hello" << endl;
                } 
                break;
            case 5:
                mypause();
                break;
            case 6:
                myquit();
                break;
            case 7:
                help();
                break;
                
            default:
                cout << "Default" << endl;
                break;
        }

    return;
}
/*

void externalRedirection() {
    
}

void externalPiping() {

}

void runExternal() {

}
*/
