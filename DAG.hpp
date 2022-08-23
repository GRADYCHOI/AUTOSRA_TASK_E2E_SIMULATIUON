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
    std::vector<std::shared_ptr<TASK>> tasks2;
    std::vector<std::shared_ptr<RUNNABLE>> runnables;
    std::vector<int> runnablePrecedence;
    // Sorted by Priority
    std::vector<int> taskPriority;
    std::vector<int> runnablePriority;
    std::vector<std::vector<int>> runnablePriorities;
 
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
    void SetTasksPriority();

    void DoRandomTaskMapping();
    bool CheckMappable();
    double GetUtilization();
    

    //void AddTask(Task task);

    void Simulate();
};

#endif