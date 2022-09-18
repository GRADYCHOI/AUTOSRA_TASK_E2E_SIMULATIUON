#ifndef __SIMULATION_TYPES__HPP__

#define __SIMULATION_TYPES__HPP__

struct ExecutionInformation {
    long long int startTime;
    long long int endTime;
};

struct ResultInformation {
    int inputRunnableId;
    int outputRunnableId;
    int sequenceIndex;
    int reactionTime;
    int dataAge;
};

#endif