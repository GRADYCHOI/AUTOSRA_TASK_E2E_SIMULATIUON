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
	
	// initializer
	int InitializeMaxCycle();
	double InitializeHyperPeriod();
	
	// Save Input/Output Runnable list
    void SetInputRunnableList();
    void SetOutputRunnableList();
	
	// Generate Runnables
    void GenerateRunnables(int numumberOfRunnables);
    void RandomEdge();

    // Generate Tasks
    void GenerateTasks(int numberOfTasks);
	
	// Utilization
	bool CheckMappable();
    void ClearTaskMapping();
    double GetUtilization();
	
	// Set Precedence
    int CheckPrecedence(std::vector<int>& precedenceOfRunnables, std::shared_ptr<RUNNABLE> runnable, int precedence);


public:
    DAG() {}
    ~DAG() { tasks.clear(); runnables.clear(); }

	// Get Tasks & Runnables
    const std::vector<std::shared_ptr<TASK>>& GetTasks() const { return tasks_; }
    const std::vector<std::shared_ptr<RUNNABLE>>& GetRunnables() const { return runnables_; }
	
	const std::shared_ptr<TASK> GetTask(const int index) const { return tasks_[index]; }
    const std::shared_ptr<RUNNABLE> GetRunnable(const int index) const { return runnables_[index]; }
	
	const int GetNumberOfTasks() { return static_cast<int>(tasks_.size()); }
    const int GetNumberOfRunnables() { return static_cast<int>(runnables_.size()); }

	// Get Input & Output Runnables
    const std::vector<std::shared_ptr<RUNNABLE>>& GetInputRunnables() const { return inputRunnables_; }
    const std::vector<std::shared_ptr<RUNNABLE>>& GetOutputRunnables() const { return outputRunnables_; }
	
	const int GetNumberOfInputRunnables() { return static_cast<int>(inputRunnables_.size()); }
    const int GetNumberOfOutputRunnables() { return static_cast<int>(outputRunnables_.size()); }
	
	// Priority
    const std::vector<std::shared_ptr<TASK>>& GetTaskPriority() const { return taskOfPriority_; }
	const std::vector<std::vector<std::shared_ptr<RUNNABLE>>>& GetRunnableOfPrecedence() const { return runnablesOfPrecedence_; }
	
	// Get Parameter (UnSafe Method)
	const int GetMaxCycle() { return (maxCycle_ != -1) ? maxCycle_ : InitializeMaxCycle(); }
    const double GetHyperPeriod(); { return (hyperPeriod_ != -1.0) hyperPeriod_ : InitializeeHyperPeriod(); }

	// Generate Command
	void GenerateDag();

    // Mapping
    void SetMapping(std::unique_ptr<Mapping>&& newMapping) { mapping = std::move(newMapping); }
    void DoMapping() { mapping->DoMapping(tasks, runnables); }

    // Set Precedence
    void SetRunnablePrecedence();

    // Save to .json
    void ParseDag(std::string jsonPath);
    void SaveDag(std::string thisTime);
};

#endif



// g++ main.cpp DAG.cpp TASK.cpp RUNNABLE.cpp -o main -Wall