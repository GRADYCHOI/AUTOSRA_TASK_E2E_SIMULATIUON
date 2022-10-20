#ifndef __SIMULATION_TYPES__HPP__

#define __SIMULATION_TYPES__HPP__

struct RequiredTime {
    long long int startTime;
    long long int endTime;
};

struct ResultInformation {
    int seedNumber;
    long long int reactionTime;
    long long int dataAge;
};

#endif