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
    
    std::vector<std::shared_ptr<TASK>> tasks;
    std::vector<std::shared_ptr<RUNNABLE>> runnables;
    std::vector<int> runnablePrecedence;
 
    void SetParameters();
    void SetNumberOfTasks();
    void SetNumberOfRunnables();
    void SetHyperPeriod();

public:
    DAG();
    ~DAG();
    void GenerateRunnables(int num);
    void RandomEdge();
    void DisplayRunnablesPtr(); // -> 러너블 edge도 출력되게 수정
    void SetRunnablePrecedence();
    int GetNumberOfTasks();
    int GetNumberOfRunnables();
    double GetHyperPeriod();
    double GetReactionTime();
    double GetDataAge();
    void AddRunnables();
    void AddTask();
    int CheckPrecedence(std::shared_ptr<RUNNABLE> runnable, int precedence);
    void AddTaskPtr(const std::shared_ptr<TASK>& task);
    void GenerateTasks(int numberOfTasks);

    void SetTaskPriority();
    void SetRunnablePriority(int index);

    void SetRunnablePriorities();
    void ExpandRunnablePriorities(std::vector<std::vector<int>> incompleteRunnablePriority, int pointer, int maxSize);

    int GetNumberOfSequenceCase();
    void DoRandomTaskMapping();
    bool CheckMappable();
    void ClearTaskMapping();
    double GetUtilization();
    

    //void AddTask(Task task);

    void Simulate();
};

#endif