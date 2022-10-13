#ifndef __SIMULATION_HPP__

#define __SIMULATION_HPP__

#include <algorithm>
#include <vector>
#include <memory>
#include <cmath>
#include <limits.h>
#include <cstring>
#include <cstdio>
#include <map>
#include <fstream>
#include <numeric>
#include <iomanip>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include "DAG.hpp"
#include "communication.hpp"
#include "simulation_types.hpp"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/ostreamwrapper.h"


class Simulation {
private:
    std::shared_ptr<DAG> dag_;

    // Command Method Pattern
    friend class Communication;
    std::unique_ptr<Communication> communication_;

    std::vector<std::vector<std::vector<int>>> sequenceMatrix_; // [task][precedence][runnable]

    std::vector<bool> visitedPermutationNumber_;
    std::vector<int> visitedWorstCycle_;

    std::map<int, std::map<int, std::vector<RequiredTime>> processExecutions_;

	// Store results of all cases
    std::vector<std::vector<ResultInformation>> results_;

    // Showing information
    int maxCycle_;
    double hyperPeriod_;
    int numberOfTasks_;
    int numberOfRunnables_;
    int numberOfInputRunnables_;
    int numberOfOutputRunnables_;
    double utilization_;

    // directory of save files
    std::string dataDirectory_;

    void Initialize();

    int GetNumberOfCase();

    void SetSequence(int caseIndex);
    void SetSequenceMatrix();
	
	// Strategy Pattern
	void SetCommunication(std::unique_ptr<Communication>&& newCommunication) { communication_ = std::move(newCommunication); }

	// Strategy Pattern
	void SetRunnableCommunicationTimes(std::vector<std::vector<RequiredTime>>& runnableTimeTable) { communication_->SetTimeTable(); }

    void SetProcessExecutions();
    void TraceTime(auto& inputRunnableIter, int inputCycle, std::shared_ptr<RUNNABLE>& thisRunnable, int thisCycle);

    void InitializeProcessExecutions();

    void CreateProcessExecutions();
    void TraceProcessExecutions(auto& inputRunnableIter, std::shared_ptr<RUNNABLE>& thisRunnable, std::vector<bool>& visitiedRunnable);

    void CreateVisitedWorstCycle()
	
    int GetMaxReactionTime();
    int GetMaxDataAge();

	ResultInformation GetResult();

    void SaveDataToCSV();

public:
    Simulation(std::shared_ptr<DAG> newDag) { dag_ = newDag; Initialize(); }
    ~Simulation() {}

    void Simulate();

    /*
    rapidjson::Value SaveProcessTime(rapidjson::Document::AllocatorType& allocator);
    rapidjson::Value SaveReactionTime(rapidjson::Document::AllocatorType& allocator);
    rapidjson::Value SaveDataAge(rapidjson::Document::AllocatorType& allocator);
    */
};

#endif