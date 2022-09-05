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
    std::unique_ptr<DAG> dag;

    // Command Method Pattern
    friend class Communication;
    std::unique_ptr<Communication> communication_;

    int maxCycle_;
    double hyperPeriod_;

    int numberOfTasks_;
    int numberOfRunnables_;
    int numberOfInputRunnables_;
    int numberOfOutputRunnables_;

    // std::map<std::pair<int, int>, std::vector<ExecutionInformation>> processExecutions;

	// Store results of all cases
    std::vector<ResultInformation> results_;
	
	// part of file name
	std::string simulationTime_;

    void Initialize();
    void ClearTables();
	
	std::vector<RunnableInformation> GetRunnableInformations();
	
	// Command Pattern
	void GetRunnableCommunications(std::vector<std::vector<std::vector<double>>>& runnableCommunications) {
        communication_->GetCommunicationTable(this, runnableCommunications);
    }
	
    double GetReactionTime(std::vector<std::vector<std::vector<double>>>& runnableCommunications);
    double GetDataAge(std::vector<std::vector<std::vector<double>>>& runnableCommunications);

	void SetResult(std::vector<std::vector<std::vector<double>>>& runnableCommunications);

public:
    Simulation(std::unique_ptr<DAG>&& newDag) { dag = std::move(newDag); Initialize(); }
    ~Simulation() {}

    void Simulate();

	// Command Pattern
	void SetCommunication(std::unique_ptr<Communication>&& newCommunication) { communication = std::move(newCommunication); }
	
    void SetProcessExecutions();
    void TraceProcess(int inputRunnableIndex, int inputCycle, int thisRunnableId, int thisCycle, int hyperPeriodCount, std::map<int, double>& path);
    
    std::vector<ResultInformation> GetBestReactionTime(int numberOfCase);
    std::vector<ResultInformation> GetWorstReactionTime(int numberOfCase);

    std::vector<ResultInformation> GetBestDataAge(int numberOfCase);
    std::vector<ResultInformation> GetWorstDataAge(int numberOfCase);

    void SaveDag();
    void SaveData();

    rapidjson::Value SaveReactionTime(rapidjson::Document::AllocatorType& allocator);
    rapidjson::Value SaveDataAge(rapidjson::Document::AllocatorType& allocator);
};

#endif