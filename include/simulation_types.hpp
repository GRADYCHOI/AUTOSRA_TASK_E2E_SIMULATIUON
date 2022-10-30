#ifndef __SIMULATION_TYPES__HPP__

#define __SIMULATION_TYPES__HPP__


// this exists for using Runnable class
class RUNNABLE;

struct RequiredTime {
    long long int startTime;
    long long int endTime;
};

struct ResultInformation {
    int seedNumber;
    long long int reactionTime;
    long long int dataAge;
};

struct VisitedInformation {
    std::shared_ptr<RUNNABLE> runnable;
    int inputRunnableCycle;
    int worstCycle;
    long long int startTime;
    long long int endTime;
};

#endif