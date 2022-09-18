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
    std::unique_ptr<Communication> execution_ = std::unique_ptr<Communication>(new RunnableImplicit());
    std::unique_ptr<Communication> communication_;

    int maxCycle_;
    long long int hyperPeriod_;

    int numberOfTasks_;
    int numberOfRunnables_;
    int numberOfInputRunnables_;
    int numberOfOutputRunnables_;

    // std::map<std::pair<int, int>, std::vector<ExecutionInformation>> processExecutions;

    std::vector<std::vector<int>> sequence_;

	// Store results of all cases
    std::vector<std::vector<ResultInformation>> results_;
	
	// part of file name
	std::string simulationTime_;

    void Initialize();

    const int GetNumberOfPermutation(int number);

    void GetRunnableScheduleInformations(int communicationMethod,
                                         std::vector<std::vector<std::vector<int>>>& runnableExecutionPermutation,
                                         std::vector<std::vector<std::vector<int>>>& runnableCommunicationPermutation,
                                         std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableExecutions,
                                         std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableCommunications);

    void GetRunnableExecutions(std::vector<std::vector<std::vector<int>>>& runnablePermutation, std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableExecutions) {
        execution_->GetCommunicationTable(dag_, numberOfRunnables_, hyperPeriod_, runnablePermutation, runnableExecutions);
    }
	
	// Command Pattern
	void GetRunnableCommunications(std::vector<std::vector<std::vector<int>>>& runnablePermutation, std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableCommunications) {
        communication_->GetCommunicationTable(dag_, numberOfRunnables_, hyperPeriod_, runnablePermutation, runnableCommunications);
    }
	
    int GetReactionTime(std::map<std::pair<int, int>, std::vector<ExecutionInformation>>& processExecutions);
    int GetDataAge(std::vector<int> executionPermutationPointer,
                      std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableExecutions,
                      std::map<std::pair<int, int>, std::vector<ExecutionInformation>>& processExecutions);

    void SetSequence(int numberOfCase, std::vector<std::vector<std::vector<int>>>& runnableExecutionPermutation);
    void SetTaskPriorityRunnablePriority(int numberOfCase, std::vector<std::vector<std::vector<int>>>& runnableExecutionPermutation);
    void SetRunnablePriorityRunnablePriority(int numberOfCase, std::vector<std::vector<std::vector<int>>>& runnableExecutionPermutation);
    
    const int GetMaxOutputTaskPriority(std::shared_ptr<RUNNABLE> tmpRunnable);
    const int GetMaxOutputRunnablePrecedence(std::shared_ptr<RUNNABLE> tmpRunnable);

	ResultInformation GetResult(int caseIndex,
                                std::vector<std::vector<std::vector<int>>>& runnableExecutionPermutation,
                                std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableExecutions,
                                std::vector<std::vector<std::vector<int>>>& runnableCommunicationPermutation,
                                std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableCommunications);

public:
    Simulation(std::shared_ptr<DAG> newDag) { dag_ = newDag; Initialize(); }
    ~Simulation() {}

    void Simulate(int communicationMethod);

	// Command Pattern
	void SetCommunication(std::unique_ptr<Communication>&& newCommunication) { communication_ = std::move(newCommunication); }
	
    void SetProcessExecutions(std::vector<int>& executionPermutationPointer,
                              std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableExecutions,
                              std::vector<int>& communicationPermutationPointer,
                              std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableCommunications,
                              std::map<std::pair<int, int>, std::vector<ExecutionInformation>>& processExecutions);
    void TraceProcess(std::vector<int> executionPermutationPointer,
                      std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableExecutions,
                      std::vector<int> communicationPermutationPointer,
                      std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableCommunications,
                      int inputRunnableId,
                      int inputCycle,
                      int thisRunnableId,
                      int thisCycle,
                      int thisHyperPeriodCount,
                      std::vector<int>& worstCyclePerRunnable,
                      std::map<std::pair<int, int>, std::vector<ExecutionInformation>>& processExecutions);
    
    std::vector<ResultInformation>& GetBestReactionTime();
    std::vector<ResultInformation>& GetBestDataAge();

    void SaveDag();
    void SaveMapping();
    void SaveData();

    rapidjson::Value SaveReactionTime(rapidjson::Document::AllocatorType& allocator);
    rapidjson::Value SaveDataAge(rapidjson::Document::AllocatorType& allocator);
};

#endif