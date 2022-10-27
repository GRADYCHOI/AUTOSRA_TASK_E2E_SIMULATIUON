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
#include "sequence.hpp"
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
    friend class Sequence;
    std::unique_ptr<Sequence> sequence_;

    // Command Method Pattern
    friend class Communication;
    std::unique_ptr<Communication> communication_;

    std::vector<bool> visitedPermutationNumber_;
    std::vector<int> visitedWorstCycle_;

    // processExecutions
    std::map<int, std::map<int, std::vector<RequiredTime>>> processExecutions_;

	// Store results of all cases
    std::vector<ResultInformation> results_;

    // Showing information
    int maxCycle_;
    long long int hyperPeriod_;
    int numberOfTasks_;
    int numberOfRunnables_;
    int numberOfInputRunnables_;
    int numberOfOutputRunnables_;
    double utilization_;
    double utilizationBound_;

    // directory of files
    std::string dataDirectory_;

    void Initialize();

    void InitializeSequenceCommand();
    void SetSequenceCommand(std::unique_ptr<Sequence>& sequence) { sequence_ = std::move(sequence); }

    void SetSequence(int caseIndex) { sequence_->SetSequence(caseIndex); }
    int GetNumberOfCase() { return sequence_->GetNumberOfCase(); }
    int GetNumberOfRemainedCase() { return sequence_->GetNumberOfRemainedCase(); }
    int GetRandomEmptyIndex() { return sequence_->GetRandomEmptyIndex(); }

	// Strategy Pattern
	void SetRunnableCommunicationTimes() { communication_->SetTimeTable(); }

    void SetProcessExecutions();
    void TraceTime(auto& inputRunnableIter, int inputCycle, const std::shared_ptr<RUNNABLE>& thisRunnable, int thisCycle);

    void InitializeProcessExecutions();

    void CreateProcessExecutions();
    void TraceProcessExecutions(auto& inputRunnablePair, const std::shared_ptr<RUNNABLE>& thisRunnable, std::vector<bool>& visitiedRunnable);

    void CreateVisitedWorstCycle();
    void InitializeVisitedWorstCycle();
	
	ResultInformation GetResult();
    long long int GetWorstReactionTime();
    long long int GetWorstDataAge();
    void SaveDataToCSV(ResultInformation& result);  // one save in one iteration
    void SaveAllDataToCSV();  // save after whole iteration

    void GetDetailResult(std::map<int, std::vector<ResultInformation>>& results);
    //void GetReactionTimeList(std::map<int, std::map<int, std::vector<ResultInformation>>>& results);
    //void GetDataAgeList(std::map<int, std::map<int, std::vector<ResultInformation>>>& results);
    //void InitializeResultsMap(std::map<int, std::map<int, std::vector<ResultInformation>>>& results, int seedNumber);
    void SaveDetailToJson();

    //ResultInformation ParseData(int seedNumber);

    void DisplayDag();
    void DisplayResult(ResultInformation& result, double processTime, int limitProcessTime);

    void SetDataDirectory();
    void MakeDataDirectory();

public:
    Simulation(std::shared_ptr<DAG> newDag) : dag_(newDag) { Initialize(); }
    ~Simulation() { std::clog << "Delete Simulation Class" << std::endl; }

    void Simulate();

    // Strategy Pattern
	void SetCommunicationCommand(std::unique_ptr<Communication>&& newCommunication) { communication_ = std::move(newCommunication); }

    /*
    rapidjson::Value SaveProcessTime(rapidjson::Document::AllocatorType& allocator);
    rapidjson::Value SaveReactionTime(rapidjson::Document::AllocatorType& allocator);
    rapidjson::Value SaveDataAge(rapidjson::Document::AllocatorType& allocator);
    */
};

#endif