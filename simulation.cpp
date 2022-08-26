#include "simulation.hpp"

void Simulation::Initialize() {
    this->maxCycle = this->dag->GetMaxCycle();
    this->hyperPeriod = this->dag->GetHyperPeriod();
    this->numberOfTasks = this->dag->GetNumberOfTasks();
    this->numberOfRunnables = this->dag->GetNumberOfRunnables();
    this->numberOfInputRunnables = this->dag->GetNumberOfInputRunnables();
    this->numberOfOutputRunnables = this->dag->GetNumberOfOutputRunnables();

    std::unique_ptr<RunnableInformation[]> tmpRunnableInformations(new RunnableInformation[this->numberOfRunnables]);
    this->runnableInformations = std::move(tmpRunnableInformations);

    std::unique_ptr<ExecutionInformation[]> tmpRunnableExecutions(new ExecutionInformation[this->numberOfRunnables * maxCycle]);
    this->runnableExecutions = std::move(tmpRunnableExecutions);

    std::unique_ptr<CommunicationInformation[]> tmpRunnableCommunications(new CommunicationInformation[this->numberOfRunnables * maxCycle]);
    this->runnableCommunications = std::move(tmpRunnableCommunications);

    this->ClearTables();
}

void Simulation::ClearTables() {
    RunnableInformation initialRunnableInformation = {-1, -1, -1.0, -1.0, -1.0};
    ExecutionInformation initialExecutionInformation = {-1.0, -1.0};
    CommunicationInformation initialCommunicationInformation = {-1.0, -1.0};

    memcpy(this->runnableInformations.get(), &initialRunnableInformation, sizeof(struct RunnableInformation) * this->numberOfRunnables);
    memcpy(this->runnableExecutions.get(), &initialExecutionInformation, sizeof(struct ExecutionInformation) * this->numberOfRunnables * maxCycle);
    memcpy(this->runnableCommunications.get(), &initialCommunicationInformation, sizeof(struct CommunicationInformation) * this->numberOfRunnables * maxCycle);
}

void Simulation::Simulate() {
    SetRunnableInformations();
    SetRunnableExecutions();
    SetRunnableCommunications();
    SetProcessExecutions();
    GetReactionTime();
    GetDataAge();
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

    std::vector<int> runnablePriority = this->dag->GetRunnablePriority();

    for (auto &task : this->dag->GetTasks()) {
        for (auto &runnable : task->GetRunnables()) {
            int runnableId = runnable->GetId();

            this->runnableInformations[runnableId].taskId = task->GetId();
            this->runnableInformations[runnableId].priority = runnablePriority[runnableId];
            this->runnableInformations[runnableId].period = task->GetPeriod();
            this->runnableInformations[runnableId].offset = task->GetOffset();
            this->runnableInformations[runnableId].executionTime = runnable->GetExecutionTime();
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

    double* emptyTimes = new double[static_cast<int>(this->hyperPeriod)];
    memset(emptyTimes, 1.0, sizeof(double) * static_cast<int>(this->hyperPeriod));

    for (auto &runnable : this->dag->GetOrderOfPriorityRunnables()) {
        int runnableId = runnable->GetId();
        int runnableMaxCycle = static_cast<int>(this->hyperPeriod) / this->runnableInformations[runnableId].period;

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
                    executionTime -= emptyTimes[integerReleaseTime];
                    emptyTimes[integerReleaseTime] = 0.0;

                    integerReleaseTime += 1;
                } else {
                    this->runnableExecutions[runnableId * this->maxCycle + cycle].endTime = static_cast<double>(integerReleaseTime) + executionTime;
                    emptyTimes[integerReleaseTime] -= executionTime;
                    executionTime = 0.0;
                }
            }
        }
    }

    delete[] emptyTimes;
}

void Simulation::SetProcessExecutions() {
    for (auto &runnable : this->dag->GetInputRunnables()) {
        int eachMaxCycle = this->hyperPeriod / this->runnableInformations[runnable->GetId()].period;

        for (int cycle = 0; cycle < eachMaxCycle; cycle++) {
            this->TraceProcess(runnable->GetId(), cycle, runnable->GetId(), cycle, 0);
        }
    }
}

void Simulation::TraceProcess(int inputRunnableId, int inputCycle, int thisRunnableId, int thisCycle, int thisHyperPeriodCount) {
    // --------------------------------------------------------------------------------------------------------------
    // runnableExecutions : [maxCycle X numberOfRunnables]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : First Cycle's ExecutionInformation (startTime, endTime)
    // 2 : Second Cycle's ExecutionInformation (startTime, endTime)
    // ..
    // --------------------------------------------------------------------------------------------------------------
    // runnableCommunications : [maxCycle X numberOfRunnables]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : First Cycle's CommunicationInformation (readTime, writeTime)
    // 2 : Second Cycle's CommunicationInformation (readTime, writeTime)
    // ..
    // --------------------------------------------------------------------------------------------------------------
    // processExecutions : [maxCycle X (InputRunnable, OutputRunnable) pair]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : First Cycle's ExecutionTimeInformation (startTime, writeTime)
    // 2 : Second Cycle's ExecutionTimeInformation (startTime, writeTime)
    // ..
    // --------------------------------------------------------------------------------------------------------------

    if (this->dag->GetRunnable(thisRunnableId)->GetStatus() == 1) {
        if (this->processExecutions.find(std::make_pair(inputRunnableId, thisRunnableId)) != this->processExecutions.end()) {
            std::vector<ExecutionInformation> tmpVector = { {this->runnableExecutions[inputRunnableId * this->maxCycle + inputCycle].startTime, this->runnableExecutions[thisRunnableId * this->maxCycle + thisCycle].endTime + thisHyperPeriodCount * this->hyperPeriod} };
            this->processExecutions.insert(std::make_pair(std::make_pair(inputRunnableId, thisRunnableId), tmpVector));
        } else {
            ExecutionInformation tmpInfo = {this->runnableExecutions[inputRunnableId * this->maxCycle + inputCycle].startTime, this->runnableExecutions[thisRunnableId * this->maxCycle + thisCycle].endTime + thisHyperPeriodCount * this->hyperPeriod};
            this->processExecutions[std::make_pair(inputRunnableId, thisRunnableId)].push_back(tmpInfo);
        }
    } else {
        for (auto &outputRunnable : this->dag->GetRunnable(thisRunnableId)->GetOutputRunnables()) {
            int outputRunnableId = outputRunnable->GetId();

            int tmpCycle = 0;
            double hyperPeriodCount = 0;
            int outputRunnableReadTime = this->runnableExecutions[outputRunnableId * this->maxCycle + tmpCycle].endTime;

            // If thisRunnable has hyperPeriod Count, outputRunnable have same hyperPeriod start.
            while ((this->runnableExecutions[thisRunnableId * maxCycle + thisCycle].startTime) > outputRunnableReadTime) {
                tmpCycle++;

                if (tmpCycle == maxCycle || this->runnableExecutions[outputRunnableId * maxCycle + tmpCycle].endTime == -1.0) {
                    outputRunnableReadTime = this->runnableExecutions[outputRunnableId * maxCycle].endTime + this->hyperPeriod;
                    tmpCycle = 0;
                    hyperPeriodCount++;
                } else {
                    outputRunnableReadTime = this->runnableExecutions[outputRunnableId * maxCycle + tmpCycle].endTime;
                }
            }

            this->TraceProcess(inputRunnableId, inputCycle, outputRunnableId, tmpCycle, hyperPeriodCount);
        }
    }
}

void Simulation::GetReactionTime() {
    double WorstReactionTime = 0.0;
    std::pair<int, int> WorstPair;

    for (auto StoE : processExecutions) {
        double tmpThisReactionTime =  StoE.second[1] - StoE.second[0];
        if (WorstReactionTime < tmpThisReactionTime) {
            WorstReactionTime = tmpThisReactionTime;
            WorstPair.first = StoE.first.first;
            WorstPair.second = StoE.first.second;
        }       
    }
    cout << "Worst Pair Input, Output Runnable : " << WorstPair.first << " " << WorstPair.second << ", Reaction Time : " << WorstReactionTime << endl;
}

void Simulation::GetDataAge() {
    double WorstDataAge = 0.0;
    double WorstReactionTime = 0.0;
    std::pair<int, int> WorstPair;
    std::vector<double> dataage;

    for (auto StoE : processExecutions) {
        double tmpThisReactionTime = StoE.second[1] - StoE.second[0];
        double tmpThisDataAge = tmpThisReactionTime;
        for (auto tmpStoE : processExecutions) {
            // if 

            double tmpThisReactionTime =  StoE.second[1] - StoE.second[0];
            if (WorstReactionTime < tmpThisReactionTime) {
                WorstReactionTime = tmpThisReactionTime;
                WorstPair.first = StoE.first.first;
                WorstPair.second = StoE.first.second;
            }
        }
    }
}