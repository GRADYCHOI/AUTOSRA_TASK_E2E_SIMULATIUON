#ifndef __COMMUNICATION__HPP__

#define __COMMUNICATION__HPP__

#include "DAG.hpp"
#include "simulation_types.hpp"


class Communication {
public:
    Communication() {}
    virtual ~Communication() {}

    virtual void GetCommunicationTable(RunnableInformation* runnableInformations, ExecutionInformation* runnableExecutions, int numberOfRunnables, int maxCycle, CommunicationInformation* runnableCommunications) = 0;
};

class RunnableImplicit : public Communication {
public:
    void GetCommunicationTable(RunnableInformation* runnableInformations, ExecutionInformation* runnableExecutions, int numberOfRunnables, int maxCycle, CommunicationInformation* runnableCommunications);
};

class TaskImplicit : public Communication {
public:
    void GetCommunicationTable(RunnableInformation* runnableInformations, ExecutionInformation* runnableExecutions, int numberOfRunnables, int maxCycle, CommunicationInformation* runnableCommunications);
};

class LET : public Communication {
public:
    void GetCommunicationTable(RunnableInformation* runnableInformations, ExecutionInformation* runnableExecutions, int numberOfRunnables, int maxCycle, CommunicationInformation* runnableCommunications);
};

#endif