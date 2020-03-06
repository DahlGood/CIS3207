#include "Shell.h"
#include <fstream>

int main(int argc, char* argv[]) {
    
    clr();

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