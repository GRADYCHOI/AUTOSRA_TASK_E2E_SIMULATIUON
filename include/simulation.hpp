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

const int RunnableImplicitMethod = 100;
const int TaskImplicitMethod = 101;
const int LETMethod = 102;

class Simulation {
private:
    std::shared_ptr<DAG> dag_;

    // Command Method Pattern
    friend class Communication;
    std::unique_ptr<Communication> communication_;

    std::vector<bool> visitiedPermutationNumber_;

    // std::map<std::pair<int, int>, std::vector<RequiredTime>> processExecutions;

	// Store results of all cases
    std::vector<std::vector<ResultInformation>> results_;

    // Estimate time
    std::vector<std::clock_t> starts_;
    std::vector<std::clock_t> ends_;

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

    void SetSequence(int numberOfCase);

    const int GetNumberOfPermutation(int number);
	
	// Strategy Pattern
	void SetCommunication(std::unique_ptr<Communication>&& newCommunication) { communication_ = std::move(newCommunication); }

	// Strategy Pattern
	void GetRunnableCommunications() { communication_->GetTimeTable(dag_, runnableTimeTable); }
	
    int GetReactionTime(std::pair<const std::pair<int, int>, std::vector<RequiredTime>>& processExecution);
    int GetDataAge(std::vector<int>& executionPermutationPointer,
                   std::vector<std::vector<std::vector<RequiredTime>>>& runnableExecutions,
                   std::pair<const std::pair<int, int>, std::vector<RequiredTime>>& processExecution);

    std::vector<std::vector<ResultInformation>>& GetBestReactionTime();
    std::vector<std::vector<ResultInformation>>& GetBestDataAge();

    const int GetMaxReactionTime(std::vector<ResultInformation>& a);
    const int GetMaxDataAge(std::vector<ResultInformation>& a);

	std::vector<ResultInformation> GetResult(int caseIndex,
                                std::vector<std::vector<std::vector<int>>>& runnableExecutionPermutation,
                                std::vector<std::vector<std::vector<RequiredTime>>>& runnableExecutions,
                                std::vector<std::vector<std::vector<int>>>& runnableCommunicationPermutation,
                                std::vector<std::vector<std::vector<RequiredTime>>>& runnableCommunications);

public:
    Simulation(std::shared_ptr<DAG> newDag) { dag_ = newDag; Initialize(); }
    ~Simulation() {}

    void Simulate(int communicationMethod);
	
    void SetProcessExecutions(std::vector<int>& executionPermutationPointer,
                              std::vector<std::vector<std::vector<RequiredTime>>>& runnableExecutions,
                              std::vector<int>& communicationPermutationPointer,
                              std::vector<std::vector<std::vector<RequiredTime>>>& runnableCommunications,
                              std::map<std::pair<int, int>, std::vector<RequiredTime>>& processExecutions);
    void TraceProcess(std::vector<int>& executionPermutationPointer,
                      std::vector<std::vector<std::vector<RequiredTime>>>& runnableExecutions,
                      std::vector<int>& communicationPermutationPointer,
                      std::vector<std::vector<std::vector<RequiredTime>>>& runnableCommunications,
                      int inputRunnableId,
                      int inputCycle,
                      int thisRunnableId,
                      int thisCycle,
                      int thisHyperPeriodCount,
                      std::vector<int>& worstCyclePerRunnable,
                      std::map<std::pair<int, int>, std::vector<RequiredTime>>& processExecutions);

    void SaveDag();
    void SaveMapping();
    void SaveData();
    void SaveDataToCSV();

    rapidjson::Value SaveProcessTime(rapidjson::Document::AllocatorType& allocator);
    rapidjson::Value SaveReactionTime(rapidjson::Document::AllocatorType& allocator);
    rapidjson::Value SaveDataAge(rapidjson::Document::AllocatorType& allocator);
};

#endif