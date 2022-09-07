#ifndef __COMMUNICATION__HPP__

#define __COMMUNICATION__HPP__

#include "DAG.hpp"
#include "simulation_types.hpp"


class Simulation;

class Communication {
public:
    Communication() {}
    virtual ~Communication() {}

    virtual void GetCommunicationTable(std::shared_ptr<DAG>& dag, int numberOfRunnables, float hyperPeriod, std::vector<std::vector<std::vector<int>>>& runnablePermutation, std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableCommunications) = 0;
};

class RunnableImplicit : public Communication {
public:
    void GetCommunicationTable(std::shared_ptr<DAG>& dag, int numberOfRunnables, float hyperPeriod, std::vector<std::vector<std::vector<int>>>& runnablePermutation, std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableCommunications);
};

class TaskImplicit : public Communication {
public:
    void GetCommunicationTable(std::shared_ptr<DAG>& dag, int numberOfRunnables, float hyperPeriod, std::vector<std::vector<std::vector<int>>>& runnablePermutation, std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableCommunications);
};

class LET : public Communication {
public:
    void GetCommunicationTable(std::shared_ptr<DAG>& dag, int numberOfRunnables, float hyperPeriod, std::vector<std::vector<std::vector<int>>>& runnablePermutation, std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableCommunications);
};

#endif