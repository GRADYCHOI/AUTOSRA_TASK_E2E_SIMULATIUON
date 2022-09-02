#ifndef __COMMUNICATION__HPP__

#define __COMMUNICATION__HPP__

#include "DAG.hpp"
#include "simulation_types.hpp"


class Communication {
public:
    Communication() {}
    virtual ~Communication() {}

    virtual void GetCommunicationTable(std::vector<RunnableInformation>& runnableInformations, int numberOfRunnables, int maxCycle, std::vector<std::vector<ExecutionInformation>>& runnableCommunications) = 0;
};

class RunnableImplicit : public Communication {
public:
    void GetCommunicationTable(std::vector<RunnableInformation>& runnableInformations, int numberOfRunnables, int maxCycle, std::vector<std::vector<ExecutionInformation>>& runnableCommunications);
};

class TaskImplicit : public Communication {
public:
    void GetCommunicationTable(std::vector<RunnableInformation>& runnableInformations, int numberOfRunnables, int maxCycle, std::vector<std::vector<ExecutionInformation>>& runnableCommunications);
};

class LET : public Communication {
public:
    void GetCommunicationTable(std::vector<RunnableInformation>& runnableInformations, int numberOfRunnables, int maxCycle, std::vector<std::vector<ExecutionInformation>>& runnableCommunications);
};

#endif