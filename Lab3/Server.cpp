#include "Server.h"
#include <typeinfo>
#include <string.h>
#include <unistd.h>

#define DEFAULT_DICTIONARY "/usr/share/dict/words";
#define DEFAULT_PORT 8888;

int main(int argv, char* argc[]) {


    
    /*
    while() {
        //connected_socket = accept(listening_socket);
        //add connected_socket to the work queue
        //signal any sleeping workers that theres a new socket in the queue.

    }
    */


    //Connection Queue - Accessible to all threads.
    //custom circular buffer?
    queue<int> connections;

    //Log Queue - Accessible to all threads.
    queue<string> log;
        
    //Dictionary - Accessible to all threads.
    unordered_set<string> dictionarySet;

    int port;
    string dictionaryName;
    
    if(argv == 1) {
        dictionaryName = DEFAULT_DICTIONARY;
        port = DEFAULT_PORT;
    }
    else if(argv == 2) {
        if(strcmp(typeid(argc[1]).name(), "string") == 1) {
            dictionaryName = argc[1];
            port = DEFAULT_PORT;
        }
    }
    else {
        if(strcmp(typeid(argc[1]).name(), "string") == 1) {
            dictionaryName = argc[1];
            port = atoi(argc[2]);
        }
        else {
            dictionaryName = argc[2];
            port = atoi(argc[1]);
        }
    }

    

    cout << port << endl;

    loadDictionary(dictionaryName, &dictionarySet);

    //Struct containing the log queue, connection queue, and dictionary. Everything a thread could possibly need to execute.
    SpellCheckDependincies *dependencies = new SpellCheckDependincies;

    dependencies->connections = connections;
    dependencies->log = log;
    dependencies->dictionary = dictionarySet;

    spawnThreads(dependencies);

    cout << dependencies->log.front() << endl;

    
    

    //Push event (word and whether it was correctly spelled or not).

    return 0;
}

void spawnThreads(SpellCheckDependincies *dependencies) {
    
    pthread_t worker_1;
    pthread_t worker_2;
    pthread_t worker_3;
    pthread_t worker_4;
    pthread_t logging;

    pthread_create(&worker_1, NULL, &worker, (void *)dependencies);
    pthread_create(&worker_2, NULL, &worker, (void *)dependencies);
    pthread_create(&worker_3, NULL, &worker, (void *)dependencies);
    pthread_create(&worker_4, NULL, &worker, (void *)dependencies);
    pthread_create(&logging, NULL, &logger, (void *)dependencies);

    return;
}


void *worker(void *dependencies) {

    //cout << ((SpellCheckDependincies *)dependencies)->log.front() << endl;
    ((SpellCheckDependincies *)dependencies)->log.push("Hello");
    cout << ((SpellCheckDependincies *)dependencies)->log.front() << endl;
    
    while(true) {
        while(!((SpellCheckDependincies *)dependencies)->connections.empty()) {
            int socketD = ((SpellCheckDependincies *)dependencies)->connections.front();
            ((SpellCheckDependincies *)dependencies)->connections.pop();
            //notify that theres an empty spot in the queue
            //service client
            clientServicer(socketD, ((SpellCheckDependincies *)dependencies));
            //close socket
        }
    }
    
    return NULL;
}


void *logger(void *arg) {
    //monitor log queue and process entries by removing and writing them to a log.

    /*
        while(true) {
            while(log is not empty) {
                remove an entry from the log queue
                write the entry to the log file
            }
        }
    */
    return NULL;
}


void clientServicer(int socket, SpellCheckDependincies *dependencies) {
        char wordToCheck[100];
        char logBuffer[100];

        char* notice = "You may now begin entering words to spell check.";
        write(socket, notice, sizeof(notice));
        while(true) {
            read(socket, wordToCheck, sizeof(wordToCheck));
            if(spellCheck(wordToCheck, ((SpellCheckDependincies *)dependencies)->dictionary)) {
                cout << "\"" << wordToCheck << "\" was spelled correctly." << endl;
                strcpy(logBuffer, wordToCheck);
                strcat(logBuffer, " MISPELLED");
                write(socket, logBuffer, sizeof(logBuffer));
                //Push log buffer to log queue.
                ((SpellCheckDependincies *)dependencies)->log.push(logBuffer);
            }
            else {
                cout << "Sorry, you must have mispelled \"" << wordToCheck << "\" or it's not present in the dictionary used by the server." << endl;
                strcpy(logBuffer, wordToCheck);
                strcat(logBuffer, " OK");
                write(socket, logBuffer, sizeof(logBuffer));
                //Push log buffer to log queue
                ((SpellCheckDependincies *)dependencies)->log.push(logBuffer);
            }
        }
    
    return;
}