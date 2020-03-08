#include "Shell.h"
#include<bits/stdc++.h> 
//Needed for open()
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
//Needed for piping
#include <sys/wait.h>

void processCommand(vector<vector<char*> * > parsed_input) {
    
    //Since we're only worrying about single redirection / pipes for the most part, we only have to check the first command.
    int position = isBuiltIn(parsed_input.at(0)->at(0));
    
    //Internal commands
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
    //External Commands
    else {
        //Are bash commands used.
        if(getBashCommand().size() != 0) {
            if(strcmp(getBashCommand().at(0), "|") == 0) {
                externalPiping(parsed_input);
            }
            else if(strcmp(getBashCommand().at(0), "&") == 0) {
                externalBE(parsed_input);
            }
            else {
                externalRedirection(getBashCommand(), parsed_input);
            }
        }
        //No bash commands used.
        else {
            processExternal(parsed_input);
        }
    }

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

void externalRedirection(vector<char*> bashCommandsIncluded, vector<vector<char*> * > parsed_input) {
    
    if(!validCommand(parsed_input.at(0)->at(0))) {
        return;
    }
    

    char** command = parsed_input.at(0)->data();
    int fileOne;
    int fileTwo;

    if(bashCommandsIncluded.size() == 2) {
        if(strcmp(bashCommandsIncluded.at(1), ">") == 0) {
            fileOne = open(parsed_input.at(2)->at(0), O_RDONLY, S_IRUSR | S_IRGRP);
            fileTwo = open(parsed_input.at(4)->at(0), O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
        }
        else if(strcmp(bashCommandsIncluded.at(1), ">>") == 0) {
            fileOne = open(parsed_input.at(2)->at(0), O_RDONLY, S_IRUSR | S_IRGRP);
            fileTwo = open(parsed_input.at(4)->at(0), O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
        }
        else {
            cout << "Error: Check your input for possible mistakes. Expected input should look like: command < fileOne > fileTwo." << endl;
        }

        int stdin_saved = dup(STDIN_FILENO);
        int stdout_saved = dup(STDOUT_FILENO);

        dup2(fileOne, STDIN_FILENO);
        dup2(fileTwo, STDOUT_FILENO);

        pid_t process = fork();
        if(process == -1) {
            cout << "Error: problem creaing process." << endl;
        }
        if(process == 0) {
            close(fileOne);
            close(fileTwo);
            execvp(command[0], command);
        }
        else {
            dup2(stdin_saved, STDIN_FILENO);
            dup2(stdout_saved, STDOUT_FILENO);
            close(stdin_saved);
            close(stdout_saved);
            waitpid(-1, NULL, 0);
        }

    }
    else {

        int stdin_saved = dup(STDIN_FILENO);
        int stdout_saved = dup(STDOUT_FILENO);

        if(strcmp(bashCommandsIncluded.at(0), "<") == 0) {
            fileOne = open(parsed_input.at(2)->at(0), O_RDONLY, S_IRUSR | S_IRGRP);
            dup2(fileOne, STDIN_FILENO);
        }
        else if(strcmp(bashCommandsIncluded.at(0), ">") == 0) {
            fileOne = open(parsed_input.at(2)->at(0), O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
            dup2(fileOne, STDOUT_FILENO);
        }
        else if(strcmp(bashCommandsIncluded.at(0), ">>") == 0) {
            fileOne = open(parsed_input.at(2)->at(0), O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
            dup2(fileOne, STDOUT_FILENO);
        }
        else {
            cout << "Error: Check your input for possible mistakes. Expected input should look like: command < fileOne  OR command > fileOne." << endl;
        }


        pid_t process = fork();
        if(process == -1) {
            cout << "Error: problem creaing process." << endl;
        }
        if(process == 0) {
            close(fileOne);
            execvp(command[0], command);
        }
        else {
            dup2(stdin_saved, STDIN_FILENO);
            dup2(stdout_saved, STDOUT_FILENO);
            close(stdin_saved);
            close(stdout_saved);
            waitpid(-1, NULL, 0);
        }

    }


    
}

void externalPiping(vector<vector<char*> * > parsed_input) {
    
    if(!validCommand(parsed_input.at(0)->at(0))) {
        return;
    }
    else if(!validCommand(parsed_input.at(2)->at(0))) {
        return;
    }
    
    
    char** commandOne = parsed_input.at(0)->data();
    char** commandTwo = parsed_input.at(2)->data();

    int fd[2];
    if(pipe(fd) != 0) {
        cout << "Error: Couldn't create pipe." << endl;
    }

    pid_t processOne = fork();
    if(processOne == -1) {
        cout << "Error: Couldn't create process one." << endl;
    }
    else if(processOne == 0) {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        execvp(commandOne[0], commandOne);
    }
    else {
        waitpid(0, NULL, 0);
    }

    pid_t processTwo = fork();
    if(processTwo == -1) {
        cout << "Error: Couldn't create process two." << endl;
    }
    else if(processTwo == 0) {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        execvp(commandTwo[0], commandTwo);
    }
    else {
        //Wait until this specific process is finished.
        waitpid(2, NULL, 0);
    }

    close(fd[0]);
    close(fd[1]);

    return;

}

void externalBE(vector<vector<char*> * > parsed_input) {
    
    if(!validCommand(parsed_input.at(0)->at(0))) {
        return;
    }
    else if(!validCommand(parsed_input.at(2)->at(0))) {
        return;
    }
    

    char** commandOne = parsed_input.at(0)->data();
    char** commandTwo = parsed_input.at(2)->data();

    //int fd[2];
    //pipe(fd);

    pid_t processOne = fork();
    if(processOne == -1) {
        cout << "Error: Couldn't create process one." << endl;
    }
    else if(processOne == 0) {
        
        execvp(commandOne[0], commandOne);
    }
    else {
        //close(fd[0]);
        //dup2(fd[1], STDIN_FILENO);
        //waitpid(-1, NULL, 0);
    }

    pid_t processTwo = fork();
    if(processTwo == -1) {
        cout << "Error: Couldn't create process two." << endl;
    }
    else if(processTwo == 0) {
        execvp(commandTwo[0], commandTwo);
    }
    else {
        waitpid(-1, NULL, 0);
    }

    return;

}

void processExternal(vector<vector<char*> * > parsed_input) {
    
    if(!validCommand(parsed_input.at(0)->at(0))) {
        cout << "NOT VALID" << endl;
        return;
    }
    cout << "made it here" << endl;
    char** command = parsed_input.at(0)->data();
    
    pid_t process = fork();
    if(process == -1) {
        cout << "Error: couldnt fork." << endl;
        return;
    }
    else if(process == 0) {
        execvp(command[0], command);
    }
    else {
        waitpid(-1, NULL, 0);
    }

    return;
}

bool validCommand(char* command) {
    bool valid = false;
    for(auto x : getEnvironPaths()) {
        string absoluteCommandPath = x;
        absoluteCommandPath += "/";
        absoluteCommandPath += command;
        
        if(access(absoluteCommandPath.c_str(), X_OK) == 0) {
            cout << "Yes, because path variables." << endl;
            valid = true;
            return true;
        }
        else if(access(command, X_OK) == 0) {
            cout << "Yes, because current dir." << endl;
            valid = true;
            return true;
        }
    }

    cout << "Error " << command << " is not a valid command." << endl;

    return valid;
}