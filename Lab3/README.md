# Lab 3: Multi-Threaded Network Spell Checker
## Author: Luke Dependahl


### Program Desciption

The project is broken into two parts. The server and the client.
The server accepts connections and the client connects. These are two seperate applications and do not depend on each other at all, however the client was developed with our server in mind.


#### Server

The is driven by multiple threads. Main, workers and a logger.
Main sets up network settings and accepts connection from clients.
When a connection is accepted it's socket is placed in what is essentially a connection queue that is implemented with a circular buffer, CBuffer.

The worker threads check for content in CBuffer and if there is a new connection it's sent to clientServicer() to be processed. The processing of servicing clients is just a loop that accepts messages in the form of single words from a client through a socket.

After a word is received it is spell checked and the result of spell checking is put into a log queue that is implemented with a simple vector.

#### Client

Similarly the client is also driven by threads. myClient MUST be passed an address and a port when run. When myClient starts, X number of threads are spawned. These threads establish a connection to the server with information specified in the command line arguments. Once a connection has been established, it randomly picks a word from a mock dictionary and sends it to the server for spell checking. The PID of the calling thread, a time stamp and the result of spell checking are all added to a client-log.