#include <unordered_set>
#include <iostream>
#include <fstream>
#include <ctype.h>


using namespace std;

extern unordered_set<string> dictionarySet;
//extern work queue

string convertCase(string word);

int loadDictionary();
string spellCheck();
string convertCase(string word);