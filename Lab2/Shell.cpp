#include "Shell.h"
#include <fstream>

vector<char*> paths;

int main(int argc, char* argv[]) {
    
    clr();

    setEnvironPaths();

    string rawCommand;

    if(argc > 1) {
        //open file specified in argv
        ifstream io;
        io.open(argv[1]);
        if(!io.is_open()) {
            cout << "Couldnt open the batch file specified." << endl;
        }

        while(io.good()) {

            getline(io, rawCommand);

            try
            {
                parseInput(rawCommand);
            }
            catch(int error)
            {
                cout << "Error: All commands must start with a command." << endl;
            }
            
            

        }
        

    }
    
    else { 

        printStatus();
        

        while(getline(cin, rawCommand)) {

            parseInput(rawCommand);

        }
        

    }
    
   return 0;
}

void getUsername(char* username) {
    getlogin_r(username, 50);
}

void setEnvironPaths() {
    char* envPath = getenv("PATH");
    char* path = strtok(envPath, ":");

    while(strcmp(path, ".") != 0) {
        paths.push_back(path);
        path = strtok(NULL, ":");
    }

    return;
}

vector<char*> getEnvironPaths() {
    return paths;
}