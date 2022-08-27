#ifndef __SIMULATION_HPP__

#define __SIMULATION_HPP__

#include <algorithm>
#include <vector>
#include <memory>
#include <cmath>
#include <limits.h>
#include <cstring>
#include <map>
#include "DAG.hpp"
#include "communication.hpp"
#include "simulation_types.hpp"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"


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

    std::unique_ptr<RunnableInformation[]> runnableInformations;
    std::unique_ptr<ExecutionInformation[]> runnableExecutions;
    std::unique_ptr<CommunicationInformation[]> runnableCommunications;
    std::map<std::pair<int, int>, std::vector<ExecutionInformation>> processExecutions;

    void Initialize();
    void ClearTables();

    void SaveDAG();
    void SaveMapping();
    void SaveReactionTime();
    void SaveDataAge();

public:
    Simulation(std::unique_ptr<DAG>&& newDag) { dag = std::move(newDag);}
    ~Simulation() = default;

    void Simulate();

    void SetRunnableInformations();
    void SetRunnableExecutions();
    void SetRunnableCommunications() {
        communication->GetCommunicationTable(this->runnableInformations.get(), this->runnableExecutions.get(), this->numberOfRunnables, this->maxCycle, this->runnableCommunications.get());
    }
    void SetProcessExecutions();
    void TraceProcess(int inputRunnableIndex, int inputCycle, int thisRunnableId, int thisCycle, int hyperPeriodCount);
    void GetReactionTime();
    void GetDataAge();

    void SetCommunication(std::unique_ptr<Communication>&& newCommunication) { communication = std::move(newCommunication); }

    void SaveData();
};

#endif