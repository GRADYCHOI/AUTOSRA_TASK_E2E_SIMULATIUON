#ifndef __SIMULATION_TYPES__HPP__

#define __SIMULATION_TYPES__HPP__

struct ExecutionInformation {
    double startTime;
    double endTime;
};

struct ResultInformation {
    int sequenceIndex;
    double reactionTime;
    double dataAge;
};

#endif