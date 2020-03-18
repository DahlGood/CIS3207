#include "Server.h"

int main() {

    /*

        Needs:
            work queue (circular buffer?). Stores all connections / connected sockets. Going to be globally declared in Server.h

        Done:
            dictionary - globally declared in Server.h 

    */

    
    if(loadDictionary() == -1) {
        cout << "Error: Couldn't load specified dictionary." << endl;
        exit(1);
    }

    return 0;
}

int loadDictionary() {
    
    ifstream dictionaryFile;
    string buffer;
    string dictionaryName;
    
    dictionaryFile.open("dictionary_479k.txt");
    if(!dictionaryFile.is_open()) {
        cout << "Problem opening dictionary file." << endl;
        return -1;
    }

    while(getline(dictionaryFile, buffer)) {
        dictionarySet.insert(convertCase(buffer));
    }

    return 0;

}