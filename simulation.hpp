#ifndef __SIMULATION_HPP__

#define __SIMULATION_HPP__

#include "DAG.hpp"
#include "communication.hpp"
#include "simulation_types.hpp"
#include <algorithm>
#include <vector>
#include <memory>
#include <cmath>
#include <limits.h>
#include <cstring>


class Simulation {
private:
    DAG * dag;

    int maxCycle;
    double hyperPeriod;

    int numberOfTasks;
    int numberOfRunnables;
    int numberOfInputRunnables;
    int numberOfOutputRunnables;

    struct RunnableInformation* runnableInformations;
    struct ExecutionInformation* runnableExecutions;
    struct CommunicationInformation* runnableCommunications;
    struct ExecutionInformation* processExecutions;

    // Command Method Pattern
    friend class Communication;
    Communication * communication;

    void Initialize();
    void ClearTables();

    void SetArrivalTable(double* readTable, double* writeTable, int inputRunnableIndex, int inputCycle, int hyperPeriodCount, int thisRunnableId, int thisCycle, int maxCycle, double* arrivalTable);

public:
    Simulation(DAG *newDag) { dag = newDag;}
    ~Simulation() { 
        delete dag;

        delete[] runnableInformations;
        delete[] runnableExecutions;
        delete[] runnableCommunications;
        delete[] processExecutions;

        delete communication;
    }

    void Simulate();

    void SetRunnableInformations();
    void SetRunnableExecutions();
    void SetRunnableCommunications() {
        communication->GetCommunicationTable(this->runnableInformations, this->runnableExecutions, this->numberOfRunnables, this->maxCycle, this->runnableCommunications);
    }
    void SetProcessExecutions();
    void TraceProcess(int inputRunnableIndex, int inputCycle, int thisRunnableId, int thisCycle, int hyperPeriodCount);
    void GetReactionTime();
    void GetDataAge();

    void SetCommunication(Communication *newCommunication) { communication = newCommunication; }

    void SaveData();
};

#endif