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
                signalUpdater(1);
                //generator();
                //pause();
            }
            else if(i >= 2 && i < 4) {
                cout << "Creating sigTwo Handlers " << processType[i] << endl;
                //Creates 2 signal two handlers
                signalUpdater(2);
                //pause();
            }
            else if(i == 4) {
                
                //Creates 1 reporting process
                cout << "Creating reporter" << endl;
                reporter(1);
                //pause();
            }
            else if(i > 4 && i < 8) {
                
                //Creates 3 signal generators
                cout << "Creating sigGen" << endl;
                generator();
                //signalUpdater(1);
                //pause();
            }
        }
        else {
            //THIS IS WORKING WITH WAITPID HERE
            //waitpid(processType[i], NULL, 0);
            //cout << "Reached end of process creation." << endl;
            
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
                    for(int y = 0; y < SUB_PROCESSES; y++) {
                        //If max count reached kill all child processes.
                        cout << "Attempting to kill process " << y << " " << processType[y] <<  endl;
                        kill(processType[y], SIGINT);
                    }
                    //Free shared memory in parent.
                    //shmdt(count);
                    cout << "\t\t\tEXITING" << endl;
                    exit(0);
                    
                }  
                wait(NULL);
            }
            //waitpid(processType[i], NULL, 0);
        }
    }

    //shmdt(count);
    //exit(0);

    return 0;
}

void signalHandler(int signal) {

    if(signal == SIGINT) {
        cout << "Killing " << getpid() << " in signalHandler" << endl;
        //shmdt(count);
        exit(0);
    }
    if(signal == SIGUSR1) {
        cout << "\t\t\tIncrementing SIGUSER1 in signalHandler" << endl;
    }
    else if(signal == SIGUSR2) {
        cout << "\t\t\tIncrementing SIGUSER2 in signalHandler" << endl;
    }
    
}

void signalUpdater(int value) {
    
    if(value == 1) {
        block_signal(2);
        signal(SIGUSR1, signalHandler);
        signal(SIGINT, signalHandler);

    }
    if(value == 2) {
        block_signal(1);
        signal(SIGUSR2, signalHandler);
        signal(SIGINT, signalHandler);

    }

    while(true) {

        cout << "\t\tSignal Updater paused" <<  endl;
        pause();
        cout << "\t\tSignal Updater unpaused" <<  endl;

        if(value == 1) {
            //pthread_mutex_lock(&count->mutexOne);
            cout << "\t\t\tIncrementing SIGUSER1 in signalUpdater" << endl;
            //count->receivedSIGUSR1++;
            //pthread_mutex_unlock(&count->mutexOne);
        }
        if(value == 2) {
            //pthread_mutex_lock(&count->mutexTwo);
            cout << "\t\t\tIncrementing SIGUSER2 in signalUpdater" << endl;
            //count->receivedSIGUSR2++;
            //pthread_mutex_unlock(&count->mutexTwo);
        }

    }

    return;
}

void reporterHandler(int sig) {

    cout << "Arrived " << getpid() << " in reporterHandler" << endl;
    if(sig == SIGINT) {
        cout << "Killing " << getpid() << " in reporterHandler" << endl;
        //free shared memory
        //cout << "In report handler, pid+1 = " << getpid()+1 << endl;
        //kill(getpid()+1, SIGINT);
        exit(0);
    }

    if(sig == SIGUSR1) {
        //Handle reporting.
        cout << "\t\t\tHandling SIGUSER1 in reporterHandler" << endl;
    }
    else if(sig == SIGUSR2) {
        //Handle reporting.
        cout << "\t\t\tHandling SIGUSER2 in reporterHandler" << endl;
    }
    

}

void reporter(int value) {

    signal(SIGINT, reporterHandler);
    signal(SIGUSR1, reporterHandler);
    signal(SIGUSR2, reporterHandler);

    while(true) {

        cout << "\t\tReporting process paused" << endl;
        pause();

        cout << "\t\tReporting process unpaused - this is where the reporting would take place." << endl;

        /*

            Handle reporting. Only keeping track of last x elements so maybe use the circular buffer from the last assignment :)

        */

    }

}

void generatorHandler(int signal) {

    cout << "Arrived " << getpid() << " in generatorHandler." << endl;
    
    if(signal == SIGINT) {
        cout << "Killing " << getpid() << " in generatorHandler" << endl;
        //shmdt(count);
        exit(0);
    }

    
}

void generator() {

    cout << "Arrived " << getpid() << " in generator." << endl;

    signal(SIGINT, generatorHandler);

    srand(time(NULL));
    while(true) {

        //Do stuff
        double randNum = randGenerator(0, 1);
        if(randNum < 0.5) {
            //Signal SIGUSR1
            cout << "\t\t\tSending signal to SIGUSR1 from generator" << endl;
            kill(1, SIGUSR1);
            //sentSIGUSR1++ INSURE MUTEX
        }
        else if(randNum >= 0.5){
            //Signal SIGUSR2
            cout << "\t\t\tSending signal to SIGUSR2 from generator" << endl;
            kill(1, SIGUSR2);
            //sentSIGUSR2++ INSURE MUTEX
        }
        

        sleep(randGenerator(0.01, 0.1));

    }
}

double randGenerator(double lowerBound, double upperBound) {
    //cout << "Arrived in randGenerator" << endl;
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