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
protected:
    std::shared_ptr<DAG> dag_;

    // Command Method Pattern
    friend class Sequence;
    std::unique_ptr<Sequence> sequence_;

    // Command Method Pattern
    friend class Communication;
    std::unique_ptr<Communication> communication_;

    std::vector<bool> visitedPermutationNumber_;
    std::vector<VisitedInformation> visitedWorstCycleList_;
    std::vector<int> visitedWorstCycle_;

    // path
    std::vector<std::vector<bool>> pathes_;

    // processExecutions
    std::map<int, std::map<int, std::vector<RequiredTime>>> processExecutions_;

	// Store results of all cases
    std::map<int, std::map<int, ResultInformation>> results_;

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

    void CreateVisitedWorstCycleList();
    void InitializeVisitedWorstCycleList();
    void SetProcessExecutionsList();
    void TraceTimeList(std::shared_ptr<RUNNABLE> inputRunnable, int inputRunnableCycle);
    void SetExecutionList(auto& inputRunnableIter, int inputRunnableCycle);

    void SetProcessExecutions();
    void TraceTime(auto& inputRunnableIter, int inputCycle, const std::shared_ptr<RUNNABLE>& thisRunnable, int thisCycle);

    void InitializeProcessExecutions();

    void CreateProcessExecutions();
    void TraceProcessExecutions(auto& inputRunnablePair, const std::shared_ptr<RUNNABLE>& thisRunnable, std::vector<bool>& visitiedRunnable);

    void CreateVisitedWorstCycle();
    void InitializeVisitedWorstCycle();

    void SetPathes();
    void TracePath(const std::shared_ptr<RUNNABLE>& runnable, std::vector<bool> tmp_path);

    void SetResult();
	void SetResultMap();
    void InitResultMap();
    void GetWorstReactionTime();
    void GetWorstDataAge();
    void SaveDataToCSV(int seedNumber);  // one save in one iteration
    // void SaveAllDataToCSV();  // save after whole iteration

    void DisplayDag();
    void DisplayResult(int seedNumber, double processTime, int limitProcessTime);

    void SetDataDirectory();
    void MakeDataDirectory();

    std::vector<long long int> numberOfPathList_;
    std::vector<std::vector<bool>> visitRunnable_;
    void SetNumberOfPathList();
    const long long int GetNumberOfPath();
    void SetVisitRunnable();
    void GetReactionTimeList();
    long long int SetReactionTimeNew(std::vector<bool> visitRunnable, int cycle);

    void SetReactionTimeThread(std::shared_ptr<RUNNABLE> inputRunnable, int inputRunnableCycle, std::map<int, std::map<int, std::vector<RequiredTime>>>::iterator inputRunnableIter);

public:
    Simulation(std::shared_ptr<DAG> newDag) : dag_(newDag) { Initialize(); }
    ~Simulation() { std::clog << "Delete Simulation Class" << std::endl; }

    void Simulate();
    void SimulateTest();

    // Strategy Pattern
	void SetCommunicationCommand(std::unique_ptr<Communication>&& newCommunication) { communication_ = std::move(newCommunication); }

    /*
    rapidjson::Value SaveReactionTime(rapidjson::Document::AllocatorType& allocator);
    rapidjson::Value SaveDataAge(rapidjson::Document::AllocatorType& allocator);
    */
};

#endif