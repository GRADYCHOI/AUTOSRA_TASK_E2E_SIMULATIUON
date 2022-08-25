#ifndef __DAG_HPP__

#define __DAG_HPP__

#include "TASK.hpp"
#include <algorithm>
#include <vector>
#include <memory>
#include <cmath>
#include <limits.h>
#include <cstring>


class DAG
{ 
private: 
    // Sorted by ID
    std::vector<std::shared_ptr<TASK>> tasks;
    std::vector<std::shared_ptr<RUNNABLE>> runnables;

    // Sorted by ID
    std::vector<std::shared_ptr<RUNNABLE>> inputRunnables;
    std::vector<std::shared_ptr<RUNNABLE>> outputRunnables;

    // Sorted by Priority
    std::vector<int> taskPriority;
    std::vector<int> runnablePriority;
    std::vector<std::vector<int>> runnablePriorities;

    // Sorted by ID (Because of duplicate numbers)
    std::vector<int> runnablePrecedence;

public:
    DAG();
    ~DAG();

    std::shared_ptr<TASK> GetTask(const int index) const { return tasks[index]; }
    std::shared_ptr<RUNNABLE> GetRunnable(const int index) const { return runables[index]; }
    std::vector<std::shared_ptr<TASK>> GetTasks() const { return tasks; }
    std::vector<std::shared_ptr<RUNNABLE>> GetRunnables() const { return runables; }

    std::vector<std::shared_ptr<TASK>> GetOrderOfPriorityTasks();
    std::vector<std::shared_ptr<RUNNABLE>> GetOrderOfPriorityRunnables();

    std::vector<std::shared_ptr<RUNNABLE>> GetInputRunnables() const { return inputRunnables; }
    std::vector<std::shared_ptr<RUNNABLE>> GetOutputRunnables() const { return outputRunnables; }

    int GetNumberOfInputRunnables() { static_cast<int>inputRunnables.size(); }
    int GetNumberOfOutputRunnables() { static_cast<int>outputRunnables.size(); }

    int GetNumberOfTasks() { static_cast<int>tasks.size(); }
    int GetNumberOfRunnables() { static_cast<int>runnables.size(); }

    int GetMaxCycle();
    double GetHyperPeriod();

    // Save Input/Output Runnable list
    void SetInputRunnableList();
    void SetOutputRunnableList();

    // Generate Runnables
    void GenerateRunnables(int numumberOfRunnables);
    void RandomEdge(); 

    // Generate Tasks
    void GenerateTasks(int numberOfTasks);

    // Mapping
    void DoRandomTaskMapping();
    bool CheckMappable();
    void ClearTaskMapping();
    double GetUtilization();

    // Set Precedence
    void CheckPrecedence(std::shared_ptr<RUNNABLE> runnable, int precedence);
    void SetRunnablePrecedence();

    // Get Priority
    void SetTaskPriority();
    void SetRunnablePriority(int index);
    void SetRunnablePriorities();
    void ExpandRunnablePriorities(std::vector<std::vector<int>> incompleteRunnablePriority, int iterator, int maxSize);
    void SetRunnablePrecedence();
    void CheckPrecedence(std::shared_ptr<RUNNABLE> runnable, int precedence);

    // Sequence Case
    int GetNumberOfSequenceCase() { return static_cast<int>runnablePriorities.size(); }

    // Debugging
    void DisplayRunnables();
};

#endif



// g++ main.cpp DAG.cpp TASK.cpp RUNNABLE.cpp -o main -Wall