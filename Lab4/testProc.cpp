#include "processHeader.h"


#define TIME_TO_RUN 30
#define MAX_SIGNAL_COUNT 100000
#define SUB_PROCESSES 8

Counters *count;
int concurrentProcesses;

struct timespec timeSIGUSR1;
double timeSumSIGUSR1;

struct timespec timeSIGUSR2;
double timeSumSIGUSR2;


int main() {

    srand(time(NULL));

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


    //Controlling execution duration of the program
    //time_t startTime = time(nullptr);

    //Creating processes.
    pid_t processType[8];
    for(int i = 0; i < SUB_PROCESSES; i++) {

        cout << "Value of I in Parent = " << i << endl;
        processType[i] = fork();
        if(processType[i] == -1) {
            cout << "ERROR FORKING" << endl;
        }
        //cout << "PID = " << processType[i] << endl;
        if(processType[i] == 0) {
            //unblock_signal(SIGUSR1);
            //unblock_signal(SIGUSR2);
            //cout << "Creating: " << i << " " << getppid() << " -> " << getpid() << endl;
            cout << "Value of I in Child = " << i << " " << getppid() << " -> " << getpid() << endl;
            if(i < 3) {
                cout << "Creating sigGen" << endl;
                generator();
                
            }
            else if(i >= 3 && i < 5) {
                cout << "Creating sigOne Handlers " << processType[i] << endl;
                unblock_signal(SIGUSR1);
                
                while(true) {
                    signal(SIGUSR1, signalHandler);
                    pause();
                }
            }
            else if(i >= 5 && i < 7) {
                cout << "Creating sigTwo Handlers " << processType[i] << endl;
                unblock_signal(SIGUSR2);
                
                while(true) {
                    signal(SIGUSR2, signalHandler);
                    pause();
                }
                
            }
            else if(i == 7) {
                cout << "Creating reporter" << endl;
                unblock_signal(SIGUSR1);
                unblock_signal(SIGUSR2);
                
                while(true) {
                    signal(SIGUSR1, reporterHandler);
                signal(SIGUSR2, reporterHandler);
                    pause();
                }
            }
            if(i == 7) {
                waitpid(processType[i], NULL, 0);
            }
            
        }
        else {
            if(i == SUB_PROCESSES-1) {

                if(TIME_TO_RUN == -1) {
                    while(true) {
                        if(maxCountReached()) {

                           for(int y = 0; y < SUB_PROCESSES; y++) {
                                //If max count reached kill all child processes.
                                cout << "Attempting to kill process " << y << " " << processType[y] <<  endl;
                                kill(processType[y], SIGTERM);
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
                    sleep(TIME_TO_RUN);

                    for(int y = 0; y < SUB_PROCESSES; y++) {
                        //If max count reached kill all child processes.
                        cout << "Attempting to kill process " << y << " " << processType[y] <<  endl;
                        kill(processType[y], SIGTERM);
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

            //sleep(1);
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
        //exit(0);
    }
    if(signal == SIGUSR1) {
        cout << "\t\t\tReceived SIGUSER1 in signalHandler" << endl;
        pthread_mutex_lock(&count->mutexOne);
        count->receivedSIGUSR1++;
        pthread_mutex_unlock(&count->mutexOne);
    }
    else if(signal == SIGUSR2) {
        cout << "\t\t\tReceived SIGUSER2 in signalHandler" << endl;
        pthread_mutex_lock(&count->mutexTwo);
        count->receivedSIGUSR2++;
        pthread_mutex_unlock(&count->mutexTwo);
    }
    
}

void signalUpdater(int value) {

    cout << "SignalUpdater " << getppid() << " -> " << getpid() << endl;

    //unblock_signal(SIGINT);
    //signal(SIGINT, signalHandler);

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
            pthread_mutex_lock(&count->mutexOne);
            count->receivedSIGUSR1++;
            pthread_mutex_unlock(&count->mutexOne);
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
    ofstream log;

    log.open("logP.txt", ios::app);
    if(!log.is_open()) {
        cout << "Couldnt open file correctly" << endl;
    }

    cout << "ReporterHandler " << getppid() << " -> " << getpid() << endl;
    if(sig == SIGINT) {
        cout << "Killing " << getppid() << " -> " << getpid() << " in reporterHandler" << endl;
        //free shared memory
        int detatchedVal = shmdt(count);
        if(detatchedVal == -1) {
            cout << "Failed to detatch shared memory." << endl;
        }
        //exit(0);
    }
    cout << "CONCURRENT PROC UP" << endl;
    concurrentProcesses++;
    if(sig == SIGUSR1) {
        cout << "REPORT SIG 1" << endl;
        clock_gettime(CLOCK_REALTIME, &timeSIGUSR1);
        timeSumSIGUSR1 += timeSIGUSR1.tv_sec + timeSIGUSR1.tv_nsec;
        timeSumSIGUSR1 /= 1000000000L;
         cout << "REPORT SIG 1 end" << endl;
    }
    else if(sig == SIGUSR2) {
        cout << "REPORT SIG 2" << endl;
        clock_gettime(CLOCK_REALTIME, &timeSIGUSR2);
        timeSumSIGUSR2 += timeSIGUSR2.tv_sec + timeSIGUSR2.tv_nsec;
        timeSumSIGUSR1 /= 1000000000L;
        cout << "REPORT SIG 2 end" << endl;
    }

    if(concurrentProcesses == 10) {
        pthread_mutex_lock(&count->mutexTwo);
        
        time_t avgTime1 = timeSumSIGUSR1/10.0;
        time_t avgTime2 = timeSumSIGUSR2/10.0;

        auto timenow = chrono::system_clock::to_time_t(chrono::system_clock::now());

        log << "SIGUSR1 | " << " | Sent " << count->sentSIGUSR1 << " Received " << count->receivedSIGUSR1 << " | " << "Current Time " << ctime(&timenow) << " | Avg Between Signals " << avgTime1 << endl;
        log << "SIGUSR2 | " << " | Sent " << count->sentSIGUSR2 << " Received " << count->receivedSIGUSR2 << " | " << "Current Time " << ctime(&timenow) << " | Avg Between Signals " << avgTime2 << endl;
        log << endl;



        timeSumSIGUSR1 = 0;
        timeSumSIGUSR2 = 0;
        avgTime1 = 0;
        avgTime2 = 0;
        concurrentProcesses = 0;

        pthread_mutex_unlock(&count->mutexTwo);
    }

    //log.close();

    
}

void reporter(int value) {

    

}

void generatorHandler(int signal) {

    cout << "Generator Handler " << getppid() << " -> " << getpid() << endl;
    
    if(signal == SIGINT) {
        cout << "Killing " << getppid() << " -> " << getpid() << " in generatorHandler" << endl;
        int detatchedVal = shmdt(count);
        if(detatchedVal == -1) {
            cout << "Failed to detatch shared memory." << endl;
        }
        //exit(0);
    }

    
}

void generator() {
    //sleep(2);

    //cout << "Generator " << getppid() << " -> " << getpid() << endl;

    //signal(SIGINT, generatorHandler);

    

    int x = 0;
    while(true) {

        struct timespec whats_a_long;
        whats_a_long.tv_sec = 0;
        whats_a_long.tv_nsec = randGenerator(0.01, 0.1) * 1000000000L;
        //whats_a_long.tv_nsec = 0.01 * 1000000000L;
        //whats_a_long.tv_nsec = 1;
        nanosleep(&whats_a_long, NULL);
        double randNum = randGenerator(0.0, 1.0);
        if(randNum < 0.5) {
            kill(0, SIGUSR1);
            cout << "Sent SIGUSR1 " << getppid() << " -> " << getpid() << endl;
            pthread_mutex_lock(&count->mutexOne);
            count->sentSIGUSR1++;
            pthread_mutex_unlock(&count->mutexOne);
            
        }
        else if(randNum >= 0.5){
            kill(0, SIGUSR2);
            cout << "Sent SIGUSR2 " << getppid() << " -> " << getpid() << endl;
            pthread_mutex_lock(&count->mutexTwo);
            count->sentSIGUSR2++;
            pthread_mutex_unlock(&count->mutexTwo);
            
        }
        
    }
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
        cout << "NOPEEEE" << endl;
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

