#include "Shell.h"
#include <fstream>

int main(int argc, char* argv[]) {
    
    clr();
    printStatus();

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
        

        while(getline(cin, rawCommand)) {

            parseInput(rawCommand);

            /*
            for(int i = 0; i < parsed_input.size; i++) {
                if(getBackgroundExecution()) {
                    processBackgroundExecution()
                }
                else {
                    if(getPipe()) {
                        processPipe()
                    }
                    if(getRedirect()) {
                        processRedirect()
                    }
                }
                
            }
            */
            
            


        }
        

    }
    
   return 0;
}

void getUsername(char* username) {
    getlogin_r(username, 50);
}