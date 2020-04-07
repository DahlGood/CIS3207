#include "myClient.h"
#include <chrono>
#include <ctime>  

pthread_mutex_t connection_security;
pthread_cond_t connection_state;

pthread_mutex_t random_security;


int main(int argv, char* argc[]) {

    // ofstream io1;

    // //Clears file
    // io1.open("client-log.txt", ios::trunc);
    // if(!io1.is_open()) {
    //     cout << "Couldn't open the log." << endl;
    // }
    // io1.close();

    vector<string> log;    
    
    Args *arguments = (Args *)malloc(sizeof(Args));
    
    arguments->ip = argc[1];
    arguments->port = atoi(argc[2]);
    arguments->log = log;

    

    spawnThreads(5, arguments);

    return 0;
}

void spawnThreads(int numThreads, Args *arguments) {
    
    //pthread_mutex_init(&connection_security, NULL);
    //pthread_cond_init(&connection_state, NULL);

    //pthread_mutex_init(&random_security, NULL);

    pthread_t logging;
    pthread_create(&logging, NULL, &logger, (void *)arguments);
    
    pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * numThreads);

    int x = 0;
    while(x < numThreads) {
        pthread_create(&threads[x], NULL, &makeConnection, (void *)arguments);
        pthread_join(threads[x], NULL);
        x++;
    }
   
//    pthread_t worker1;
//    pthread_create(&worker1, NULL, makeConnection, (void *)args);
//    pthread_join(worker1, NULL);

}

void *makeConnection(void *args) {
    //g++ myClient.cpp -o myClient -pthread -std=c++11


    
    
    //pthread_mutex_lock(&connection_security);
    //pthread_cond_wait(&connection_state, &connection_security);
    
    //Socket descriptors operate like file descriptors.
    int socket_descriptor;
    
    //Sockets are defined as structs. (sockaddr_in defined in netinet/in.h) this struct contains 3 important values. sin_family = address type (ex ipv4/6), sin_port = port its operating on, sin_addr = address it's operating on.
    //struct sockaddr_in server;
    struct sockaddr_in client;

    // Create socket (create active socket descriptor) (socket(domain, type, protocol))
    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0); //arg translation = socket(ipv4, tcp, IP) (IP = 0, the same value that appears in the protocol field in a packet header when using IPs.)
    if (socket_descriptor == -1){ 
        puts("Error creating socket!");
        exit(1);
    }

    //Setup the socket with relevant data.
    client.sin_family = AF_INET; //ipv4
    client.sin_addr.s_addr = INADDR_ANY;//inet_addr(((Args *)args)->ip.c_str()); // IP entered when client starts //inet_aton(ip.c_str(), &addrptr) //INADDR_ANY
    client.sin_port = htons(8888);//htons(((Args *)args)->port); //Server port defaults to 8888 btw.
    
    int connection = connect(socket_descriptor, (sockaddr*)&client, sizeof(client));
    if(connection == -1) {
        cout << "connection failed" << endl;
    }
    
    unordered_set<string> dictionary;
    loadDictionary(&dictionary);

    string input;
    char output[100];

    int readResult1 = recv(socket_descriptor, output, sizeof(output), 0);
    if(readResult1 == -1) {
        cout << "Couldnt write to server." << endl;
    }
    if(!readResult1) {
        cout << "NOPE" << endl;
    }

    string randomWord = randomWordGenerator(dictionary);

    cout << "Random Word = " << randomWord.c_str() << endl;

    int writeResult = write(socket_descriptor, randomWord.c_str(), strlen(randomWord.c_str()));
    if(writeResult == -1) {
        cout << "Couldnt write to server." << endl;
    }
    
    int readResult2 = read(socket_descriptor, output, sizeof(output));
    if(readResult2 == -1) {
        cout << "Couldnt write to server." << endl;
    }

    //Pushing to the log
    ((Args *)args)->log.push_back(output);

    close(socket_descriptor);

    //pthread_cond_signal(&connection_state);
    //pthread_mutex_unlock(&connection_security);

    return NULL;
}

void *logger(void *args) {

    time_t t; // t passed as argument in function time()
    struct tm * tt; // decalring variable for localtime()
    pid_t pid = syscall(__NR_gettid);

    //Declaring output file stream.
    ofstream io;

    //Clears file
    cout << "Got to log" << endl;
    io.open("client-log.txt", ios::trunc);
    if(!io.is_open()) {
        cout << "Couldn't open the log." << endl;
    }
    io.close();

    //Writes to log.
    while(true) {
        
        // pthread_mutex_lock(&log_security);

        if(((Args *)args)->log.size() == 0) {
            // pthread_cond_wait(&log_used, &log_security);
            // pthread_cond_signal(&log_free);
        }

        //I genuinly wrote out these conditions then realized that this is way to stupidly funny to delete. size > 0...
        while(! (((Args *)args)->log.size() <= 0)) {
            io.open("client-log.txt", ios::app);
            if(!io.is_open()) {
                cout << "Couldn't opent the log." << endl;
            }

            
            time (&t); //passing argument to time()
            tt = localtime(&t);
            
            io << "PID: " << pid << " | Time: " << asctime(tt) << " | " << ((Args *)args)->log.back();
            ((Args *)args)->log.pop_back();

            io.close();
        }

        // pthread_mutex_unlock(&log_security);
        
    }

    return NULL;
}

string randomWordGenerator(unordered_set<string> dictionary) {
    //Generates a random number between 0 and 60. (the size of our random word dictionary.)
    srand(syscall(__NR_gettid));

    int num;
    int x = 0;
    while(x < 4) {
        num = (rand() % (60 + 1));
        x++;
    }

    auto iter = begin(dictionary);

    advance(iter, num);
    
    string randomWord = *iter;
    randomWord += '\n';
    return randomWord;
}

void loadDictionary(unordered_set<string> *dictionarySet) {
    string wordBuffer;
    ifstream dictionaryFile;

    dictionaryFile.open("random_words.txt");

    if(!dictionaryFile.is_open()) {
        cout << "Problem opening dictionary file." << endl;
        exit(EXIT_FAILURE);
    }

    while(getline(dictionaryFile, wordBuffer)) {
        dictionarySet->insert(convertCase(wordBuffer));
    }

}

string convertCase(string word) {

    for(unsigned int i = 0; i < word.length(); i++) {
        word[i] = tolower(word[i]);
    }

    return word;

}
