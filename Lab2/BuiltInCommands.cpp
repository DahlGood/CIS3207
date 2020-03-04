#include "Shell.h"
#include <unistd.h>


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

}
/*
void dir() {
    
    current_path = GetCurrentDirectory()
    if(GetCurrentDirectory wasnt successful) {
        print what went wrong.
    }
    for(all entries in directory_iterator(path)) {
        print entry.path();
    }

}

void dir(string path) {

    current_path = GetCurrentDirectory()
    for(all entryies in directory_iterator(path)) {
        print entry.path();
    }

}

void environ() {
    print getenv("USER"); //they dont deserve anything else.

}

void echo(string arg) {
    return arg
}

void help() {

}

void pause() {

    while (user input != '\n') {

    }

}

void quit() {

    exit()

}

*/


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