#include "simulation.hpp"

void Simulation::Initialize() {
    this->maxCycle = this->dag->GetMaxCycle();
    this->hyperPeriod = this->dag->GetHyperPeriod();
    this->numberOfTasks = this->dag->GetNumberOfTasks();
    this->numberOfRunnables = this->dag->GetNumberOfRunnables();
    this->numberOfInputRunnables = this->dag->GetNumberOfInputRunnables();
    this->numberOfOutputRunnables = this->dag->GetNumberOfOutputRunnables();

    this->runnableInformations = new struct RunnableInformation[this->numberOfRunnables];
    this->runnableExecutions = new struct ExecutionInformation[this->numberOfRunnables * maxCycle];
    this->runnableCommunications = new struct CommunicationInformation[this->numberOfRunnables * maxCycle];
    this->processExecutions = new ExecutionInformation[this->numberOfInputRunnables * this->numberOfOutputRunnables * maxCycle];

    this->ClearTables();
}

void Simulation::ClearTables() {
    struct RunnableInformation initialRunnableInformation = {-1, -1, -1.0, -1.0, -1.0};
    struct ExecutionInformation initialExecutionInformation = {-1.0, -1.0};
    struct CommunicationInformation initialCommunicationInformation = {-1.0, =1.0};

    memcpy(runnalbeInformations, &initialRunnableInformation, sizeof(struct RunnableInformation) * this->numberOfRunnables);
    memset(runnableExecutions, &initialExecutionInformation, sizeof(struct ExecutionInformation) * this->numberOfRunnables * maxCycle);
    memset(runnableCommunications, &initialCommunicationInformation, sizeof(struct CommunicationInformation) * this->numberOfRunnables * maxCycle);
    memset(processExecutions, &initialExecutionInformation, sizeof(struct ExecutionInformation) * this->numberOfInputRunnables * this->numberOfOutputRunnables * maxCycle);
}

void Simulation::Simulate() {

}

void Simulation::SaveData() {

}

void Simulation::SetRunnableInformations() {
    // --------------------------------------------------------------------------------------------------------------
    // runnableInformations : [1 X numberOfRunnables]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : RunnableInformation (taskId, priority, period, offset, executionTime)
    // --------------------------------------------------------------------------------------------------------------

    for (auto &task : this->dag->GetTasks()) {
        for (auto &runnable : task->GetRunnables()) {
            int runnableId = runnable->GetId();

            this->runnalbeInformations[runnableId].taskId = task->GetId();
            this->runnalbeInformations[runnableId].priority = this->dag->GetPriority(runnableId);
            this->runnalbeInformations[runnableId].period = task->GetPeriod();
            this->runnalbeInformations[runnableId].offset = task->GetOffset();
            this->runnalbeInformations[runnableId].executionTime = runnable->GetExecutionTime();
        }
    }
}

// Only for special case (periods&offsets are executed in every ms)
void Simulation::SetRunnableExecutions() {
    // --------------------------------------------------------------------------------------------------------------
    // runnableInformations : [1 X numberOfRunnables]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : RunnableInformation (taskId, priority, period, offset, executionTime)
    // --------------------------------------------------------------------------------------------------------------
    // runnableExecutions : [maxCycle X numberOfRunnables]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : First Cycle's ExecutionInformation (startTime, endTime)
    // 2 : Second Cycle's ExecutionInformation (startTime, endTime)
    // ..
    // --------------------------------------------------------------------------------------------------------------

    double* emptyTimes = new double[static_cast<int>this->GetHyperPeriod()];
    memset(emptyTimes, 1.0, sizeof(double) * static_cast<int>this->GetHyperPeriod());

    for (auto &runnable : this->dag->GetRunnables()) {
        int runnableId = runnable->GetId();
        int runnableMaxCycle = static_cast<int>(this->GetHyperPeriod()) / this->runnableInformations[runnableId].period;

        for (int cycle = 0; cycle < runnableMaxCycle; cycle++) {
            double releaseTime = this->runnableInformations[runnableId].period * cycle + this->runnableInformations[runnableId].offset;
            double deadTime = this->runnableInformations[runnableId].period * (cycle + 1) + this->runnableInformations[runnableId].offset;

            int integerReleaseTime = static_cast<int>(std::floor(releaseTime));

            // Regard time-line
            while (emptyTimes[integerReleaseTime] == 0.0) integerReleaseTime++;

            // Set start time
            this->runnableExecutions[runnableId * this->maxCycle + cycle].startTime = static_cast<double>(integerReleaseTime) + (1 - emptyTimes[integerReleaseTime]);
            if (this->runnableExecutions[runnableId].startTime > deadTime) {
                std::cerr << "[Scheduling Error] : This sequence can't scheduling";
                throw runnable;
            }

            // Set end time
            double executionTime = this->runnableInformations[runnableId].executionTime;

            while (executionTime) {
                if (emptyTimes[integerReleaseTime] < executionTime) {
                    execution -= emptyTimes[integerReleaseTime];
                    emptyTimes[integerReleaseTime] = 0.0;

                    integerReleaseTime += 1;
                } else {
                    this->runnableExecutions[runnableId * this->maxCycle + cycle].endTime = static_cast<double>integerReleaseTime + executionTime;
                    emptyTimes[integerReleaseTime] -= executionTime;
                    executionTime = 0.0;
                }
            }
        }
    }

    delete[] emptyTimes;
}

void Simulation::SetProcessExecutions() {
    for (int inputRunnableIndex = 0; inputRunnableIndex < this->numberOfInputRunnables; inputRunnableIndex++) {
        for (int cycle = 0; cycle < this->maxCycle; cycle++) {
            this->SetArrivalTable(inputRunnableIndex, cycle, 0, inputRunnableIndex, cycle);
        }
    }
}

void Simulation::TraceProcess(int inputRunnableIndex, int inputCycle, int thisRunnableId, int thisCycle, int thisHyperPeriodCount) {
    if (this->dag->GetRunnable(thisRunnableId)->GetStatus() == 1) {
        this->processExecutions[this->dag->GetOutputRunnableIndex(thisRunnableId) * this->numberOfInputRunnables * maxCycle + inputRunnableIndex * maxCycle + inputCycle] = this->runnableExecutions[thisRunnableId * maxCycle + thisCycle] + this->hyperPeriod * hyperPeriodCount;
    } else {
        for (auto &outputRunnable : this->dag->GetRunnable(thisRunnableId)->GetOutputRunnables()) {
            int outputRunnableId = outputRunnable->GetId();

            int tmpCycle = 0;
            double hyperPeriodCount = 0;
            int outputRunnableReadTime = readTable[outputRunnableId * maxCycle + tmpCycle];

            // TODO : regard hyperPeriod
            while ((writeTable[thisRunnableId * maxCycle + thisCycle] + this->hyperPeriod * thisHyperPeriodCount) > outputRunnableReadTime) {
                tmpCycle++;

                if (tmpCycle == maxCycle || readTable[outputRunnableId * maxCycle + tmpCycle] == -1.0) {
                    outputRunnableReadTime = readTable[outputRunnableId * maxCycle] + this->hyperPeriod;
                    tmpCycle = 0;
                    hyperPeriodCount++;
                } else {
                    outputRunnableReadTime = readTable[outputRunnableId * maxCycle + tmpCycle] + (this->hyperPeriod * hyperPeriodCount);
                }
            }

            this->SetArrivalTable(readTable, writeTable, inputRunnableIndex, hyperPeriodCount, inputCycle, outputRunnableId, tmpCycle, maxCycle, arrivalTable);
        }
    }
}

void Simulation::GetReactionTime() {

}

void Simulation::GetDataAge() {

}