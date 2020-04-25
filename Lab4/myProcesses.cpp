#include "processHeader.h"


#define TIME_TO_RUN -1
#define MAX_SIGNAL_COUNT 100000
#define SUB_PROCESSES 8

Counters *count;

int main() {

    block_signal(SIGUSR1);
    block_signal(SIGUSR2);
    //block_signal(SIGINT);
    
    //Set-up mutexes
    pthread_mutex_t mutexOne;
    pthread_mutexattr_t attrOne;
    pthread_mutexattr_init(&attrOne);
    pthread_mutexattr_setpshared(&attrOne, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&mutexOne, &attrOne);

    pthread_mutex_t mutexTwo;
    pthread_mutexattr_t attrTwo;
    pthread_mutexattr_init(&attrTwo);
    pthread_mutexattr_setpshared(&attrTwo, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&mutexOne, &attrTwo);


    //Controlling execution duration of the program
    //time_t startTime = time(nullptr);

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
    

    pid_t mainProcess = getpid();

    //Creating processes.
    pid_t processType[8];
    for(int i = 0; i <= SUB_PROCESSES; i++) {

        cout << "Value of I = " << i << endl;
        processType[i] = fork();
        if(processType[i] == -1) {
            cout << "ERROR FORKING" << endl;
        }
        cout << "PID = " << processType[i] << endl;
        if(processType[i] == 0) {

            cout << "Value of I in child = " << i << endl;
            if(i < 2) {
                //Creates 2 signal one handlers
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
        }
        else {
            
            //THIS IS WORKING WITH WAITPID HERE
            //waitpid(processType[i], NULL, 0);
            //cout << "Reached end of process creation." << endl;
            
            //if we've completed every iteration AND we're in the original parent then continue.
            //getpid() == mainProcess
            if(i == SUB_PROCESSES) {
                //TIME_TO_RUN will be -1 when we're exiting on the condition of MAX_SIGNAL_COUNT
                if(TIME_TO_RUN == -1) {
                    while(true) {
                        if(maxCountReached()) {
                            /*
                            for(int i = 0; i < SUB_PROCESSES; i++) {
                                
                                //If max count reached, kill all child processes.
                                cout << "Attempting to kill process " << i << " " << processType[i] << endl;
                                kill(processType[i], SIGINT);
                                waitpid(processType[i], NULL, 0);
                                
                            }
                            */

                           for(int y = 0; y < SUB_PROCESSES; y++) {
                                //If max count reached kill all child processes.
                                cout << "Attempting to kill process " << y << " " << processType[y] <<  endl;
                                kill(processType[y], SIGINT);
                            }
                            //Making sure all proceess close before main closes. Purely to make the console look nicer.
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
                    
                    
                    /*
                    for(int y = SUB_PROCESSES-1; y >= 0; y--) {
                        //If max count reached kill all child processes.
                        cout << "Attempting to kill process " << y << " " << processType[y] <<  endl;
                        kill(processType[y], SIGINT);
                        //waitpid(processType[y], NULL, 0);
                    }
                    */
                    

                    //sleep(5);

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
                //waitpid(processType[i], NULL, 0);
                //wait(NULL);
            }
            //waitpid(processType[i], NULL, 0);
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

    unblock_signal(SIGINT);
    signal(SIGINT, signalHandler);

    if(value == 1) {
        signal(SIGUSR1, signalHandler);
        unblock_signal(SIGUSR1);
    }
    if(value == 2) {
        signal(SIGUSR2, signalHandler);
        unblock_signal(SIGUSR2);
        
    }
    
    while(true) {

        //cout << "\t\tSignal Updater paused " << getpid() <<  endl;
        pause();
        //cout << "\t\tSignal Updater unpaused " << getpid() <<  endl;

        if(value == 1) {
            //cout << "in signal handler 1" << endl;
            cout << "Received SIGUSR1" << endl;
            pthread_mutex_lock(&count->mutexOne);
            count->receivedSIGUSR1++;
            pthread_mutex_unlock(&count->mutexOne);
        }
        if(value == 2) {
            //cout << "in signal handler 2" << endl;
            cout << "Received SIGUSR2" << endl;
            pthread_mutex_lock(&count->mutexTwo);
            count->receivedSIGUSR2++;
            pthread_mutex_unlock(&count->mutexTwo);
        }

    }

    return;
}

void reporterHandler(int sig) {

    //cout << "Arrived " << getpid() << " in reporterHandler" << endl;
    if(sig == SIGINT) {
        cout << "Killing " << getpid() << " in reporterHandler" << endl;
        //free shared memory
        int detatchedVal = shmdt(count);
        if(detatchedVal == -1) {
            cout << "Failed to detatch shared memory." << endl;
        }
        exit(0);
    }

    if(sig == SIGUSR1) {
        //Handle reporting.
        cout << "\t\t\tReporting SIGUSER1" << endl;
        //kill(0, SIGINT);
    }
    else if(sig == SIGUSR2) {
        //Handle reporting.
        cout << "\t\t\tReporting SIGUSER2" << endl;
    }
    

}

void reporter(int value) {

    unblock_signal(SIGUSR1);
    unblock_signal(SIGUSR2);
    unblock_signal(SIGINT);
    signal(SIGINT, reporterHandler);
    signal(SIGUSR1, reporterHandler);
    signal(SIGUSR2, reporterHandler);
    
    while(true) {
        

        //cout << "\t\tReporting process paused" << endl;
        //pause();
        
        //cout << "\t\tReporting process unpaused - this is where the reporting would take place." << endl;

        //pthread_mutex_lock(&count->mutexOne);
        cout << "\t\t\t\tSent " << count->sentSIGUSR1 << " SIGUSR1 signals & received " << count->receivedSIGUSR1 << endl;
        //pthread_mutex_unlock(&count->mutexOne);

        //pthread_mutex_lock(&count->mutexTwo);
        cout << "Sent " << count->sentSIGUSR2 << " SIGUSR2 signals & received " << count->receivedSIGUSR2 << endl;
        //pthread_mutex_unlock(&count->mutexTwo);

        /*

            Handle reporting. Only keeping track of last x elements so maybe use the circular buffer from the last assignment :)

        */

    }

}

void generatorHandler(int signal) {

    //cout << "Arrived " << getpid() << " in generatorHandler." << endl;
    
    if(signal == SIGINT) {
        cout << "Killing " << getpid() << " in generatorHandler" << endl;
        int detatchedVal = shmdt(count);
        if(detatchedVal == -1) {
            cout << "Failed to detatch shared memory." << endl;
        }
        exit(0);
    }

    
}

void generator() {
    // block_signal(1);
    // block_signal(2);

    unblock_signal(SIGINT);
    signal(SIGINT, generatorHandler);

    srand(time(NULL));

    //cout << "Arrived " << getpid() << " in generator." << endl;
    int x = 0;
    while(true) {

        double randNum = randGenerator(0.0, 1.0);
        if(randNum < 0.5) {
            //Signal SIGUSR1
            //cout << "Sending SIGUSR1" << endl;
            cout << "Sent SIGUSR1" << endl;
            pthread_mutex_lock(&count->mutexOne);
            count->sentSIGUSR1++;
            pthread_mutex_unlock(&count->mutexOne);
            kill(0, SIGUSR1);
        }
        else if(randNum >= 0.5){
            //Signal SIGUSR2
            //cout << "Sending SIGUSR2" << endl;
            cout << "Sent SIGUSR2" << endl;
            pthread_mutex_lock(&count->mutexTwo);
            count->sentSIGUSR2++;
            pthread_mutex_unlock(&count->mutexTwo);
            kill(0, SIGUSR2);

        }
        sleep(randGenerator(0.01, 0.1));
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