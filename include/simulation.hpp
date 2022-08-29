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
    std::unique_ptr<Communication> communication;

    int maxCycle;
    double hyperPeriod;

    int numberOfTasks;
    int numberOfRunnables;
    int numberOfInputRunnables;
    int numberOfOutputRunnables;

    std::string simulationTime;

    std::vector<RunnableInformation> runnableInformations;
    std::vector<std::vector<ExecutionInformation>> runnableExecutions;
    std::vector<std::vector<ExecutionInformation>> runnableCommunications;
    std::map<std::pair<int, int>, std::vector<ExecutionInformation>> processExecutions;

    std::vector<ResultInformation> results;

    void Initialize();
    void ClearTables();

    void SaveReactionTime();
    void SaveDataAge();

public:
    Simulation(std::unique_ptr<DAG>&& newDag) { dag = std::move(newDag); Initialize(); }
    ~Simulation() = default;

    void Simulate();

    void SetRunnableInformations();
    void SetRunnableExecutions();
    void SetRunnableCommunications() {
        communication->GetCommunicationTable(this->runnableInformations, this->runnableExecutions, this->numberOfRunnables, this->maxCycle, this->runnableCommunications);
    }
    void SetProcessExecutions();
    void TraceProcess(int inputRunnableIndex, int inputCycle, int thisRunnableId, int thisCycle, int hyperPeriodCount, std::map<int, double>& path);
    void SetResult();

    double GetReactionTime();
    double GetDataAge();

    void SetCommunication(std::unique_ptr<Communication>&& newCommunication) { communication = std::move(newCommunication); }

    std::vector<ResultInformation> GetBestReactionTime(int numberOfCase);
    std::vector<ResultInformation> GetWorstReactionTime(int numberOfCase);

    std::vector<ResultInformation> GetBestDataAge(int numberOfCase);
    std::vector<ResultInformation> GetWorstDataAge(int numberOfCase);

    void SaveDAG();
    void SaveData();

    rapidjson::Value SaveReactionTime();
    rapidjson::Value SaveDataAge();
};

#endif