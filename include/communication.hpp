#ifndef __COMMUNICATION__HPP__

#define __COMMUNICATION__HPP__

#include <thread>
#include <future>
#include <ctime>
#include "DAG.hpp"
#include "simulation_types.hpp"


#define MAX_THREAD 20

class Communication {
protected:
    std::shared_ptr<DAG> dag_;

    int unit_;
    int numberOfCores_;
    std::vector<int> emptyTimes_;

    void InitializeMembers();
    void InitializeEmptyTimes();
    void InitializeRunnables();

public:
    Communication(std::shared_ptr<DAG> newDag) : dag_(newDag) { InitializeMembers(); InitializeRunnables(); }
    virtual ~Communication() { std::clog << "Delete Communication Class" << std::endl; }
    
    virtual void SetTimeTable() = 0;
};

class RunnableImplicit : public Communication {
public:
    using Communication::Communication;

    void SetTimeTable();
};

class TaskImplicit : public Communication {
public:
    using Communication::Communication;

    void SetTimeTable();
};

class LET : public Communication {
public:
    using Communication::Communication;
    
    void SetTimeTable();
};

#endif