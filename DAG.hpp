#ifndef __DAG_HPP__

#define __DAG_HPP__

#include "TASK.hpp"
#include <algorithm>
#include <vector>
#include <memory>


class DAG
{ 
private:
    int numOfTasks = -1;
    int numOfRunnables = -1;
    double hyperPeriod = -1.0;

    double reactionTime = -1.0;
    double dataAge = -1.0;
    
    std::vector<std::shared_ptr<TASK>> task;  // Tasks로 바꿔야 함
    std::vector<std::shared_ptr<RUNNABLE>> runnables;
 
    void SetParameters();
    void SetNumberOfTasks();
    void SetNumberOfRunnables();
    void SetHyperPeriod();

    // simulation
    void setExecutionTable();

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