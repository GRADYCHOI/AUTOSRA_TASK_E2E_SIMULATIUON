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
    
    // Sorted by ID
    std::vector<std::shared_ptr<TASK>> tasks;  // Tasks로 바꿔야 함
    std::vector<std::shared_ptr<RUNNABLE>> runnables;

    // Sorted by ID
    std::vector<int> inputRunnables;
    std::vector<int> outputRunnables;

    // Sorted by Priority
    std::vector<int> taskPriority;
    std::vector<int> runnablePriority;
    std::vector<std::vector<int>> runnablePriorities;

    // Sorted by ID (Because of duplicate numbers)
    std::vector<int> runnablePrecedence;
 
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
    double GetHyperPeriod() const;
    double GetReactionTime();
    double GetDataAge();
    void AddRunnables();
    void AddTask();

    //void AddTask(Task task);

    void Simulate();
};

#endif



// g++ main.cpp DAG.cpp TASK.cpp RUNNABLE.cpp -o main -Wall