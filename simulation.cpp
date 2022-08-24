#include "simulation.hpp"


void Simulation::Simulate() {
    int numberOfRunnables = this->dag->GetNumberOfRunnables();
    int maxCycle = this->dag->GetMaxCycle();

    int numberOfInputRunnables = this->dag->GetNumberOfInputRunnables();
    int numberOfOutputRunnables = this->dag->GetNumberOfOutputRunnables();

    double* runnableInformations = new double[numberOfRunnables * 5];
    double* runnableStartTable = new double[numberOfRunnables * maxCycle];
    double* runnableEndTable = new double[numberOfRunnables * maxCycle];
    double* runnableReadTable = new double[numberOfRunnables * maxCycle];
    double* runnableWriteTable = new double[numberOfRunnables * maxCycle];
    double* arrivalTable = new double[numberOfInputRunnables * numberOfOutputRunnables * maxCycle];

    memset(runnableInformations, -1.0, sizeof(double) * numberOfRunnables * 5);
    memset(runnableStartTable, -1.0, sizeof(double) * numberOfRunnables * maxCycle);
    memset(runnableEndTable, -1.0, sizeof(double) * numberOfRunnables * maxCycle);
    memset(runnableReadTable, -1.0, sizeof(double) * numberOfRunnables * maxCycle);
    memset(runnableWriteTable, -1.0, sizeof(double) * numberOfRunnables * maxCycle);
    memset(arrivalTable, -1.0, sizeof(double) * numberOfInputRunnables * numberOfOutputRunnables * maxCycle);


}

void Simulation::SaveData() {

}

void Simulation::GetRunnableInformations(double* runnableInformations) {
    // --------------------------------------------------------------------------------------------------------------
    // runnableInformations : [5 X numberOfRunnables]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : (Mapped) Task ID
    // 2 : Priority
    // 3 : Period
    // 4 : Offset
    // 5 : Execution Time
    // --------------------------------------------------------------------------------------------------------------

    for (auto &task : this->dag->GetTasks()) {

    }
}

void Simulation::GetExecutionTable(double* runnableInformations, int size, int maxCycle, double* startTable, double* endTable) {

}

void Simulation::SetArrivalTable(double* readTable, double* writeTable, int inputRunnableIndex, int inputCycle, int hyperPeriodCount, int thisRunnableId, int thisCycle, int maxCycle, double* arrivalTable) {

}

void Simulation::GetArrivalTable(double* readTable, double* writeTable, int maxCycle, double* arrivalTable) {

}

void Simulation::GetReactionTime(double* arrivalTable, double* readTable, int maxCycle, double* reactionTime) {

}

void Simulation::GetDataAge(double* arrivalTable, double* writeTable, int maxCycle, double* dataAge) {

}