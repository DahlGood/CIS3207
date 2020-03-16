#include "Shell.h"
#include <unistd.h>
#include <dirent.h>


void cd(char* path) {

    try
    {
        int returnedVal = chdir(path);
        //Not putting the rest of the code in this else may cause problems.
        if(returnedVal != 0) {
            throw(returnedVal);
        }

    }
    catch(int returnedVal)
    {
        cout << "Problem changing directories. Returned Value: " << returnedVal << endl;
    }

    return;

}

void clr() {
    cout << "\033[H\033[2J" << endl;
    return;
}

void dir() {

    DIR *directory = NULL;

    struct dirent *directoryInfo = NULL;

    directory = opendir(".");
    if(directory == NULL) {
        cout << "Error: Couldn't open directory." << endl;
    }

    while((directoryInfo = readdir(directory))) {
        if(directoryInfo == NULL) {
            cout << "Error: Couldn't read directory." << endl;
        }

        cout << directoryInfo->d_name << endl;
    }

    closedir(directory);

    return;

}

void dir(char* path) {

    DIR *directory = NULL;

    struct dirent *directoryInfo = NULL;

    directory = opendir(path);
    if(directory == NULL) {
        cout << "Error: Couldn't open directory." << endl;
    }

    while((directoryInfo = readdir(directory))) {
        if(directoryInfo == NULL) {
            cout << "Error: Couldn't read directory." << endl;
        }

        cout << directoryInfo->d_name << endl;
    }

    closedir(directory);

    return;

}


void myenviron() {

    string path[] = {"HOST", "USER", "HOME", "PATH", "OSTYPE"};

    //cout << getenv("PATH") << endl; //they dont deserve anything else.

    for(string x : path) {
        cout << x << " = " << getenv(x.c_str()) << endl;
    }

    return;

}


void myecho(vector<char *> cmdGrpOne) {
    if(cmdGrpOne.size() < 2) {
        throw(-1);
    }
    for(unsigned int i = 1; i < cmdGrpOne.size(); i++) {
        if(cmdGrpOne.at(i) == NULL) {
            cout << endl;
            return;
        }
        cout << cmdGrpOne.at(i) << " ";
    }

    return;

}

void mypause() {
    string waittill;
    cout << "Press any key to continue.";
    while(cin.get() != '\n') {
        return;
    }

}

void myquit() {
    exit(0);
}


void help() {
    cout << "cd - takes you to a directory. Ex: cd Directory/Directory2" << endl;
    cout << "clr - clears the current screen. Ex: clr " << endl;
    cout << "dir - prints content of a directory. Ex: dir, dir Directory/Directory2" << endl;
    cout << "environ - prints helpful environment variables" << endl;
    cout << "echo - takes at least one argument. Ex: echo Hi, echo Hello World" << endl;
    cout << "pause - waits for a user to continue. Ex: pause" << endl;
    cout << "quit - exits the shell. Ex: quit" << endl;
    cout << "help - prints all the built in commands and their uses. Ex: help" << endl;
    return;
}

void help(char* command) {
    
    switch (isBuiltIn(command)) {
            case 0:
                cout << "cd - takes you to a directory. Ex: cd Directory/Directory2" << endl;
                break;
            case 1:
                cout << "clr - clears the current screen. Ex: clr" << endl;
                break;
            case 2:
                cout << "dir - prints content of a directory. Ex: dir, dir Directory/Directory2" << endl;
                break;
            case 3:
                cout << "environ - prints helpful environment variables" << endl;
                break;
            case 4:
                cout << "echo - takes at least one argument. Ex: echo Hi, echo Hello World" << endl;
                break;
            case 5:
                cout << "pause - waits for a user to continue. Ex: pause" << endl;
                break;
            case 6:
                cout << "quit - exits the shell. Ex: quit" << endl;
                break;
            case 7:
                cout << "help - prints a specific built in command or all built in commands and how to use them. Ex: help, help echo" << endl;
                break;
            default:
                cout << "Sorry " << command << " is not a built in command." << endl;
                break;
        }
        
    
    return;
}

void printStatus() {
    //max size of the current directory.
    char pathBuffer[200];

    //Gets the new current directory and puts it in the buffer.
    getcwd(pathBuffer, 200);

    //Prints the username and directory to make things pretty.
    char usernameBuffer[50];
    getUsername(usernameBuffer);
    cout << usernameBuffer << ":~ "<< pathBuffer << "> ";
    
    return;

}