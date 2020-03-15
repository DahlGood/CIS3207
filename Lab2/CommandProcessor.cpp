#include "Shell.h"
#include<bits/stdc++.h> 
//Needed for open()
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
//Needed for piping
#include <sys/wait.h>


void processCommand(vector<vector<char*> * > parsed_input) {
    //If the command is a built in but we want to use piping we should just use the external version of the command. For example echo.
    //So to do this, Check if the bash command vecotor has content in it. If it does then check if it's a pipe. If it is, go straight to piping. Otherwise continue like normal.
    if(getBashCommand().size() > 0) {
        if(strcmp(getBashCommand().at(0), "|") == 0) {
            externalPiping(parsed_input);
            return;
        }
    }
    
    //Since we're only worrying about single redirection / pipes for the most part (except in the case mentioned above), we only have to check the first command.
    //Using the vector index position returned by isBuiltIn to check if the command is a built in command.
    int position = isBuiltIn(parsed_input.at(0)->at(0));
    
    //Internal commands
    //If its a built in command (aka if the vector index returned is less than the total size of the vector) then process the command as a built in.
    if(position < 8) {
        //If a bash command was entered.
        if(getBashCommand().size() != 0) {
            //Pass builtInRedirection a list of all bash commands used, the output file specified, 
            builtInRedirection(getBashCommand(), parsed_input);
        }
        else {
            //Process the command as a built in.
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

    //Vector of all built in commands.
    vector<string> built_ins{"cd", "clr", "dir", "environ", "echo", "pause", "quit", "help"};

    //Vector index if found, size of vector if not.
    unsigned int position = find(built_ins.begin(), built_ins.end(), command) - built_ins.begin();
    
    return position;

}

//There are two options for the dir command. dir and dir Directory. This determines what one to use.
void chooseDir(char* argument) {
    if(argument != NULL) {
        dir(argument);
    }
    else {
        dir();
    }
}

//There are two options for the help command. help and help Command. This determines what one to use.
void chooseHelp(char* argument) {
    if(argument != NULL) {
        help(argument);
    }
    else {
        help();
    }
}


//Project specifications say this only needs to handle output redirection and only for 4 built in commands.
void builtInRedirection(vector<char*> bashCommandsIncluded, vector<vector<char*> * > parsed_input) {
    
    //Declaring a file descriptior.
    int file;

    //Determining how to open the file depending on the bach command used.
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

    if(file == -1) {
        close(file);
        cout << "Error: Couldn't open " << parsed_input.at(2)->at(0) << endl;
        return;
    }

    //Make file look like stdout.
    //Saving stdout.
    int stdout_saved = dup(STDOUT_FILENO);
    if(stdout_saved == -1) {
        cout << "Error: Problem saving stdout file discriptor." << endl;
    }
    
    //Making the file look like stdout.
    if(dup2(file, STDOUT_FILENO) == -1) {
        cout << "Error: Problem duping file to stdout." << endl;
    }
    close(file);

    //Running the command.
    processBuiltIn(parsed_input);

    //Restoring the original file descriptor so the program can continue normally.
    if(dup2(stdout_saved, STDOUT_FILENO) == -1) {
        cout << "Error: Problem restoring stdout." << endl;
    }
    close(stdout_saved);
    
    return;

}

//Uses the vector index returned to choose what command to run.
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
                    myecho(*parsed_input.at(0));
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
                chooseHelp(parsed_input.at(0)->at(1));
                break;
            default:
                cout << "Default" << endl;
                break;
        }

    return;
}

void externalRedirection(vector<char*> bashCommandsIncluded, vector<vector<char*> * > parsed_input) {
    
    //Converts the vector command group to an array. Needed to run exec.
    char** execProcess = parsed_input.at(0)->data();
    
    //Declaring file descriptors.
    int fileOne;
    int fileTwo;

    //Determining how to open the files depending on the bach command(s) used.
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

        if(fileOne == -1) {
            close(fileOne);
            close(fileTwo);
            cout << "Error: Couldn't open " << parsed_input.at(2)->at(0) << endl;
            return;
        }

        //Saving the file descriptors.
        int stdin_saved = dup(STDIN_FILENO);
        int stdout_saved = dup(STDOUT_FILENO);

        if(stdin_saved == -1 || stdout_saved == -1) {
            cout << "Error: Problem restoring stdin or stdout." << endl;
            return;
        }

        //Setting the appropriate file descriptors.
        //Making fileOne look like stdin.
        int fileOneDup2 = dup2(fileOne, STDIN_FILENO);
        //Making fileTwo look like stdout.
        int fileTwoDup2 = dup2(fileTwo, STDOUT_FILENO);

        if(fileOneDup2 == -1 || fileTwoDup2 == -1) {
            cout << "Problem duping fileOne or fileTwo" << endl;
            return;
        }

        //Executing the commands requested.
        pid_t process = fork();
        if(process == -1) {
            cout << "Error: Problem creating process." << endl;
        }
        if(process == 0) {
            close(fileOne);
            close(fileTwo);
            
            if(execvp(execProcess[0], execProcess) == -1) {
                cout << "Error: Problem executing " << parsed_input.at(0)->at(0) << endl;
            }
        }
        else {

            //Waiting till ANY child from its process group has finished.
            waitpid(-1, NULL, 0);

            //Restoring file descriptors
            int restoredstdin = dup2(stdin_saved, STDIN_FILENO);
            int restoredstdout = dup2(stdout_saved, STDOUT_FILENO);

            if(restoredstdin == -1 || restoredstdout == -1) {
                cout << "Problem restoring stdin or stdout." << endl;
                return;
            }

            close(stdin_saved);
            close(stdout_saved);
        }

    }
    else {

        //Saving file descriptors.
        int stdin_saved = dup(STDIN_FILENO);
        int stdout_saved = dup(STDOUT_FILENO);

        if(stdin_saved == -1 || stdout_saved == -1) {
            cout << "Problem saving stdin or stdout." << endl;
            return;
        }

        //Setting the appropriate file descriptors.
        if(strcmp(bashCommandsIncluded.at(0), "<") == 0) {
            fileOne = open(parsed_input.at(2)->at(0), O_RDONLY, S_IRUSR | S_IRGRP);
            if(fileOne == -1) {
                close(fileOne);
                cout << "Error: Couldn't open " << parsed_input.at(2)->at(0) << endl;
                return;
            }
            else {
                //Making fileOne look like stdin.
                if(dup2(fileOne, STDIN_FILENO) == -1) {
                    cout << "Error: Problem making fileOne look like stdin." << endl;
                }
            }
        }
        else if(strcmp(bashCommandsIncluded.at(0), ">") == 0) {
            fileOne = open(parsed_input.at(2)->at(0), O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
            //Making fileTwo look like stdout.
            if(dup2(fileOne, STDOUT_FILENO) == -1) {
                cout << "Error: Problem making fileOne look like stdout." << endl;
                return;
            }
        }
        else if(strcmp(bashCommandsIncluded.at(0), ">>") == 0) {
            fileOne = open(parsed_input.at(2)->at(0), O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
            //Making fileOne look like stdout. 
            if(dup2(fileOne, STDOUT_FILENO) == -1) {
                cout << "Error: Problem making fileOne look like stdout." << endl;
                return;
            }
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
            if(execvp(execProcess[0], execProcess) == -1) {
                cout << "Error: Problem executing " << parsed_input.at(2)->at(0) << endl;
            }
        }
        else {

            //Waiting till ANY child from its process group has finished.
            waitpid(-1, NULL, 0);

            
            //Restoring file descriptors
            int savedstdin = dup2(stdin_saved, STDIN_FILENO);
            int savedstdout = dup2(stdout_saved, STDOUT_FILENO);

            if(savedstdin == -1 || savedstdout == -1) {
                cout << "Error: Problem restoring stdin or stdout." << endl;
                return;
            }
            close(stdin_saved);
            close(stdout_saved);

        }

    }
    
}

void externalPiping(vector<vector<char*> * > parsed_input) {
    
    //Converts the vector command group to an array. Needed to run exec.
    char** execOne = parsed_input.at(0)->data();
    char** execTwo = parsed_input.at(2)->data();

    //Creating a pipe.
    int fd[2];
    if(pipe(fd) != 0) {
        cout << "Error: Couldn't create pipe." << endl;
    }

    //Executing the commands requested.
    pid_t processOne = fork();
    if(processOne == -1) {
        cout << "Error: Couldn't create process one." << endl;
    }
    else if(processOne == 0) {
        //Closing the pipes stdin
        close(fd[0]);

        //Making the pipes stdout look like stdout.
        if(dup2(fd[1], STDOUT_FILENO) == -1) {
            cout << "Error: Problem making pipe stdout look like stdout." << endl;
            return;
        }
        close(fd[1]);

        //Executing the command requested.
        if(execvp(execOne[0], execOne) == -1) {
            cout << "Error: Problem executing " << parsed_input.at(0)->at(0) << endl;
        }
    }
    else {
        //Waiting till ANY child from its process group has finished.
        waitpid(-1, NULL, 0);
    }

    pid_t processTwo = fork();
    if(processTwo == -1) {
        cout << "Error: Couldn't create process two." << endl;
    }
    else if(processTwo == 0) {
        //Closing the pipes stdout.
        close(fd[1]);
        //Making the pipes stdin look like stdin.
        if(dup2(fd[0], STDIN_FILENO) == -1) {
            cout << "Error: Problem making pipe stdin look like stdin" << endl;
            return;
        }
        close(fd[0]);

        if(execvp(execTwo[0], execTwo) == -1) {
            cout << "Error: Problem executing " << parsed_input.at(2)->at(0) << endl;
        }
    }
    else {
        //Closing the pipe
        close(fd[0]);
        close(fd[1]);

        //Waiting till ANY child from its process group has finished.
        waitpid(-1, NULL, 0);
    }

    return;

}

void externalBE(vector<vector<char*> * > parsed_input) {

    //Converts the vector command group to an array. Needed to run exec.
    char** execOne = parsed_input.at(0)->data();
    char** execTwo = parsed_input.at(2)->data();

    //Executing the commands requested.
    pid_t processOne = fork();
    if(processOne == -1) {
        cout << "Error: Couldn't create process one." << endl;
    }
    else if(processOne == 0) {
        if(execvp(execOne[0], execOne) == -1) {
            cout << "Error: Problem executing " << parsed_input.at(0)->at(0) << endl;
        }
    }
    else {

        //Waiting till ANY child from its process group has finished.
        waitpid(-1, NULL, 0);
    }

    pid_t processTwo = fork();
    if(processTwo == -1) {
        cout << "Error: Couldn't create process two." << endl;
    }
    else if(processTwo == 0) {
        if(execvp(execTwo[0], execTwo) == -1) {
            cout << "Error: Problem executing " << parsed_input.at(2)->at(0) << endl;
        }
    }
    else {

        //Waiting till ANY child from its process group has finished.
        waitpid(-1, NULL, 0);
    }

    return;

}

void processExternal(vector<vector<char*> * > parsed_input) {
    //Converts the vector command group to an array. Needed to run exec.
    char** command = parsed_input.at(0)->data();
    
    //Executing the commands requested.
    pid_t process = fork();
    if(process == -1) {
        cout << "Error: Couldn't create process." << endl;
        return;
    }
    else if(process == 0) {
        
        if(execvp(command[0], command) == -1) {
            cout << "Error: Problem executing " << parsed_input.at(0)->at(0) << endl;
        }
    }
    else {

        //Waiting till ANY child from its process group has finished.
        waitpid(-1, NULL, 0);
    }

    return;
}