#ifndef __DAG_HPP__

#define __DAG_HPP__

#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <cmath>
#include <limits>
#include <cstring>
#include <time.h>
#include <stdlib.h>
#include <float.h>
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
	float InitializeHyperPeriod();
	
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
	
	// Set Precedence
    void CheckPrecedence(std::vector<int>& precedenceOfRunnables, const std::shared_ptr<RUNNABLE>& runnable, int precedence);


public:
    DAG() {}
    ~DAG() { tasks_.clear(); runnables_.clear(); }

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

    const std::vector<std::shared_ptr<TASK>> GetTasksPriority();
	
	const int GetNumberOfInputRunnables() { return static_cast<int>(inputRunnables_.size()); }
    const int GetNumberOfOutputRunnables() { return static_cast<int>(outputRunnables_.size()); }

    const int GetMaxCycle();
    const long long int GetHyperPeriod();

	// Generate Command
	void GenerateDag();

    // Mapping
    void SetMapping(std::unique_ptr<Mapping>&& newMapping) { mapping_ = std::move(newMapping); }
    void DoMapping() { mapping_->DoMapping(tasks_, runnables_); }

    // Set Priority
    void SetRunnablePrecedence();
    void SetTaskPriority();

    void SetAllCaseRunnablePriority();
    void SetPrecedenceRunnablePriority();

    float GetUtilization();

    // Save to .json
    void ParseDag(std::string jsonPath);
    void SaveDag(std::string thisTime);

    void ParseMapping(std::string jsonPath);
    void SaveMapping(std::string thisTime);
};

#endif



// g++ main.cpp DAG.cpp TASK.cpp RUNNABLE.cpp -o main -Wall