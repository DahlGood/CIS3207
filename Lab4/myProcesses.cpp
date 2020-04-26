#include "processHeader.h"


#define TIME_TO_RUN 30
#define MAX_SIGNAL_COUNT 100000
#define SUB_PROCESSES 8

Counters *count;
int concurrentProcesses;
vector<time_t> pastSIGUSR1;
vector<time_t> pastSIGUSR2;

// void childDied(int signal) {
//     if(signal == SIGINT) {
//         sleep(4);
//         exit(0);
//     }
//     if(signal == SIGCHLD) {
//         cout << "A CHILD HAS DIED!!!!!!!!!!!!!" << endl;
//     }

// }

int main() {

    //Set-up shared memory - Initialization of globally shared variables
    //int shmget(key_t key, size_t size, int shmflg);  | IPC_EXCL

    int sharedMem = shmget(IPC_PRIVATE, sizeof(Counters), 0666 | IPC_CREAT);
    if(sharedMem == -1) {
        cout << "ERROR IN SHMGET" << endl;
        cout << errno << endl;
    }

    count = (Counters *)shmat(sharedMem, NULL, 0);
    if(count == (Counters *)-1) {
        cout << "ERROR IN SHMAT" << endl;
        cout << errno << endl;
    }

    block_signal(SIGUSR1);
    block_signal(SIGUSR2);
    
    //Set-up mutexes
    pthread_mutexattr_t attrOne;
    pthread_mutexattr_init(&attrOne);
    pthread_mutexattr_setpshared(&attrOne, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&(count->mutexOne), &attrOne);

    pthread_mutexattr_t attrTwo;
    pthread_mutexattr_init(&attrTwo);
    pthread_mutexattr_setpshared(&attrTwo, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&(count->mutexTwo), &attrTwo);

    pthread_mutexattr_t attrThree;
    pthread_mutexattr_init(&attrThree);
    pthread_mutexattr_setpshared(&attrThree, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&(count->mutexThree), &attrThree);


    //Controlling execution duration of the program
    //time_t startTime = time(nullptr);

    //Creating processes.
    pid_t processType[8];
    for(int i = 0; i <= SUB_PROCESSES; i++) {

        cout << "Value of I in Parent = " << i << endl;
        processType[i] = fork();
        if(processType[i] == -1) {
            cout << "ERROR FORKING" << endl;
        }
        //cout << "PID = " << processType[i] << endl;
        if(processType[i] == 0) {
            //cout << "Creating: " << i << " " << getppid() << " -> " << getpid() << endl;
            cout << "Value of I in Child = " << i << " " << getppid() << " -> " << getpid() << endl;
            if(i < 2) {
                cout << "Creating sigOne Handlers " << processType[i] << endl;
                signalUpdater(1);
            }
            else if(i >= 2 && i < 4) {
                //Creates 2 signal two handlers
                cout << "Creating sigTwo Handlers " << processType[i] << endl;
                signalUpdater(2);
            }
            else if(i == 4) {
                cout << "Creating reporter" << endl;
                reporter(1);
            }
            else if(i > 4 && i < 8) {
                cout << "Creating sigGen" << endl;
                generator();
            }
            if(i == 7) {
                waitpid(processType[i], NULL, 0);
                //wait(NULL);
            }
            
        }
        else {
            
 
            if(i == SUB_PROCESSES) {

                if(TIME_TO_RUN == -1) {
                    while(true) {
                        if(maxCountReached()) {

                           for(int y = 0; y < SUB_PROCESSES; y++) {
                                //If max count reached kill all child processes.
                                cout << "Attempting to kill process " << y << " " << processType[y] <<  endl;
                                kill(processType[y], SIGINT);
                            }

                            sleep(2);
                            
                            cout << count->sentSIGUSR1 << " " << count->receivedSIGUSR1 << endl;
                            cout << count->sentSIGUSR2 << " " << count->receivedSIGUSR2 << endl;

                            //Free shared memory in parent and exit.
                            int detatchedVal = shmdt(count);
                            if(detatchedVal == -1) {
                                cout << "Failed to detatch shared memory." << endl;
                            }
                            exit(0);
                        }
                    }
                }
                else {
                    cout << "MADE IT HERE" << endl;
                    sleep(TIME_TO_RUN);
                    cout << "MAIN FUNCTION PID " << getpid() << endl;
                    
                    for(int y = 0; y < SUB_PROCESSES; y++) {
                        //If max count reached kill all child processes.
                        cout << "Attempting to kill process " << y << " " << processType[y] <<  endl;
                        kill(processType[y], SIGINT);
                    }
                    
                    sleep(2);
                    cout << count->sentSIGUSR1 << " " << count->receivedSIGUSR1 << endl;
                    cout << count->sentSIGUSR2 << " " << count->receivedSIGUSR2 << endl;

                    //Free shared memory in parent and exit.
                    int detatchedVal = shmdt(count);
                    if(detatchedVal == -1) {
                        cout << "Failed to detatch shared memory." << endl;
                    }

                    exit(0);
                    
                }  

            }

            sleep(1);
        }
    }

    return 0;
}

void signalHandler(int signal) {

    if(signal == SIGINT) {
        cout << "Killing " << getpid() << " in signalHandler" << endl;
        int detatchedVal = shmdt(count);
        if(detatchedVal == -1) {
            cout << "Failed to detatch shared memory." << endl;
        }
        exit(0);
    }
    if(signal == SIGUSR1) {
        cout << "\t\t\tReceived SIGUSER1 in signalHandler" << endl;
    }
    else if(signal == SIGUSR2) {
        cout << "\t\t\tReceived SIGUSER2 in signalHandler" << endl;
    }
    
}

void signalUpdater(int value) {

    cout << "SignalUpdater " << getppid() << " -> " << getpid() << endl;

    unblock_signal(SIGINT);
    signal(SIGINT, signalHandler);

    if(value == 1) {
        unblock_signal(SIGUSR1);
        signal(SIGUSR1, signalHandler);
    }
    if(value == 2) {
        unblock_signal(SIGUSR2);
        signal(SIGUSR2, signalHandler);
    }
    
    while(true) {

        pause();

        if(value == 1) {
            cout << "Received SIGUSR1 " << getppid() << " -> " << getpid() << endl;
            pthread_mutex_lock(&count->mutexTwo);
            count->receivedSIGUSR1++;
            pthread_mutex_unlock(&count->mutexTwo);
        }
        if(value == 2) {
            cout << "Received SIGUSR2 " << getppid() << " -> " << getpid() << endl;
            pthread_mutex_lock(&count->mutexTwo);
            count->receivedSIGUSR2++;
            pthread_mutex_unlock(&count->mutexTwo);
        }

    }

    return;
}

void reporterHandler(int sig) {

    
    //pthread_mutex_unlock(&count->mutexThree);
    //yuyuyyupthread_mutex_unlock(&count->mutexOne);
    
    cout << "ReporterHandler " << getppid() << " -> " << getpid() << endl;
    if(sig == SIGINT) {
        cout << "Killing " << getppid() << " -> " << getpid() << " in reporterHandler" << endl;
        //free shared memory
        int detatchedVal = shmdt(count);
        if(detatchedVal == -1) {
            cout << "Failed to detatch shared memory." << endl;
        }
        exit(0);
    }

    //pthread_mutex_lock(&count->mutexTwo);
    if(sig == SIGUSR1) {
        //cout << "\t\t\tReporting SIGUSER1 " << getppid() << " -> " << getpid() << endl;
        pastSIGUSR1.push_back(time(NULL));
    }
    else if(sig == SIGUSR2) {
        //cout << "\t\t\tReporting SIGUSER2 " << getppid() << " -> " << getpid() << endl;
        pastSIGUSR2.push_back(time(NULL));
    }
    //pthread_mutex_unlock(&count->mutexTwo);

    
}

void reporter(int value) {

    cout << "Reporter " << getppid() << " -> " << getpid() << endl;

    unblock_signal(SIGUSR1);
    unblock_signal(SIGUSR2);
    unblock_signal(SIGINT);
    signal(SIGINT, reporterHandler);
    signal(SIGUSR1, reporterHandler);
    signal(SIGUSR2, reporterHandler);
    
    while(true) {
        
        pause();

        ofstream log;
    
        auto timenow = chrono::system_clock::to_time_t(chrono::system_clock::now());

        log.open("logP.txt", ios::app);
        if(!log.is_open()) {
            cout << "Couldnt open file correctly" << endl;
        }

        //wewewewewewpthread_mutex_lock(&count->mutexOne);
        //pthread_mutex_lock(&count->mutexThree);
        concurrentProcesses++;
        if(concurrentProcesses == 10) {
            pthread_mutex_lock(&count->mutexOne);
            pthread_mutex_lock(&count->mutexTwo);
            concurrentProcesses = 0;

            time_t sumSIGUSR1;
            time_t sumSIGUSR2;
            for(auto x : pastSIGUSR1) {
                sumSIGUSR1 += x;
            }
            for(auto x : pastSIGUSR2) {
                sumSIGUSR2 += x;
            }

            time_t avgTime1 = sumSIGUSR1/pastSIGUSR1.size();
            time_t avgTime2 = sumSIGUSR2/pastSIGUSR2.size();
            //ctime(&timenow);
            
            
            log << "SIGUSR1 | " << " | Sent " << count->sentSIGUSR1 << " Received " << count->receivedSIGUSR1 << " | " << "Current Time " << ctime(&timenow) << " | Avg Between Signals " << ctime(&avgTime1) << endl;
            log << "SIGUSR2 | " << " | Sent " << count->sentSIGUSR2 << " Received " << count->receivedSIGUSR2 << " | " << "Current Time " << ctime(&timenow) << " | Avg Between Signals " << ctime(&avgTime2) << endl;
            

            sumSIGUSR1 = 0;
            sumSIGUSR2 = 0;
            avgTime1 = 0;
            avgTime2 = 0;
            pastSIGUSR1.clear();
            pastSIGUSR2.clear();

            pthread_mutex_unlock(&count->mutexOne);
            pthread_mutex_unlock(&count->mutexTwo);

        }

        // pthread_mutex_lock(&count->mutexOne);
        
        // cout << "Sent " << count->sentSIGUSR1 << " SIGUSR1 signals & received " << count->receivedSIGUSR1 << endl;

        // cout << "Sent " << count->sentSIGUSR2 << " SIGUSR2 signals & received " << count->receivedSIGUSR2 << endl;

        // pthread_mutex_unlock(&count->mutexOne);

    }

}

void generatorHandler(int signal) {

    cout << "Generator Handler " << getppid() << " -> " << getpid() << endl;
    
    if(signal == SIGINT) {
        cout << "Killing " << getppid() << " -> " << getpid() << " in generatorHandler" << endl;
        int detatchedVal = shmdt(count);
        if(detatchedVal == -1) {
            cout << "Failed to detatch shared memory." << endl;
        }
        exit(0);
    }

    
}

void generator() {
    //sleep(2);

    cout << "Generator " << getppid() << " -> " << getpid() << endl;

    signal(SIGINT, generatorHandler);

    srand(time(NULL));

    int x = 0;
    while(true) {
        x++;
        double randNum = randGenerator(0.0, 1.0);
        if(randNum < 0.5) {
            cout << "Sent SIGUSR1 " << getppid() << " -> " << getpid() << endl;
            pthread_mutex_lock(&count->mutexOne);
            count->sentSIGUSR1++;
            pthread_mutex_unlock(&count->mutexOne);
            kill(0, SIGUSR1);
        }
        else if(randNum >= 0.5){

            cout << "Sent SIGUSR2 " << getppid() << " -> " << getpid() << endl;
            pthread_mutex_lock(&count->mutexOne);
            count->sentSIGUSR2++;
            pthread_mutex_unlock(&count->mutexOne);
            kill(0, SIGUSR2);

        }
        //Fix these times
        struct timespec whats_a_long;
        whats_a_long.tv_sec = 0;
        //whats_a_long.tv_nsec = randGenerator(0.01, 0.1) * 1000000000L;
        whats_a_long.tv_nsec = 0.01 * 1000000000L;
        //whats_a_long.tv_nsec = 1;
        nanosleep(&whats_a_long, NULL);
    }
    //pause();
}

double randGenerator(double lowerBound, double upperBound) {
    int ran = rand();
    double randomNum = (fmod((double)ran, upperBound*100) + lowerBound*100);
    return randomNum/100;
}

void block_signal(int signal) {
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, signal);
    int sigReturn = sigprocmask(SIG_BLOCK, &sigset, NULL);
    if(sigReturn != 0) {
        cout << errno << endl;
    }
    
}

void unblock_signal(int signal) {
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, signal);
    int sigReturn = sigprocmask(SIG_UNBLOCK, &sigset, NULL);
    if(sigReturn != 0) {
        cout << errno << endl;
    }
    
}


bool maxCountReached() {
    int total = (count->sentSIGUSR1 + count->sentSIGUSR2);
    if(total >= MAX_SIGNAL_COUNT) {
        return true;
    }
    
    return false;
}

