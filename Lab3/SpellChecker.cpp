#include "Server.h"


bool spellCheck(string word, unordered_set<string> dictionarySet) {
    word = convertCase(word);

    if(dictionarySet.find(word) == dictionarySet.end()) {
        return false;
    }
    else {
        return true;
    }

}

string convertCase(string word) {

    for(unsigned int i = 0; i < word.length(); i++) {
        word[i] = tolower(word[i]);
    }

    return word;

}

void loadDictionary(string dictionaryName, unordered_set<string> *dictionarySet) {
    string wordBuffer;
    ifstream dictionaryFile;

    dictionaryFile.open(dictionaryName);

    if(!dictionaryFile.is_open()) {
        cout << "Problem opening dictionary file." << endl;
        exit(EXIT_FAILURE);
    }

    while(getline(dictionaryFile, wordBuffer)) {
        dictionarySet->insert(convertCase(wordBuffer));
    }

}