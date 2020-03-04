#include "Shell.h"
#include <unistd.h>
#include <dirent.h>


void cd(char* path) {

    //Skips to a new line.
    cout << endl;

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

    printStatus();
    

}
    


void clr() {

    //printf("\033[H\033[2J");
    cout << "\033[H\033[2J" << endl;

    printStatus();

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

    printStatus();

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

    printStatus();

}


void myenviron() {

    string path[] = {"HOST", "USER", "HOME", "PATH", "OSTYPE"};

    //cout << getenv("PATH") << endl; //they dont deserve anything else.

    for(string x : path) {
        cout << x << " = " << getenv(x.c_str()) << endl;
    }

    printStatus();

}


void myecho(char* arg) {
    if(arg == NULL) {
        throw(-1);
    }
    cout << arg << endl;
}

void mypause() {
    string waittill;
    while(cin.get() != '\n') {
        return;
    }

}

void myquit() {

    exit(0);

}


void help() {
    cout << "print help pages?" << endl;
}




void printStatus() {
    //max size of the current directory.
    char pathBuffer[200];

    //Gets the new current directory and puts it in the buffer.
    getcwd(pathBuffer, 200);

    //Prints the username and directory to make things pretty.
    char usernameBuffer[50];
    getUsername(usernameBuffer);
    cout << usernameBuffer << ":~ "<< pathBuffer << ">";
}