#include <iostream>
#include "RUNNABLE.hpp"


class TASK
{
private:
    double period;
    double offset;
    double executionTime;

    int numberOfRunnables;
    int priority;

    std::vector<std::shared_ptr<RUNNABLE>> runnables;

    void SetExecutionTime();
    void SetNumberOfRunnables();

public:
    TASK(int id, double period, double offset);
    ~TASK();

    void SetPriority(int priority);

    double GetPeriod();
    double GetOffset();
    double GetExecutionTime();
    int GetNumberOfRunnables();
    int GetPriority();

    void AddRunnable(const std::shared_ptr<RUNNABLE>& runnable);
};