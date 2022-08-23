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

    int GetNumberOfTasks();
    int GetNumberOfRunnables();
    double GetHyperPeriod() const;

    void SetInputRunnableList();
    void SetOutputRunnableList();

    int GetNumberOfInputRunnables();
    int GetNumberOfOutputRunnables();

    void GenerateRunnables(int numumberOfRunnables);
    void GenerateTasks(int numberOfTasks);
    void SetRandomEdge();

    void AddRunnablePtr(const std::shared_ptr<RUNNABLE>& runnable);
    void AddTaskPtr(const std::shared_ptr<TASK>& task);
    void RandomEdge(); 

    void SetRunnablePrecedence();
    void CheckPrecedence(std::shared_ptr<RUNNABLE> runnable, int precedence);
    int GetRunnablePrecedence(int index);
    
    void SetTaskPriority();
    void SetRunnablePriority(int index);
    void SetRunnablePriorities();
    void ExpandRunnablePriorities(std::vector<std::vector<int>> incompleteRunnablePriority, int pointer, int maxSize);

    static bool CompareTaskPeriod(std::pair<int, double> a, std::pair<int, double> b) { return a.second < b.second; }
    static bool CompareRunnablePrecedence(std::pair<int, int> a, std::pair<int, int> b) { return a.second < b.second; }

    int GetNumberOfSequenceCase();

    void DoRandomTaskMapping();
    bool CheckMappable();
    void ClearTaskMapping();
    double GetUtilization();

    void SimulateImplicitRunnable();
    void SimulateTaskImplicitTask();

    int GetMaxCycle();

    void GetTaskPeriods(double* periods);
    void GetTaskOffsets(double* offsets);
    void GetTaskExecutionTimes(double* executions);

    void GetRunnablePeriods(double* periods);
    void GetRunnableOffsets(double* offsets);
    void GetRunnableExecutionTimes(double* executions);

    void GetTaskInfo(double* periods, double* offsets, double* executions);
    void GetRunnableInfo(double* periods, double* offsets, double* executions);

    void GetExecutionTable(double* periods, double* offsets, double* executions, int size, int maxCycle, double* startTable, double* endTable);

    void GetReadTable(double* startTable, int size, int maxCycle, double* readTable);
    void GetReleaseTimeReadTable(double* periods, double* offsets, int size, int maxCycle, double* readTable);

    void GetWriteTable(double* endTable, int size, int maxCycle, double* writeTable);
    void GetReleaseTimeWriteTable(double* periods, double* offsets, int size, int maxCycle, double* writeTable);
    void GetNextJobWriteTable(double* startTable, int size, int maxCycle, double* writeTable);

    void SetArrivalTable(double* readTable, double* writeTable, int inputRunnableIndex, int inputCycle, int hyperPeriodCount, int thisRunnableId, int thisCycle, int maxCycle, double* arrivalTable);
    void GetArrivalTable(double* readTable, double* writeTable, int maxCycle, double* arrivalTable);

    void GetReactionTime(double* arrivalTable, double* readTable, int maxCycle, double* reactionTime);
    void GetDataAge(double* arrivalTable, double* writeTable, int maxCycle, double* dataAge);

    void Simulate();

    void DisplayRunnables();
};

#endif



// g++ main.cpp DAG.cpp TASK.cpp RUNNABLE.cpp -o main -Wall