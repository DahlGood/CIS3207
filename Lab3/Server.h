#ifndef SERVER_H
#define SERVER_H

#include <unordered_set>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <queue>


using namespace std;

//Server

typedef struct SpellCheckDependincies {
    queue<int> connections;
    queue<string> log;
    unordered_set<string> dictionary;
}SpellCheckDependincies;

void *worker(void *arg);
void *logger(void *arg);
void spawnThreads(SpellCheckDependincies *dependencies);
void clientServicer(int socket, SpellCheckDependincies *dependencies);

//Spell Checking
bool spellCheck(string word, unordered_set<string> dictionarySet);
string convertCase(string word);
void loadDictionary(string dictionaryName, unordered_set<string> *dictionarySet);

#endif