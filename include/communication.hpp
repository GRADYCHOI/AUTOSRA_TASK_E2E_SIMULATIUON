#ifndef __COMMUNICATION__HPP__

#define __COMMUNICATION__HPP__

#include "DAG.hpp"
#include "simulation_types.hpp"


class Communication {
public:
    Communication() {}
    virtual ~Communication() {}

    virtual void GetTimeTable(std::shared_ptr<DAG>& dag, std::vector<std::vector<RequiredTime>>& runnableTimeTable) = 0;
};

class RunnableImplicit : public Communication {
public:
    void GetTimeTable(std::shared_ptr<DAG>& dag, std::vector<std::vector<RequiredTime>>& runnableTimeTable);
};

class TaskImplicit : public Communication {
public:
    void GetTimeTable(std::shared_ptr<DAG>& dag, std::vector<std::vector<RequiredTime>>& runnableTimeTable);
};

class LET : public Communication {
public:
    void GetTimeTable(std::shared_ptr<DAG>& dag, std::vector<std::vector<RequiredTime>>& runnableTimeTable);
};

#endif