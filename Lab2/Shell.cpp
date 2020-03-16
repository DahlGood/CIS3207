#include "Shell.h"
#include <fstream>

vector<char*> paths;

int main(int argc, char* argv[]) {
    
    //Clears screen.
    clr();

    string rawCommand;
    
    //If a batch file is entered 
    if(argc > 1) {
        //open file specified in argv
        ifstream io;
        io.open(argv[1]);
        if(!io.is_open()) {
            cout << "Couldnt open the batch file specified." << endl;
        }
        //While there is still content in the file (aka we havent reached the end) continue.
        while(io.good()) {
            //Grabs content from the file line by line.
            getline(io, rawCommand);
            
            //Just some error handling.
            try
            {
                //Parse that line as a command string.
                parseInput(rawCommand);
            }
            catch(int error)
            {   
                cout << "Error: All commands must start with a command." << endl;
                
            }

        }
    }
    
    //If no batch file was specified, read and process from user input.
    else {
        //Making things look pretty.
        printStatus();
        //Getting user input.
        while(getline(cin, rawCommand)) {
            parseInput(rawCommand);
            printStatus();
        }
    }
    
   return 0;
}

//Mainly just to make things look good.
void getUsername(char* username) {
    getlogin_r(username, 50);
}