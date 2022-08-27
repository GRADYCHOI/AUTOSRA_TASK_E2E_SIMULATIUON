#include "simulation.hpp"

void Simulation::Initialize() {
    std::clog << "[simulation.cpp] CheckPoint 0" << std::endl;
    this->maxCycle = this->dag->GetMaxCycle();
    this->hyperPeriod = this->dag->GetHyperPeriod();
    this->numberOfTasks = this->dag->GetNumberOfTasks();
    this->numberOfRunnables = this->dag->GetNumberOfRunnables();
    this->numberOfInputRunnables = this->dag->GetNumberOfInputRunnables();
    this->numberOfOutputRunnables = this->dag->GetNumberOfOutputRunnables();

    std::clog << "[simulation.cpp] CheckPoint 0-1" << std::endl;
    std::unique_ptr<RunnableInformation[]> tmpRunnableInformations(new RunnableInformation[this->numberOfRunnables]);
    this->runnableInformations = std::move(tmpRunnableInformations);

    std::clog << "[simulation.cpp] CheckPoint 0-2" << std::endl;
    std::unique_ptr<ExecutionInformation[]> tmpRunnableExecutions(new ExecutionInformation[this->numberOfRunnables * this->maxCycle]);
    this->runnableExecutions = std::move(tmpRunnableExecutions);

    std::clog << "[simulation.cpp] CheckPoint 0-3" << std::endl;
    std::unique_ptr<CommunicationInformation[]> tmpRunnableCommunications(new CommunicationInformation[this->numberOfRunnables * this->maxCycle]);
    this->runnableCommunications = std::move(tmpRunnableCommunications);

    std::clog << "[simulation.cpp] CheckPoint 0-4" << std::endl;
    this->ClearTables();

    std::clog << "[simulation.cpp] CheckPoint 0-5" << std::endl;
}

void Simulation::ClearTables() {
    std::clog << "[simulation.cpp] CheckPoint 2-1" << std::endl;
    RunnableInformation initialRunnableInformation = {-1, -1, -1.0, -1.0, -1.0};
    ExecutionInformation initialExecutionInformation = {-1.0, -1.0};
    CommunicationInformation initialCommunicationInformation = {-1.0, -1.0};

    std::clog << "[simulation.cpp] CheckPoint 2-2" << std::endl;
    memcpy(this->runnableInformations.get(), &initialRunnableInformation, sizeof(RunnableInformation) * this->numberOfRunnables);

    std::clog << "[simulation.cpp] CheckPoint 2-3" << std::endl;
    std::clog << "[simulation.cpp] runnableExecutions size : " << sizeof(ExecutionInformation) * this->numberOfRunnables * this->maxCycle << std::endl;
    memcpy(this->runnableExecutions.get(), &initialExecutionInformation, sizeof(ExecutionInformation) * this->numberOfRunnables * this->maxCycle);

    std::clog << "[simulation.cpp] CheckPoint 2-4" << std::endl;
    memcpy(this->runnableCommunications.get(), &initialCommunicationInformation, sizeof(CommunicationInformation) * this->numberOfRunnables * this->maxCycle);

    std::clog << "[simulation.cpp] CheckPoint 2-5" << std::endl;
}

void Simulation::Simulate() {
    int numberOfCase = this->dag->GetNumberOfSequenceCase();

    for (int caseIndex = 0; caseIndex < numberOfCase; caseIndex++) {
        std::clog << "[simulation.cpp] CheckPoint 1" << std::endl;
        this->dag->SetRunnablePriority(caseIndex);

        std::clog << "[simulation.cpp] CheckPoint 2" << std::endl;
        this->ClearTables();

        std::clog << "[simulation.cpp] CheckPoint 3" << std::endl;
        this->SetRunnableInformations();

        std::clog << "[simulation.cpp] CheckPoint 4" << std::endl;
        this->SetRunnableExecutions();

        std::clog << "[simulation.cpp] CheckPoint 5" << std::endl;
        this->SetRunnableCommunications();
        
        std::clog << "[simulation.cpp] CheckPoint 6" << std::endl;
        this->SetProcessExecutions();

        std::clog << "[simulation.cpp] CheckPoint 7" << std::endl;
        this->GetReactionTime();

        std::clog << "[simulation.cpp] CheckPoint 8" << std::endl;
        //this->GetDataAge();

        std::clog << "[simulation.cpp] CheckPoint 9" << std::endl;
    }
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
            std::clog << "[simulation.cpp] CheckPoint 3-1" << std::endl;
            int runnableId = runnable->GetId();
            std::clog << "[simulation.cpp] CheckPoint 3-2, " << task->GetId() << ", "<< this->numberOfRunnables << std::endl;
            this->runnableInformations[runnableId].taskId = task->GetId();
            std::clog << "[simulation.cpp] CheckPoint 3-3" << std::endl;
            this->runnableInformations[runnableId].priority = runnablePriority[runnableId];
            std::clog << "[simulation.cpp] CheckPoint 3-4" << std::endl;
            this->runnableInformations[runnableId].period = task->GetPeriod();
            std::clog << "[simulation.cpp] CheckPoint 3-5" << std::endl;
            this->runnableInformations[runnableId].offset = task->GetOffset();
            std::clog << "[simulation.cpp] CheckPoint 3-6" << std::endl;
            this->runnableInformations[runnableId].executionTime = runnable->GetExecutionTime();
            std::clog << "[simulation.cpp] CheckPoint 3-7" << std::endl;
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
    std::pair<int, int> WorstPair;

    for (auto &StoEs : processExecutions) {
        double WorstReactionTime =  0.0;

        for (auto &StoE : StoEs.second) {
            double tmpThisReactionTime = StoE.endTime - StoE.startTime;
            if (WorstReactionTime < tmpThisReactionTime) {
                WorstReactionTime = tmpThisReactionTime;
                WorstPair.first = StoE.startTime;
                WorstPair.second = StoE.endTime;
            }
        }

        std::cout << "Worst Pair Input, Output Runnable : " << StoEs.first.first << " " << StoEs.first.second << ", Reaction Time : " << WorstReactionTime << std::endl;
    }
}

void Simulation::GetDataAge() {
   /* 
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
    }*/
}

void Simulation::SaveData() {

}

void Simulation::SaveDAG() {

}

void Simulation::SaveMapping() {

}

void Simulation::SaveReactionTime() {

}

void Simulation::SaveDataAge() {

}