#ifndef __TASK_HPP__

#define __TASK_HPP__

#include <iostream>
#include "RUNNABLE.hpp"


class TASK : public std::enable_shared_from_this<TASK>
{
private:
    int id = -1;
    double period = -1;
    double offset = -1;
    double executionTime = 0;

    int numberOfRunnables = 0;
    int priority = -1;

    std::vector<std::shared_ptr<RUNNABLE>> runnables;

    void SetExecutionTime();
    void SetNumberOfRunnables();

public:
    TASK(int id, double period, double offset);
    ~TASK();

    int GetId();
    void SetPriority(int priority);

    double GetPeriod();
    double GetOffset();
    double GetExecutionTime();
    int GetNumberOfRunnables();
    int GetPriority();
    const std::shared_ptr<RUNNABLE> GetRunnable(int index) const;
    const std::vector<std::shared_ptr<RUNNABLE>> GetRunnables() const;

    void AddRunnable(const std::shared_ptr<RUNNABLE> runnable);
};

#endif