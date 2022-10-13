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
#include "precedence.hpp"
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

    friend class Precedence;
    std::unique_ptr<Precedence> precedence_;

    // Entire Members
    std::vector<std::shared_ptr<TASK>> tasks_;
    std::vector<std::shared_ptr<RUNNABLE>> runnables_;

    // Special Members
    std::vector<std::shared_ptr<TASK>> tasksInPriority_;
    std::vector<std::shared_ptr<RUNNABLE>> inputRunnables_;
    std::vector<std::shared_ptr<RUNNABLE>> outputRunnables_;

    // Status
    int maxCycle_ = -1;
    long long int hyperPeriod_ = -1;
    double utilization_ = -1.0;
    double utilizationBound_ = -1.0;
	
	// Save Input/Output Runnable list
    void SetInputRunnableList();
    void SetOutputRunnableList();

	bool CheckMappable();
    void ClearMapping();
	
	// Generate Runnables
    void GenerateRunnables();
    void RandomEdge();

    // Generate Tasks
    void GenerateTasks();

    // Mapping
    void SetMapping(std::unique_ptr<Mapping>&& newMapping) { mapping_ = std::move(newMapping); }
    void DoMapping() { mapping_->DoMapping(tasks_, runnables_); }

    // Set Status
    void SetStatus();
    void SetMaxCycle();
    void SetHyperPeriod();
    void SetUtilization();
    void SetUtilizationBound();

    // Parse from .json
    void ParseDag(std::string jsonPath);
    void ParseMapping(std::string jsonPath);

public:
    // Constructor
    DAG(std::unique_ptr<Mapping>& newMapping) { SetMapping(std::move(newMapping)); GenerateRunnables(); GenerateTasks(); DoMapping(); SetStatus(); }
    DAG(std::unique_ptr<Mapping>& newMapping, const std::string dagJsonPath) { SetMapping(std::move(newMapping)); ParseDag(dagJsonPath); GenerateTasks(); DoMapping(); SetStatus(); }
    DAG(const std::string dagJsonPath, const std::string mappingJsonPath) { ParseDag(dagJsonPath); ParseMapping(mappingJsonPath); SetStatus(); }

    // Destructor
    ~DAG() {}

	// Get Functions
    const std::vector<std::shared_ptr<TASK>>& GetTasks() const { return tasks_; }
	const std::shared_ptr<TASK> GetTask(const int index) const { return tasks_[index]; }
    const int GetNumberOfTasks() { return static_cast<int>(tasks_.size()); }

    const std::vector<std::shared_ptr<RUNNABLE>>& GetRunnables() const { return runnables_; }
    const std::shared_ptr<RUNNABLE> GetRunnable(const int index) const { return runnables_[index]; }
    const int GetNumberOfRunnables() { return static_cast<int>(runnables_.size()); }

    const std::vector<std::shared_ptr<TASK>> GetTasksInPriority() const { return tasksInPriority_; }

    const std::vector<std::shared_ptr<RUNNABLE>>& GetInputRunnables() const { return inputRunnables_; }
    const std::shared_ptr<RUNNABLE> GetInputRunnable(const int index) const { return inputRunnables_[index]; }
    const int GetNumberOfInputRunnables() { return static_cast<int>(inputRunnables_.size()); }

    const std::vector<std::shared_ptr<RUNNABLE>>& GetOutputRunnables() const { return outputRunnables_; }
    const std::shared_ptr<RUNNABLE> GetOutputRunnable(const int index) const { return outputRunnables_[index]; }
    const int GetNumberOfOutputRunnables() { return static_cast<int>(outputRunnables_.size()); }

    int GetMaxCycle() const { return maxCycle_; };
    long long int GetHyperPeriod() const { return hyperPeriod_; }
    const double GetUtilization() const {return utilization_; }
    const double GetUtilizationBound() const {return utilizationBound_; }

    // Save to .json
    void SaveDag(std::string dataDirectory);
    void SaveMapping(std::string dataDirectory);
};

#endif