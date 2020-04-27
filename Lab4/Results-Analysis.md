# Lab 4: Signaling With Multi-Process and Multi-Threaded Programs
## Program Description

### Introduction

This lab required us to use signals for interprocess and interthread communication. These two types of communication were implemented in two different programs both with the goal of completing the same task. Both programs create a thread or processes respectively. Each thread has a main function, a reporting function, three signal counting functions, and three signal generating functions.

* main - creates the threads / processes and determines when the program should stop.
* reporter - receives signals and writes useful information / stats to a log file.
* signal receivers - receives and counts signals.
* generator - creates and sends signals.

### Testing

Testing this lab became tedious as I continued to develop these applications. My method for developing and testing these applications were the same for both implementations.

First I set the program to only run for 5 seconds and also capped the total number of singals generated to a fixed number. Once everything worked with those conditions I allowed the program to run for 10 seconds and increased the generator cap. I kept the time at 10 seconds and removed the generator cap once everything was working. Lastly I allowed the program to run for 30 seconds, as indicated in the lab document.

Once I knew my program worked with a time limit cap, I removed the time cap and added code that would end the program once 100,000 signals were received. (Did this first because we expect to recieve double the amount of signals than we send). My method of testing was to allow the program to end once a total of 100,000 signals were sent.

I took this approach because it allowed me to make quick adjustments to the program without having to wait 30 seconds or 15 minutes (the time it takes to receive 100k signals) just to find out whether my program worked or not.


### Results

<ins>Multi-Process Implementation</ins>

<details>
    <summary>
        Program running for 30 seconds.
    </summary>
    <a href="Results/Process30sec.txt" target="_blank">Log_30sec.txt</a>
    
</details>

<details>
    <summary>
    Program running for 100 signals received in total.
    </summary>
    <a href="Results/Process100.txt" target="_blank">Log_100.txt</a>
</details>

<details>
    <summary>
    Program running for 100 signals sent in total.
    </summary>
    <a href="Results/Process100Sebt.txt" target="_blank">Log_100Sent.txt</a>
</details>

<ins>Multi-Threaded Implementation</ins>

<details>
    <summary>
    Program running for 30 seconds.
    </summary>
    <a href="Results/Process30sec.txt" target="_blank">Log_30sec.txt</a>
</details>

<details>
    <summary>
    Program running for 100 signals received in total.
    </summary>
    <a href="Results/Process30sec.txt" target="_blank">Log_100.txt</a>
</details>

<details>
    <summary>
    Program running for 100 signals sent in total.
    </summary>
    <a href="Results/Process30sec.txt" target="_blank">Log_100Sent.txt</a>
</details>

### Analysis

* Sent vs Received
  *  The first thing we notice is that the number of signals received is almost exactly double the number of signals sent. This is due to the fact that we generate one of two types of signals with each type of signal having two signal receivers. 
  *  We have no way of checking whether one receiver has already received the generated signal so we end up with both signal receivers counting one signal. 
* Lost signals
  * Our reporter function updates a log for every 10 signals received.
  * We typically see only a loss of 2-3 signals at the end of the log at process termination. I believe this is due to the fact that the reporting process may terminate while there are signals waiting to be written, or while in the middle of counting to 10.
