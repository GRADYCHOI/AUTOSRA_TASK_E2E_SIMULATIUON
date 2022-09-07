#ifndef __SIMULATION_TYPES__HPP__

#define __SIMULATION_TYPES__HPP__

struct ExecutionInformation {
    float startTime;
    float endTime;
};

struct ResultInformation {
    int sequenceIndex;
    float reactionTime;
    float dataAge;
};

#endif