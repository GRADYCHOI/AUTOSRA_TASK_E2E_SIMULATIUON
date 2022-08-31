#ifndef __DAG_HPP__

#define __DAG_HPP__

#include <algorithm>
#include <vector>
#include <memory>
#include <cmath>
#include <limits>
#include <cstring>
#include <time.h>
#include <stdlib.h>
#include <floats.h>
#include <cstdio>
#include <map>
#include <numeric>
#include <fstream>
#include "TASK.hpp"
#include "mapping.hpp"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"


#define UTILIZATION 0.67

class DAG
{ 
private:
    // Command Pattern
    friend class Mapping;
    std::unique_ptr<Mapping> mapping_;

    // Sorted by ID
    std::vector<std::shared_ptr<TASK>> tasks_;
    std::vector<std::shared_ptr<RUNNABLE>> runnables_;

    // Sorted by ID
    std::vector<std::shared_ptr<RUNNABLE>> inputRunnables_;
    std::vector<std::shared_ptr<RUNNABLE>> outputRunnables_;
	
	// Sorted by ID (Because of duplicate numbers)
    std::vector<int> runnablePrecedence_;
	
	// Sorted by Priority
    std::vector<std::vector<int>> taskToRunnableSequences_;

	int InitializeMaxCycle();
	double InitializeHyperPeriod();
	
	// Save Input/Output Runnable list
    void SetInputRunnableList();
    void SetOutputRunnableList();
	
	bool CheckMappable();
    void ClearTaskMapping();
    double GetUtilization();


public:
    DAG() {}
    ~DAG() { tasks.clear(); runnables.clear(); }

	// Get Tasks & Runnables
    std::vector<std::shared_ptr<TASK>> GetTasks() const { return tasks_; }
    std::vector<std::shared_ptr<RUNNABLE>> GetRunnables() const { return runnables_; }
	
	std::shared_ptr<TASK> GetTask(const int index) const { return tasks_[index]; }
    std::shared_ptr<RUNNABLE> GetRunnable(const int index) const { return runnables_[index]; }
	
	int GetNumberOfTasks() { return static_cast<int>(tasks_.size()); }
    int GetNumberOfRunnables() { return static_cast<int>(runnables_.size()); }

	// Get Input & Output Runnables
    std::vector<std::shared_ptr<RUNNABLE>> GetInputRunnables() const { return inputRunnables_; }
    std::vector<std::shared_ptr<RUNNABLE>> GetOutputRunnables() const { return outputRunnables_; }
	
	int GetNumberOfInputRunnables() { return static_cast<int>(inputRunnables_.size()); }
    int GetNumberOfOutputRunnables() { return static_cast<int>(outputRunnables_.size()); }
	
	// Get Parameter (UnSafe Method)
	int GetMaxCycle() { return (maxCycle_ != -1) ? maxCycle_ : InitializeMaxCycle(); }
    double GetHyperPeriod(); { return (hyperPeriod_ != -1.0) hyperPeriod_ : InitializeeHyperPeriod(); }

    // Generate Runnables
    void GenerateRunnables(int numumberOfRunnables);
    void RandomEdge();

    // Generate Tasks
    void GenerateTasks(int numberOfTasks);

    // Mapping
    void SetMapping(std::unique_ptr<Mapping>&& newMapping) { mapping = std::move(newMapping); }
    void DoMapping() { mapping->DoMapping(tasks, runnables); }

    // Set Precedence
    int CheckPrecedence(std::shared_ptr<RUNNABLE> runnable, int precedence);
    void SetRunnablePrecedence();

    // Get Priority
    void SetTaskPriority();
    void SetRunnablePriority(int index);
    void SetRunnablePriorities();
    void ExpandRunnablePriorities(std::vector<std::vector<int>>& incompleteRunnablePriority, int iterator, int maxSize);

    std::vector<int> GetRunnablePriority();
    std::vector<int> GetTaskPriority();

    std::vector<int> GetRunnableSequence(int index);

    // Sequence Case
    const int GetCurrentSequenceIndex() const { return currentSequenceIndex; }
    int GetNumberOfSequenceCase() { return static_cast<int>(runnablePriorities.size()); }

    // Debugging
    void DisplayRunnables();

    // Save to .json
    void ParseDag(std::string jsonPath);
    void SaveDag(std::string thisTime);

    void GenerateDag();
};

#endif



// g++ main.cpp DAG.cpp TASK.cpp RUNNABLE.cpp -o main -Wall