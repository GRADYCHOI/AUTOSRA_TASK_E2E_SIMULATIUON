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
    
    std::vector<std::shared_ptr<TASK>> task;
    std::vector<std::shared_ptr<RUNNABLE>> runnables;
 
    void SetParameters();
    void SetNumberOfTasks();
    void SetNumberOfRunnables();
    void SetHyperPeriod();

public:
    DAG();
    ~DAG();
    void GenerateRunnables(int num, int in, int out);
    void RandomEdge(int num, int in);
    void DisplayRunnablesPtr(); // -> 러너블 edge도 출력되게 수정
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