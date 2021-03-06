#include "Simulator.h"
#include <fstream>
#include <string.h>


void writeStats() {
    //Opening a file.
    ofstream io;
    //Making sure file was opened correctly.
    io.open("stats.txt", ios::app);
    if(!io.is_open()) {
        cout << "Didn't open file correctly.";
        exit(EXIT_FAILURE);
    }
    io << "---------- Queues ----------" << endl;
    io << "Avg Event Queue Size: " << getAvgSizeEQ() << endl;
    io << "Max Event Queue Size: " << getMaxSizeEQ() << endl;
    io << "Avg CPU Queue Size: " << getAvgSizeCPUQ() << endl;
    io << "Max CPU Queue Size: " << getMaxSizeCPUQ() << endl;
    io << "Avg Disk1 Queue Size: " << getAvgSizeD1Q() << endl;
    io << "Max Disk1 Queue Size: " << getMaxSizeD1Q() << endl;
    io << "Avg Disk2 Queue Size: " << getAvgSizeD2Q() << endl;
    io << "Max Disk2 Queue Size: " << getMaxSizeD2Q() << endl;
    io << "Avg Network Queue Size: " << getAvgSizeNQ() << endl;
    io << "Max Network Queue Size: " << getMaxSizeNQ() << endl;
    io << "" << endl;

    io << "---------- Utilization ----------" << endl;
    io << "Utilization of CPU: " << getUtilCPU() << endl;
    io << "Utilization of Disk1: " << getUtilD1() << endl;
    io << "Utilization of Disk2: " << getUtilD2() << endl;
    io << "Utilization of Network: " << getUtilN() << endl;
    io << "" << endl;

    io << "---------- Response Time ----------" << endl;
    io << "Avg Response Time of CPU: " << getAvgRespCPU() << endl;
    io << "Max Response Time of CPU: " << getMaxRespCPU() << endl;
    io << "Avg Response Time of Disk1: " << getAvgRespD1() << endl;
    io << "Max Response Time of Disk1: " << getMaxRespD1() << endl;
    io << "Avg Response Time of Disk2: " << getAvgRespD2() << endl;
    io << "Max Response Time of Disk2: " << getMaxRespD2() << endl;
    io << "Avg Response Time of Network: " << getAvgRespN() << endl;
    io << "Max Response Time of Network: " << getMaxRespN() << endl;
    io << "" << endl;

    io << "---------- Throughput ----------" << endl;
    io << "Throughput of CPU: " << getTHPCPU() << endl;
    io << "Throughput of Disk1: " << getTHPD1() << endl;
    io << "Throughput of Disk2: " << getTHPD2() << endl;
    io << "Throughput of Network: " << getTHPN() << endl;
    io << "" << endl;

    io.close();
}

//Here include all values that need to be totalled. The functions in the file should be run at event finish.
int totalSizeEQ = 0;
int totalSizeCPUQ = 0;
int totalSizeD1Q = 0;
int totalSizeD2Q = 0;
int totalSizeNQ = 0;

int totalUsedEQ = 0;
int totalUsedCPUQ = 0;
int totalUsedD1Q = 0;
int totalUsedD2Q = 0;
int totalUsedNQ = 0;

int totalUsedCPU = 0;
int totalUsedD1 = 0;
int totalUsedD2 = 0;
int totalUsedN = 0;

int maxSizeEQ = 0;
int maxSizeCPUQ = 0;
int maxSizeD1Q = 0;
int maxSizeD2Q = 0;
int maxSizeNQ = 0;

int avgSizeEQ = 0;
int avgSizeCPUQ = 0;
int avgSizeD1Q = 0;
int avgSizeD2Q = 0;
int avgSizeNQ = 0;

int utilCPU = 0;
int utilD1 = 0;
int utilD2 = 0;
int utilN = 0;

int maxResponseEQ = 0;
int maxResponseCPUQ = 0;
int maxResponseD1Q = 0;
int maxResponseD2Q = 0;
int maxResponseNQ = 0;

int avgResponseEQ = 0;
int avgResponseCPUQ = 0;
int avgResponseD1Q = 0;
int avgResponseD2Q = 0;
int avgResponseNQ = 0;

int totalResponseEQ = 0;
int totalResponseCPUQ = 0;
int totalResponseD1Q = 0;
int totalResponseD2Q = 0;
int totalResponseNQ = 0;

int thpEQ = 0;
int thpCPUQ = 0;
int thpD1Q = 0;
int thpD2Q = 0;
int thpNQ = 0;

void setEQ(int size) {
    totalSizeEQ += size;
    if(size > maxSizeEQ) {
        maxSizeEQ = size;
    }

}

void setCPUQ(int size) {
    totalSizeCPUQ += size;
    if(size > maxSizeCPUQ) {
        maxSizeCPUQ = size;
    }
}

void setD1Q(int size) {
    totalSizeD1Q += size;
    if(size > maxSizeD1Q) {
        maxSizeD1Q = size;
    }
}

void setD2Q(int size) {
    totalSizeD2Q += size;
    if(size > maxSizeD2Q) {
        maxSizeD2Q = size;
    }
}

void setNQ(int size) {
    totalSizeNQ += size;
    if(size > maxSizeNQ) {
        maxSizeNQ = size;
    }
}


void setTimeCPUQ(int respTime) {
    totalResponseCPUQ += respTime;
    if(respTime > maxResponseCPUQ) {
        maxResponseCPUQ = respTime;
    }
}
void setTimeD1Q(int respTime) {
    totalResponseD1Q += respTime;
    if(respTime > maxResponseD1Q) {
        maxResponseD1Q = respTime;
    }
}
void setTimeD2Q(int respTime) {
    totalResponseD2Q += respTime;
    if(respTime > maxResponseD2Q) {
        maxResponseD2Q = respTime;
    }
}
void setTimeNQ(int respTime) {
    totalResponseNQ += respTime;
    if(respTime > maxResponseNQ) {
        maxResponseNQ = respTime;
    }
}

void setUsedEQ() {
    totalUsedEQ++;
}
void setUsedCPUQ() {
    totalUsedCPUQ++;
}
void setUsedD1Q() {
    totalUsedD1Q++;
}
void setUsedD2Q() {
    totalUsedD2Q++;
}
void setUsedNQ() {
    totalUsedNQ++;
}

void setUsedCPU() {
    totalUsedCPU++;
}
void setUsedD1() {
    totalUsedD1++;
}
void setUsedD2() {
    totalUsedD2++;
}
void setUsedN() {
    totalUsedN++;
}



double getAvgSizeEQ() {
    return totalSizeEQ / totalUsedEQ;
}
double getAvgSizeCPUQ() {
    return totalSizeCPUQ / totalUsedCPUQ;
}
double getAvgSizeD1Q() {
    return totalSizeD1Q / totalUsedD1Q;
}
double getAvgSizeD2Q() {
    return totalSizeD2Q / totalUsedD2Q;
}
double getAvgSizeNQ() {
    return totalSizeNQ / totalUsedNQ;
}

double getMaxSizeEQ() {
    return maxSizeEQ;
}
double getMaxSizeCPUQ() {
    return maxSizeCPUQ;
}
double getMaxSizeD1Q() {
    return maxSizeD1Q;
}
double getMaxSizeD2Q() {
    return maxSizeD2Q;
}
double getMaxSizeNQ() {
    return maxSizeNQ;
}

double getUtilCPU() {
    return totalResponseCPUQ / getFIN_TIME();
}
double getUtilD1() {
    return totalResponseD1Q / getFIN_TIME();
}
double getUtilD2() {
    return totalResponseD2Q / getFIN_TIME();
}
double getUtilN() {
    return totalResponseNQ / getFIN_TIME();
}

double getAvgRespCPU() {
    return (double)totalResponseCPUQ / (double)totalUsedCPUQ;
    
}
double getAvgRespD1() {
    return (double)totalResponseD1Q / (double)totalUsedD1Q;
}
double getAvgRespD2() {
    return (double)totalResponseD2Q / (double)totalUsedD2Q;
}
double getAvgRespN() {
    return (double)totalResponseNQ / (double)totalUsedNQ;
}

double getMaxRespCPU() {
    return maxResponseCPUQ;
}
double getMaxRespD1() {
    return maxResponseD1Q;
}
double getMaxRespD2() {
    return maxResponseD2Q;
}
double getMaxRespN() {
    return maxResponseNQ;
}

double getTHPCPU() {
    return totalUsedCPU / getFIN_TIME();
}
double getTHPD1() {
    return totalUsedD1 / getFIN_TIME();
}
double getTHPD2() {
    return totalUsedD2 / getFIN_TIME();
}
double getTHPN() {
    return totalUsedN / getFIN_TIME();
}



