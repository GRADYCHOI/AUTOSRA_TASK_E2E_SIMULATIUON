#ifndef __DAG_HPP__

#define __DAG_HPP__

#include "TASK.hpp"
#include <algorithm>
#include <vector>
#include <memory>
#include <cmath>
#include <limits.h>


class DAG
{ 
private:
    double hyperPeriod = -1.0;
    
    std::vector<std::shared_ptr<TASK>> tasks;  // Tasks로 바꿔야 함
    std::vector<std::shared_ptr<RUNNABLE>> runnables;

    std::vector<int> inputRunnables;
    std::vector<int> outputRunnables;

    std::vector<int> taskPriority;
    std::vector<int> runablePriority;
 
    void SetHyperPeriod();

public:
    DAG();
    ~DAG();
    void GenerateRunnables(int num, int in, int out);
    void RandomEdge();
    void AddRunnablePtr(const std::shared_ptr<RUNNABLE>& runnable);
    void DisplayRunnablesPtr();
    int GetNumberOfTasks();
    int GetNumberOfRunnables();
    double GetHyperPeriod();
    double GetReactionTime();
    double GetDataAge();
    void AddRunnables();
    void AddTask();

    //void AddTask(Task task);

    void Simulate();
};

#endif