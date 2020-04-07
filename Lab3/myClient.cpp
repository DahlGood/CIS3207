#include "myClient.h"


int main(int argv, char* argc[]) {
    
    
    Args *arguments = (Args *)malloc(sizeof(Args));
    arguments->ip = argc[1];
    arguments->port = atoi(argc[2]);

    

    spawnThreads(10, arguments);

    return 0;
}

void spawnThreads(int numThreads, Args *args) {
    
    /*
    pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * numThreads);
    int x = 0;
    while(x < numThreads) {
        pthread_create(&threads[x], NULL, makeConnection, (void *)args);
        pthread_join(threads[x], NULL);
        x++;
    }
    */
    
   
   pthread_t worker1;
   pthread_create(&worker1, NULL, makeConnection, (void *)args);
   pthread_join(worker1, NULL);

}

void *makeConnection(void *args) {
    
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

    struct in_addr addrptr;
    memset(&addrptr, '\0', sizeof(addrptr));
    
    //Setup the socket with relevant data.
    client.sin_family = AF_INET; //ipv4
    client.sin_addr.s_addr = inet_addr(((Args *)args)->ip.c_str()); // IP entered when client starts //inet_aton(ip.c_str(), &addrptr) //INADDR_ANY
    client.sin_port = htons(((Args *)args)->port); //Server port defaults to 8888 btw.
    
    int connection = connect(socket_descriptor, (sockaddr*)&client, sizeof(client));
    if(connection == -1) {
        cout << "connection failed" << endl;
    }

    unordered_set<string> dictionary;
    loadDictionary(&dictionary);

    string input;
    char output[100];

    int readResult1 = read(socket_descriptor, output, sizeof(output));
    if(readResult1 == -1) {
        cout << "Couldnt write to server." << endl;
    }
    //while(true) {
        
        //cout << "client>";
        //getline(cin, input);

        string randomWord = randomWordGenerator(dictionary);

        int writeResult = write(socket_descriptor, randomWord.c_str(), sizeof(randomWord.c_str()));
        if(writeResult == -1) {
            cout << "Couldnt write to server." << endl;
        }
        write(socket_descriptor, "", sizeof(randomWord.c_str()));
        
        int readResult2 = read(socket_descriptor, output, sizeof(output));
        if(readResult2 == -1) {
            cout << "Couldnt write to server." << endl;
        }
        cout << output << endl;

        //Because we're only checking one random word for each thread (as specified) lets just stop here.
    //}

    close(socket_descriptor);

    return NULL;
}

string randomWordGenerator(unordered_set<string> dictionary) {
    //Generates a random number between 0 and 60. (the size of our random word dictionary.)
    srand(time(NULL));
    
    int num = (rand() % (60 + 1));

    auto iter = begin(dictionary);

    advance(iter, num);
    
    return *iter;
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