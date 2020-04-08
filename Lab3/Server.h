#ifndef SERVER_H
#define SERVER_H

#include "circular_buffer.h"
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <syscall.h>

using namespace std;

//Server
typedef struct SpellCheckDependencies {
    CBuffer connections;
    vector<string> log;
    unordered_set<string> dictionary;
}SpellCheckDependencies;

void *worker(void *arg);
void *logger(void *arg);
void spawnThreads(SpellCheckDependencies *dependencies);
void clientServicer(int socket, SpellCheckDependencies *dependencies);
int returnIdentifier(string val);

//Spell Checking
bool spellCheck(string word, unordered_set<string> dictionarySet);
string convertCase(string word);
void loadDictionary(string dictionaryName, unordered_set<string> *dictionarySet);

#endif