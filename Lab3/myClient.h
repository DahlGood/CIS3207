#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <unordered_set>
#include <cstdlib>
#include <fstream>
#include <typeinfo>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <chrono>
#include <ctime>  
#include <syscall.h>

using namespace std;

typedef struct Args {
    std::string ip;
    int port;
}Args;

void spawnThreads(int numThreads, Args *args);
void *makeConnection(void *args);
string randomWordGenerator(unordered_set<string> dictionary);
void loadDictionary(unordered_set<string> *dictionarySet);
string convertCase(string word);
void logger(string buff);