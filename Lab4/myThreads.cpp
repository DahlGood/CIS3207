#include "threadHeader.h"


#define TIME_TO_RUN -1
#define MAX_SIGNAL_COUNT 100000
#define SUB_PROCESSES 8

Counters *count;
int concurrentProcesses;
pthread_t processType[8];

struct timespec timeSIGUSR1;
double timeSumSIGUSR1;

struct timespec timeSIGUSR2;
double timeSumSIGUSR2;


int main() {

    srand(time(NULL));

    //Set-up shared memory - Initialization of globally shared variables
    //int shmget(key_t key, size_t size, int shmflg);  | IPC_EXCL

    //Args *args = (Args *)malloc(sizeof(Args));

    // int sharedMem = shmget(IPC_PRIVATE, sizeof(Counters), 0666 | IPC_CREAT);
    // if(sharedMem == -1) {
    //     cout << "ERROR IN SHMGET" << endl;
    //     cout << errno << endl;
    // }

    // count = (Counters *)shmat(sharedMem, NULL, 0);
    // if(count == (Counters *)-1) {
    //     cout << "ERROR IN SHMAT" << endl;
    //     cout << errno << endl;
    // }

    count = (Counters *)malloc(sizeof(Counters));

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

    // pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * 8);

    //Creating processes.
    //pid_t processType[8];
    int i = 0;
    for(i; i < SUB_PROCESSES; i++) {

        cout << "I = " << i << endl;

        if(i < 2) {
            
            pthread_create(&processType[i], NULL, &signalUpdater, (void *)SIGUSR1);
              
        }
        else if(i >= 2 && i < 4) {
            pthread_create(&processType[i], NULL, &signalUpdater, (void *)SIGUSR2);
        }
        else if(i == 4) {
            pthread_create(&processType[i], NULL, &reporter, (void *)NULL);
        }
        else if(i > 4 && i < 8) {
            pthread_create(&processType[i], NULL, &generator, (void *)NULL);
        }

        // if(i == 7) {
        //     pthread_join(processType[i], NULL);
        // }

        //sleep(1);

    }
    //if(i == 7) {
        if(TIME_TO_RUN == -1) {
            while(true) {
                if(maxCountReached()) {

                    for(int y = 0; y < SUB_PROCESSES; y++) {
                        //If max count reached kill all child processes.
                        cout << "Attempting to kill process " << y << " " << processType[y] <<  endl;
                        pthread_kill(processType[y], SIGINT);
                    }

                    sleep(2);
                    
                    cout << count->sentSIGUSR1 << " " << count->receivedSIGUSR1 << endl;
                    cout << count->sentSIGUSR2 << " " << count->receivedSIGUSR2 << endl;

                    exit(0);
                }
            }
        }
        else {
            cout << "MADE IT HERE" << endl;
            sleep(TIME_TO_RUN);
            cout << "MAIN FUNCTION PID " << getpid() << endl;
            
            cout << sizeof(processType)/sizeof(processType[0]) << endl;
            // for(int y = SUB_PROCESSES-1; y >= 0; y--) {
            //     //If max count reached kill all child processes.
            //     cout << "Attempting to kill process " << y << " " << processType[y] <<  endl;
            //     pthread_kill(processType[y], SIGINT);
            // }
            
            sleep(2);
            cout << count->sentSIGUSR1 << " " << count->receivedSIGUSR1 << endl;
            cout << count->sentSIGUSR2 << " " << count->receivedSIGUSR2 << endl;

            //Free shared memory in parent and exit.

            exit(0);
            
        }  

    //}
    return 0;
}

void *generator(void* args) {

    cout << "Generator Starting"<< endl;
    //Allowing main to continue setup

    sigset_t sigset;
    int sig;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    sigaddset(&sigset, SIGUSR1);
    sigaddset(&sigset, SIGUSR2);

    while(true) {
        
        struct timespec whats_a_long;
        whats_a_long.tv_sec = 0;
        whats_a_long.tv_nsec = randGenerator(0.01, 0.1) * 1000000000.0;
        nanosleep(&whats_a_long, NULL);

        
        double randNum = randGenerator(0.0, 1.0);
        if(randNum < 0.5) {
            cout << "Sent SIGUSR1 " << endl;
            pthread_kill(processType[0], SIGUSR1);
            pthread_kill(processType[1], SIGUSR1);
            pthread_kill(processType[4], SIGUSR1);

            pthread_mutex_lock(&count->mutexOne);
            count->sentSIGUSR1++;
            pthread_mutex_unlock(&count->mutexOne);
            
        }

        else if(randNum >= 0.5){

            cout << "Sent SIGUSR2 " << endl;
            pthread_kill(processType[2], SIGUSR2);
            pthread_kill(processType[3], SIGUSR2);
            pthread_kill(processType[4], SIGUSR2);

            pthread_mutex_lock(&count->mutexOne);
            count->sentSIGUSR2++;
            pthread_mutex_unlock(&count->mutexOne);

        }

        // sigwait(&sigset, &sig);
        // if(sig == SIGINT) {
        //     int detatchedVal = shmdt(count);
        //     if(detatchedVal == -1) {
        //         cout << "Failed to detatch shared memory." << endl;
        //     }
        //     exit(0);
        // }
        // if(sig == SIGUSR1) {
        //     //do nothing
        // }
        // if(sig == SIGUSR2) {
        //     //do nothing
        // }
    }

    return NULL;
}

void *signalUpdater(void* args) {

    cout << "SignalUpdater " << endl;

    sigset_t sigset;
    int sig;
    sigemptyset(&sigset);

    if(args == (void *)SIGUSR1) {
        sigaddset(&sigset, SIGUSR1);
    }
    if(args == (void *)SIGUSR2) {
        sigaddset(&sigset, SIGUSR2);
    }
    
    while(true) {
        
        cout << "Waiting... signalUpdater" << endl;
        sigwait(&sigset, &sig);
        cout << "Releasing... signalUpdater" << endl;

        if(sig == SIGUSR1) {
            cout << "Received SIGUSR1 " << endl;
            pthread_mutex_lock(&count->mutexTwo);
            count->receivedSIGUSR1++;
            pthread_mutex_unlock(&count->mutexTwo);
        }
        if(sig == SIGUSR2) {
            cout << "Received SIGUSR2 " << endl;
            pthread_mutex_lock(&count->mutexTwo);
            count->receivedSIGUSR2++;
            pthread_mutex_unlock(&count->mutexTwo);
            cout << "Finished SIGUSR2 " << endl;
        }

    }

    return NULL;
}

void *reporter(void* args) {


    sigset_t sigset;
    int sig;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);
    sigaddset(&sigset, SIGUSR2);
    sigaddset(&sigset, SIGINT);
    

    ofstream log;
    
    

    log.open("logT.txt", ios::app);
    if(!log.is_open()) {
        cout << "Couldnt open file correctly" << endl;
    }

    cout << "Reporter Starting " << endl;

    
    while(true) {

        auto timenow = chrono::system_clock::to_time_t(chrono::system_clock::now());
        
        cout << "Waiting... reporter" << endl;
        sigwait(&sigset, &sig);
        cout << "Releasing... reporter" << endl;

        if(sig == SIGUSR1) {
            cout << "\t\t\tReporting SIGUSER1 " << getppid() << " -> " << getpid() << endl;
            clock_gettime(CLOCK_REALTIME, &timeSIGUSR1);
            timeSumSIGUSR1 += timeSIGUSR1.tv_sec + timeSIGUSR1.tv_nsec;
            timeSumSIGUSR1 /= 1000000000L;
            cout << "timeSum " << timeSumSIGUSR1 << endl;
        }
        else if(sig == SIGUSR2) {
            cout << "\t\t\tReporting SIGUSER2 " << getppid() << " -> " << getpid() << endl;
            clock_gettime(CLOCK_REALTIME, &timeSIGUSR2);
            timeSumSIGUSR2 += timeSIGUSR2.tv_sec + timeSIGUSR2.tv_nsec;
            timeSumSIGUSR2 /= 1000000000L;
        }

        concurrentProcesses++;
        if(concurrentProcesses == 10) {
            pthread_mutex_lock(&count->mutexTwo);
            

            double avgTime1 = timeSumSIGUSR1/10.0;
            double avgTime2 = timeSumSIGUSR2/10.0;

            //ctime(&timenow);
            
            log << "SIGUSR1 | " << " | Sent " << count->sentSIGUSR1 << " Received " << count->receivedSIGUSR1 << " | " << "Current Time " << ctime(&timenow) << " | Avg Between Signals " << avgTime1 << endl;
            log << "SIGUSR2 | " << " | Sent " << count->sentSIGUSR2 << " Received " << count->receivedSIGUSR2 << " | " << "Current Time " << ctime(&timenow) << " | Avg Between Signals " << avgTime2 << endl;
            log << endl;

            avgTime1 = 0;
            avgTime2 = 0;
            concurrentProcesses = 0;
            pthread_mutex_unlock(&count->mutexTwo);
        }

        //pthread_mutex_unlock(&count->mutexThree);
        //yuyuyyup

    }


    return NULL;
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
    int sigReturn = pthread_sigmask(SIG_BLOCK, &sigset, NULL);
    if(sigReturn != 0) {
        cout << errno << endl;
    }
    
}

void unblock_signal(int signal) {
    sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, signal);
    int sigReturn = pthread_sigmask(SIG_UNBLOCK, &sigset, NULL);
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