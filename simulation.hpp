#ifndef __SIMULATION_HPP__

#define __SIMULATION_HPP__

#include "DAG.hpp"
#include "communication.hpp"
#include <algorithm>
#include <vector>
#include <memory>
#include <cmath>
#include <limits.h>
#include <cstring>


class Simulation {
private:
    Communication * communication;
    DAG * dag;

    int dagMaxCycle;

    void SetArrivalTable(double* readTable, double* writeTable, int inputRunnableIndex, int inputCycle, int hyperPeriodCount, int thisRunnableId, int thisCycle, int maxCycle, double* arrivalTable);

public:
    Simulation();
    ~Simulation() { delete dag; delete communication; }

    void SetDag(DAG *newDag) { dag = newDag; }

    void Simulate();

    void SaveData();

    void GetRunnableInformations(double* runnableInformations);

    void GetExecutionTable(double* runnableInformations, int size, int maxCycle, double* startTable, double* endTable);

    void SetCommunication(Communication *newCommunication) { communication = newCommunication; }
    void GetCommunicationTable(double* runnableInformations, double* startTable, double* endTable, int numberOfRunnables, int maxCycle, double* readTable, double* writeTable) {
        communication->GetCommunicationTable(runnableInformations, startTable, endTable, numberOfRunnables, maxCycle, readTable, writeTable);
    }

    void GetArrivalTable(double* readTable, double* writeTable, int maxCycle, double* arrivalTable);

    void GetReactionTime(double* arrivalTable, double* readTable, int maxCycle, double* reactionTime);
    void GetDataAge(double* arrivalTable, double* writeTable, int maxCycle, double* dataAge);
};

#endif