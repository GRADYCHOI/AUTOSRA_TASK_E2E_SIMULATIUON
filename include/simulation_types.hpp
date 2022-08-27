#ifndef __SIMULATION_TYPES__HPP__

#define __SIMULATION_TYPES__HPP__

struct RunnableInformation {
    int taskId;
    int priority;
    int status;
    double period;
    double offset;
    double executionTime;
};

struct ExecutionInformation {
    double startTime;
    double endTime;
};

struct CommunicationInformation {
    double readTime;
    double writeTime;
};

#endif