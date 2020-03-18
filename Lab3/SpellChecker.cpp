#include "Server.h"

string spellCheck() {

    string word;
    cout << "Word to check:" << endl;
    cin >> word;
    word = convertCase(word);

    if(dictionarySet.find(word) == dictionarySet.end()) {
        //cout << "Sorry, you must have mispelled \"" << word << "\" or it's not present in the dictionary you chose." << endl;
        return (word += ", was not spelled correctly.");
    }
    else {
        //cout << "\"" << word << "\" was spelled correctly." << endl;
        return (word += ", was spelled correctly.");
    }
    

}