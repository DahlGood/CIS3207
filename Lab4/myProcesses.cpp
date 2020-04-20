#include "processHeader.h"


#define TIME_TO_RUN 5
#define MAX_SIGNAL_COUNT 100000
#define SUB_PROCESSES 8

//Counters *count;

int main() {

    //Setting up signals
    //All processes respond to SIGINT, SIGINT occurs when a user manually exits, when time has run out and when max signals are sent.
    //Stop after user input not just ctrl+c
    //signal(SIGINT, signalHandler);
    //signal(SIGINT, signalGenerator);
    //signal(SIGINT, reporter);

    //Signal Handlers accept SIGUSR1 and SIGUSR2 (Depending on the process running one type will be blocked for that specific process.)
    //signal(SIGUSR1, signalHandler);
    //signal(SIGUSR2, signalHandler);
    
    //Counters *count = (Counters *)malloc(sizeof(Counters));

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
    /*
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
    */

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
                block_signal(2);
                signal(SIGUSR1, signalHandler);
                signal(SIGINT, signalHandler);
                //pause();
            }
            else if(i >= 2 && i < 4) {
                
                cout << "Creating sigTwo Handlers " << processType[i] << endl;
                //Creates 2 signal two handlers
                block_signal(1);
                signal(SIGUSR2, signalHandler);
                signal(SIGINT, signalHandler);
                //pause();
            }
            else if(i == 4) {
                
                //Creates 1 reporting process
                cout << "Creating reporter" << endl;
                signal(SIGINT, reporter);
                //pause();
            }
            else if(i > 4 && i < 8) {
                
                //Creates 3 signal generators
                cout << "Creating sigGen" << endl;
                signal(SIGINT, signalGenerator);
                //pause();
            }
        }
        else {
            waitpid(processType[i], NULL, 0);
            cout << "Reached end of process creation." << endl;
            
            //if we've completed every iteration AND we're in the original parent then continue.
            if(i == SUB_PROCESSES) { 
                //TIME_TO_RUN will be -1 when we're exiting on the condition of MAX_SIGNAL_COUNT
                if(TIME_TO_RUN == -1) {
                    while(true) {
                        if(maxCountReached()) {
                            for(int i = 0; i < SUB_PROCESSES; i++) {
                                
                                //If max count reached, kill all child processes.
                                cout << "Attempting to kill process " << i << " " << processType[i] << endl;
                                kill(processType[i], SIGINT);
                                
                            }
                            //Free shared memory in parent and exit.
                            //shmdt(count);
                            exit(0);
                        }
                    }
                }
                else {
                    sleep(TIME_TO_RUN);
                    cout << "MAIN FUNCTION PID " << getpid() << endl;
                    for(int y = SUB_PROCESSES-1; y > 0; y--) {
                        //If max count reached kill all child processes.
                        cout << "Attempting to kill process " << y << " " << processType[y] <<  endl;
                        kill(processType[y], SIGINT);
                    }
                    //Free shared memory in parent.
                    //shmdt(count);
                    cout << "\t\t\tEXITING" << endl;
                    exit(0);
                    
                }  

                //waitpid(processType[i], NULL, 0);
                
                    
                
                //waitpid(processType[i], NULL, 0);
                //waitpid(-1, NULL, 0);
                //wait(NULL);
            }
            //waitpid(-1, NULL, 0);
            //wait(NULL);
            //waitpid(processType[i], NULL, 0);
        }
    }

    //shmdt(count);
    //exit(0);

    return 0;
}

void signalGenerator(int signal) {

    cout << "Arrived " << getpid() << " in signalGenerator." << endl;
    
    srand(time(NULL));

    while(true) {

        if(signal == SIGINT) {
            cout << "Killing " << getpid() << " in signalGenerator" << endl;
            //shmdt(count);
            exit(0);
        }

        //Do stuff
        double randNum = randGenerator(0, 1);
        if(randNum < 0.5) {
            //Signal SIGUSR1
            cout << "Sending signal to SIGUSR1" << endl;
            kill(getpid(), SIGUSR1);
            //sentSIGUSR1++ INSURE MUTEX
        }
        else if(randNum >= 0.5){
            //Signal SIGUSR2
            cout << "Sending signal to SIGUSR2" << endl;
            kill(getpid(), SIGUSR2);
            //sentSIGUSR2++ INSURE MUTEX
        }
        

        sleep(randGenerator(0.01, 0.1));

    }
}

void signalHandler(int signal) {

    cout << "Arrived " << getpid() << " in signalHandler" << endl;
    
    while(true) {

        if(signal == SIGINT) {
            cout << "Killing " << getpid() << " in signalHandler" << endl;
            //shmdt(count);
            exit(0);
        }

        if(signal == SIGUSR1) {
            //mutex
            //pthread_mutex_lock(&count->mutexOne);
            cout << "Incrementing SIGUSER1" << endl;
            //count->receivedSIGUSR1++;
            //pthread_mutex_unlock(&count->mutexOne);
        }
        else if(signal == SIGUSR2) {
            //mutex
            //pthread_mutex_lock(&count->mutexTwo);
            cout << "Incrementing SIGUSER2" << endl;
            //count->receivedSIGUSR2++;
            //pthread_mutex_unlock(&count->mutexTwo);
        }

    }
    
}

void reporter(int sig) {

    cout << "Arrived " << getpid() << " in reporter" << endl;

    while(true) {

        if(sig == SIGINT) {
            cout << "Killing " << getpid() << " in reporter" << endl;
            //shmdt(count);
            exit(0);
        }

        if(sig == SIGUSR1) {
            //Handle reporting.
            cout << "Handling SIGUSER1 in reporter" << endl;
        }
        else if(sig == SIGUSR2) {
            //Handle reporting.
            cout << "Handling SIGUSER2 in reporter" << endl;
        }

    }

}

double randGenerator(double lowerBound, double upperBound) {
    cout << "Arrived in randGenerator" << endl;
    return (fmod(rand(), upperBound) + lowerBound);
}

void block_signal(int x) {
    cout << "Arrived in block_signal " << x << endl;
    int returnVal;
    sigset_t sigset;
    sigemptyset(&sigset);
    if(x == 1) {
        sigaddset(&sigset, SIGUSR1);
        returnVal = sigprocmask(SIG_BLOCK, &sigset, NULL);
    }
    else {
        sigaddset(&sigset, SIGUSR2);
        returnVal = sigprocmask(SIG_BLOCK, &sigset, NULL);
    }

    if(returnVal != 0) {
        cout << errno << endl;
    }
    
}


bool maxCountReached() {
    cout << "Arrived in maxCountReached" << endl;
    //"You should run the program for a total of 100,000 signals" I'm going to take this as meaning the total signals sent being 100,000.
    /*
    int total = (count->sentSIGUSR1 + count->sentSIGUSR2);
    if(total >= MAX_SIGNAL_COUNT) {
        return true;
    }
    */
    
    return false;
}
