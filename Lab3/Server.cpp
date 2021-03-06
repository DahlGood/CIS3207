#include "Server.h"
#include <typeinfo>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <signal.h>

//#define DEFAULT_DICTIONARY "/usr/share/dict/words";
#define DEFAULT_DICTIONARY "/usr/share/dict/words";
#define DEFAULT_PORT 8888;

pthread_mutex_t connection_security;
pthread_cond_t connection_used;
pthread_cond_t connection_free;

pthread_mutex_t log_security;
pthread_cond_t log_used;
pthread_cond_t log_free;

int main(int argv, char* argc[]) {

    //Connection Queue - Accessible to all threads.
    //custom circular buffer?
    CBuffer connections(5);

    //Log Queue - Accessible to all threads.
    vector<string> log;
    //log.push("help");
        
    //Dictionary - Accessible to all threads.
    unordered_set<string> dictionarySet;

    //Port the server is running on.
    int port;
    //Name of the dictionary in use.
    string dictionaryName;

    //Struct containing the log queue, connection queue, and dictionary. Everything a thread could possibly need to execute.
    //SpellCheckDependencies *dependencies = new SpellCheckDependencies;
    SpellCheckDependencies *dependencies = (SpellCheckDependencies *)malloc(sizeof(SpellCheckDependencies));
    
    //Determines if the server admin has set a non-standard port or dictionary to use.
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

    dependencies->connections = connections;
    //cout << "log size: " << log.size() << endl;
    dependencies->log = log;
    //dependencies->log.push_back("hi");
    dependencies->dictionary = dictionarySet;

    //Load the dictionary.
    //loadDictionary(dictionaryName, &dictionarySet);
    loadDictionary(dictionaryName, &(dependencies->dictionary));

    //Socket descriptors operate like file descriptors.
    int socket_descriptor;
    int new_socket;

    //Sockets are defined as structs. (sockaddr_in defined in netinet/in.h) this struct contains 3 important values. sin_family = address type (ex ipv4/6), sin_port = port its operating on, sin_addr = address it's operating on.
    struct sockaddr_in server;
    struct sockaddr_in client;

    // Create socket (create active socket descriptor) (socket(domain, type, protocol))
    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0); //arg translation = socket(ipv4, tcp, IP) (IP = 0, the same value that appears in the protocol field in a packet header when using IPs.)
    if (socket_descriptor == -1){ 
        puts("Error creating socket!");
        exit(1);
    }

    //Allows the server to reuse the same address and port when the server re-starts.
    int opt = 1;
    int socket_options = setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    if(socket_options == -1) {
        puts("Error setting socket options");
    }
    
    //Setup the socket with relevant data.
    server.sin_family = AF_INET; //ipv4
    server.sin_addr.s_addr = INADDR_ANY; // defaults to 127.0.0.1 (localhost :) )
    server.sin_port = htons(port); //default according to our program is 8888. htons converts from host byte order short to network byte order short.

    //Bind (corrent the server's socket address to the socket descriptor)
    //Binds the socket to the address and port specified. Binding defined the relationship between the socket that you created and an address on your network.
    int bind_result = bind(socket_descriptor, (struct sockaddr*)&server, sizeof(server));
    if (bind_result < 0){
        puts("Error: failed to Bind.");
        exit(1);
    }

    puts("Bind done.");

    //Just the size of the socket, for later use.
    int socketSize = sizeof(struct sockaddr_in);
    
    // Listen (converts active socket to a LISTENING socket which can accept connections)
    listen(socket_descriptor, 3); //Putting the socket into a state that waits for connections to come through. We're only allowing a maximum of 3 connections to be queued at a time.
    
    puts("Waiting for incoming connections...");

    //Prevents server crashing when client exits abruptly.
    signal(SIGPIPE, SIG_IGN);

    //Create a pool of threads.
    spawnThreads(dependencies);
    
    //Loop to accept socket connections.
    while(true) {
        
        //Accept(socket descriptor, socket to be accepted, size of a socket.)
        new_socket = accept(socket_descriptor, (struct sockaddr*)&client, (socklen_t*)&socketSize);
        if (new_socket < 0){
            puts("Error: Accept failed");
        }

        //Alerts mutex that we have something in our connections queue.
        //pthread_cond_signal(&connection_used);

        puts("Connection accepted");

        pthread_mutex_lock(&connection_security);
        
        //Add this new connection to the socket queue. It looks like this whole process should be protected. For example we dont want the data of new socket to be overwritten before it can be added to the connection queue.
        dependencies->connections.slide(new_socket);
        cout << "Connections size = " << dependencies->connections.size() << endl;
        
        //If there is a connection waiting to be processed unlock connection security and let it sleep. (This should always be true.)
        if(dependencies->connections.size() > 0) {
            //Since there is now a conection ready to be processed wake up the worker thread.
            pthread_cond_signal(&connection_free);
            pthread_cond_wait(&connection_used, &connection_security);
        }


        pthread_mutex_unlock(&connection_security);

    }

    //slide event (word and whether it was correctly spelled or not).
    free(dependencies);
    //pthread_mutex_destroy(&connection_security);
    //pthread_mutex_destroy(&log_security);
    return 0;
}

void spawnThreads(SpellCheckDependencies *dependencies) {
    
    pthread_mutex_init(&connection_security, NULL);
    pthread_cond_init(&connection_used, NULL);
    pthread_cond_init(&connection_free, NULL);

    pthread_mutex_init(&log_security, NULL);
    pthread_cond_init(&log_used, NULL);
    pthread_cond_init(&log_free, NULL);

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
    
    while(true) {

        pthread_mutex_lock(&connection_security);

        //If there are no connections waiting, unlock the thread and put it to sleep.
        if(((SpellCheckDependencies *)dependencies)->connections.size() == 0) {
            pthread_cond_wait(&connection_free, &connection_security);
            pthread_cond_signal(&connection_used);
        }

        while(! (((SpellCheckDependencies *)dependencies)->connections.size() <= 0)) {
            
            int socketD = ((SpellCheckDependencies *)dependencies)->connections.snatch();
            
            //If I delete either of these prints the server doesnt properly accept client messages and I have no clue why.
            cout << "SocketID in Worker = " << socketD << endl;
            //cout << socketD << endl;

            clientServicer(socketD, ((SpellCheckDependencies *)dependencies));

        }

        pthread_mutex_unlock(&connection_security);

    }

    return NULL;
}


void *logger(void *dependencies) {
    
    //Declaring a buffer for whats going to be placed in the log.
    string buff;

    //Declaring output file stream.
    ofstream io;

    //Clears file
    io.open("log.txt", ios::trunc);
    if(!io.is_open()) {
        cout << "Couldn't open the log." << endl;
    }
    io.close();

    //Writes to log.
    while(true) {
        
        pthread_mutex_lock(&log_security);

        if(((SpellCheckDependencies *)dependencies)->log.size() == 0) {
            pthread_cond_wait(&log_used, &log_security);
            pthread_cond_signal(&log_free);
        }

        //I genuinly wrote out these conditions then realized that this is way to stupidly funny to delete. size > 0...
        while(! (((SpellCheckDependencies *)dependencies)->log.size() <= 0)) {
            io.open("log.txt", ios::app);
            if(!io.is_open()) {
                cout << "Couldn't opent the log." << endl;
            }

            buff = ((SpellCheckDependencies *)dependencies)->log.back();
            ((SpellCheckDependencies *)dependencies)->log.pop_back();
            
            io << buff;

            io.close();
        }

        pthread_mutex_unlock(&log_security);
        
    }

    return NULL;
}


void clientServicer(int socket, SpellCheckDependencies *dependencies) {

        pthread_mutex_unlock(&connection_security);
        
        //Buffer for word sent by user.
        char word2[100];

        //Word that is going to be sent to the spell checker.
        string wordToCheck;

        //Word + Spell Check result.
        string logBuffer;

        //Message sent to clients when they connect.
        char notice[51] = "You may now begin entering words to spell check.\n";
        int writeResult = write(socket, notice, sizeof(notice));
        if(writeResult == -1) {
            cout << "Couldnt write to socket." << endl;
            return;
        }

        while(true) {

            //Receives message sent from the client. recv() returns -1 if failed, otherwise the number of bytes received from the client.
            int returnVal = recv(socket, &word2, sizeof(word2), 0);
            if(returnVal == -1) {
                cout << "Failed to receive message from client." << endl;
                close(socket);
            }

            //If bytes received = 0 then gracefully close the connection. (0 bytes will be sent if the client disconnects.)
            if(!returnVal) {
                cout << "Graceful disconnection complete." << endl;
                close(socket);
                return;
            }

            //Get the word we want to spell check from the buffer. (This removes carriage returns and garbage)
            int x = 0;
            while(x < returnIdentifier(word2)) {
                wordToCheck += word2[x];
                x++;
            }

            logBuffer = wordToCheck;
            
            //Spell check
            if(spellCheck(wordToCheck, dependencies->dictionary)) {
                logBuffer += " OK\n";
                
                //Share the results with the client.
                int writeResult1 = write(socket, logBuffer.c_str(), strlen(logBuffer.c_str()));
                if(writeResult1 == -1) {
                    cout << "Couldnt write to socket1." << endl;
                    return;
                }
            }
            else {
                logBuffer += " MISPELLED\n";

                int writeResult2 = write(socket, logBuffer.c_str(), strlen(logBuffer.c_str()));
                if(writeResult2 == -1) {
                    cout << "Couldnt write to socket2." << endl;
                    return;
                }

            }

            //Places the complete spell checking result in the log.
            pthread_mutex_lock(&log_security);
            
            dependencies->log.push_back(logBuffer);

            if(dependencies->log.size() > 0) {
                pthread_cond_signal(&log_used);
                pthread_cond_wait(&log_free, &log_security);
            }

            pthread_mutex_unlock(&log_security);

            //Empties 
            wordToCheck.clear();
            logBuffer.clear();

        }
    
    return;
}

//Returns the index of a carriage return thats located in the buffer.
int returnIdentifier(string word) {

    unsigned int x = 0;
    while(x < word.size()) {
        if(word[x] == '\n') {
            return x;
        }
        if(word[x] == '\r') {
            return x;
        }

        x++;
    }

    return x;
}