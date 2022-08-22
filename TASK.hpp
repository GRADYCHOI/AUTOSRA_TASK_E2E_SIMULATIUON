#ifndef __TASK_HPP__

#define __TASK_HPP__

#include <iostream>
#include "RUNNABLE.hpp"


class TASK : public std::enable_shared_from_this<RUNNABLE>
{
private:
    int id = -1;
    double period = -1;
    double offset = -1;

    std::vector<std::shared_ptr<RUNNABLE>> runnables;

    void SetExecutionTime();

public:
    TASK(int id, double period, double offset);
    ~TASK();

    void SetPriority(int priority);

    int GetId() const;
    double GetPeriod() const;
    double GetOffset() const;
    
    double GetExecutionTime();
    int GetNumberOfRunnables();

    const std::shared_ptr<RUNNABLE> GetRunnable(int index) const;
    const std::vector<std::shared_ptr<RUNNABLE>>& GetRunnables() const;

    void AddRunnable(const std::shared_ptr<RUNNABLE> runnable);
    void ClearMapping();
};

#endif