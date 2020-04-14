### Program Design Ideas - Threads

#### Requirements
* 4 signal handleing processes.
* 3 signal generating processes.
* 1 reporting process.
* 1 Parent process :)


#### Main Process

Parent Process
* Responsibilities
  * creates the remaining 8 processes.
  * controls the execution time of the child processes and waits for their completion.
  * uses <u>2</u> signals for comminication
    * SIGUSR1 or SIGUSR2

Signal Generating Processes
* Responsibilities
  * Enforce Mutual Exclusion
  * Run in a loop and randomly select SIGUSR1 or SIGUSR2 to send to the processes in its group
  * Each loop iteration should be randomly delayed between 0.01 and 0.1 seconds
  * After a signal has been generated increment a <u>shared</u> signal sent counter for that signal type
  * ^^ Each signal type gets its own counter. (Types being SIGUSR1 & SIGUSR2)


Signal Handling Processes
* Responsibilities
  * Enforce Mutual Exclusion
  * Each process will only handle one type of signal
  * Each signal handling process also gets run in a loop
  * Wait for signals to occur
  * When a process arrives increment a <u>shared</u> signal received counter

Signal Reporting Process
* Responsibilities
  * Enforce Mutual Exclusion
  * Executes in a loop & waits for signals
  * Responds to both types of signals
  * For every 10 signals it handles it reports the system time and the count of each signal type
  * Report the average time between receptions of each signal type between those 10 signals mentioned above
  * Note: this requres the reporting process to remember the time of occurence of each signal type.
