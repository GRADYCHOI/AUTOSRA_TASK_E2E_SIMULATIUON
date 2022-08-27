#ifndef __DAG_HPP__

#define __DAG_HPP__

#include <algorithm>
#include <vector>
#include <memory>
#include <cmath>
#include <limits.h>
#include <cstring>
#include <time.h>
#include <stdlib.h>
#include "TASK.hpp"
#include "mapping.hpp"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"


class DAG
{ 
private:
    // Command Pattern
    friend class Mapping;
    std::unique_ptr<Mapping> mapping;

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
    DAG() {}
    ~DAG();

    std::shared_ptr<TASK> GetTask(const int index) const { return tasks[index]; }
    std::shared_ptr<RUNNABLE> GetRunnable(const int index) const { return runnables[index]; }
    std::vector<std::shared_ptr<TASK>> GetTasks() const { return tasks; }
    std::vector<std::shared_ptr<RUNNABLE>> GetRunnables() const { return runnables; }

    std::vector<int> GetRunnablePriority();
    std::vector<int> GetTaskPriority();

    std::vector<std::shared_ptr<TASK>> GetOrderOfPriorityTasks();
    std::vector<std::shared_ptr<RUNNABLE>> GetOrderOfPriorityRunnables();

    std::vector<std::shared_ptr<RUNNABLE>> GetInputRunnables() const { return inputRunnables; }
    std::vector<std::shared_ptr<RUNNABLE>> GetOutputRunnables() const { return outputRunnables; }

    int GetNumberOfInputRunnables() { return static_cast<int>(inputRunnables.size()); }
    int GetNumberOfOutputRunnables() { return static_cast<int>(outputRunnables.size()); }

    int GetNumberOfTasks() { return static_cast<int>(tasks.size()); }
    int GetNumberOfRunnables() { return static_cast<int>(runnables.size()); }

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
    void SetMapping(std::unique_ptr<Mapping>&& newMapping) { mapping = std::move(newMapping); }
    void DoMapping() { mapping->DoMapping(tasks, runnables); }

    bool CheckMappable();
    void ClearTaskMapping();
    double GetUtilization();

    // Set Precedence
    int CheckPrecedence(std::shared_ptr<RUNNABLE> runnable, int precedence);
    void SetRunnablePrecedence();

    // Get Priority
    void SetTaskPriority();
    void SetRunnablePriority(int index);
    void SetRunnablePriorities();
    void ExpandRunnablePriorities(std::vector<std::vector<int>>& incompleteRunnablePriority, int iterator, int maxSize);

    // Sequence Case
    int GetNumberOfSequenceCase() { return static_cast<int>(runnablePriorities.size()); }

    // Debugging
    void DisplayRunnables();

    // Save to .json
    void SaveJson();
    void ParseDag();

    void GenerateDag();
};

#endif



// g++ main.cpp DAG.cpp TASK.cpp RUNNABLE.cpp -o main -Wall