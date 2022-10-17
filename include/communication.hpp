#ifndef __COMMUNICATION__HPP__

#define __COMMUNICATION__HPP__

#include "DAG.hpp"
#include "simulation_types.hpp"


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
    ~Communication() { emptyTimes_.clear(); }
    
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