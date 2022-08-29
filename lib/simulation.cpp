#include "simulation.hpp"


double GCD(double a, double b) {
    int tmp1 = static_cast<int>(a * 1000);
    int tmp2 = static_cast<int>(b * 1000);

    while (tmp2 != 0) {
    	int tmp3 = tmp1 % tmp2;
        tmp1 = tmp2;
        tmp2 = tmp3;
    }

    return static_cast<double>(tmp1/1000);
}

void Simulation::Initialize() {
    std::clog << "[simulation.cpp] CheckPoint 0" << std::endl;
    this->maxCycle = this->dag->GetMaxCycle();
    this->hyperPeriod = this->dag->GetHyperPeriod();
    this->numberOfTasks = this->dag->GetNumberOfTasks();
    this->numberOfRunnables = this->dag->GetNumberOfRunnables();
    this->numberOfInputRunnables = this->dag->GetNumberOfInputRunnables();
    this->numberOfOutputRunnables = this->dag->GetNumberOfOutputRunnables();

    std::clog << "[simulation.cpp] CheckPoint 0-1" << std::endl;
    this->ClearTables();

    std::clog << "[simulation.cpp] CheckPoint 0-2" << std::endl;
}

void Simulation::ClearTables() {
    std::clog << "[simulation.cpp] CheckPoint 2-1" << std::endl;
    RunnableInformation initialRunnableInformation = {-1, -1, -1, -1.0, -1.0, -1.0};
    ExecutionInformation initialExecutionInformation = {-1.0, -1.0};

    std::clog << "[simulation.cpp] CheckPoint 2-2" << std::endl;
    std::vector<RunnableInformation>(this->numberOfRunnables, initialRunnableInformation).swap(this->runnableInformations);

    std::clog << "[simulation.cpp] CheckPoint 2-3" << std::endl;
    std::vector<std::vector<ExecutionInformation>>(this->numberOfRunnables, std::vector<ExecutionInformation>(this->maxCycle, initialExecutionInformation)).swap(this->runnableExecutions);

    std::clog << "[simulation.cpp] CheckPoint 2-4" << std::endl;
    std::vector<std::vector<ExecutionInformation>>(this->numberOfRunnables, std::vector<ExecutionInformation>(this->maxCycle, initialExecutionInformation)).swap(this->runnableCommunications);

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

            std::clog << "[simulation.cpp] CheckPoint 3-2" << std::endl;
            this->runnableInformations[runnableId].taskId = task->GetId();
            std::clog << "[simulation.cpp] CheckPoint 3-3" << std::endl;
            this->runnableInformations[runnableId].priority = runnablePriority[runnableId];
            std::clog << "[simulation.cpp] CheckPoint 3-4" << std::endl;
            this->runnableInformations[runnableId].status = runnable->GetStatus();
            std::clog << "[simulation.cpp] CheckPoint 3-5" << std::endl;
            this->runnableInformations[runnableId].period = task->GetPeriod();
            std::clog << "[simulation.cpp] CheckPoint 3-6" << std::endl;
            this->runnableInformations[runnableId].offset = task->GetOffset();
            std::clog << "[simulation.cpp] CheckPoint 3-7" << std::endl;
            this->runnableInformations[runnableId].executionTime = runnable->GetExecutionTime();
            std::clog << "[simulation.cpp] CheckPoint 3-7" << std::endl;
        }
    }
}

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

    double unit = this->runnableInformations[0].period;
    for (auto &task : this->dag->GetTasks()) {
        unit = GCD(unit, ((task->GetOffset() != 0.0) ? GCD(task->GetPeriod(), task->GetOffset()) : task->GetPeriod()));
    }

    std::clog << "[simulation.cpp] CheckPoint 4-1" << std::endl;
    std::vector<double> emptyTimes((static_cast<int>(this->hyperPeriod / unit)), unit);

    for (auto &runnable : this->dag->GetOrderOfPriorityRunnables()) {
        int runnableId = runnable->GetId();
        int runnableMaxCycle = static_cast<int>(this->hyperPeriod) / this->runnableInformations[runnableId].period;
        std::clog << "[simulation.cpp] CheckPoint 4-2" << std::endl;

        for (int cycle = 0; cycle < runnableMaxCycle; cycle++) {
            double releaseTime = this->runnableInformations[runnableId].period * cycle + this->runnableInformations[runnableId].offset;
            double deadTime = this->runnableInformations[runnableId].period * (cycle + 1) + this->runnableInformations[runnableId].offset;

            int unitIndex = static_cast<int>(std::floor(releaseTime / unit));
            std::clog << "[simulation.cpp] CheckPoint 4-3" << std::endl;
            std::cout << unit << std::endl;

            // Regard time-line
            while (emptyTimes[(unitIndex)] == 0.0) unitIndex++;
            std::clog << "[simulation.cpp] CheckPoint 4-4" << std::endl;

            // Set start time
            this->runnableExecutions[runnableId][cycle].startTime = (static_cast<double>(unitIndex) * unit) + (1 - emptyTimes[unitIndex]);
            if (this->runnableExecutions[runnableId][cycle].startTime > deadTime) {
                std::cerr << "[Scheduling Error] : This sequence can't scheduling";
                throw runnable;
            }
            std::clog << "[simulation.cpp] CheckPoint 4-5" << std::endl;

            // Set end time
            double executionTime = this->runnableInformations[runnableId].executionTime;

            while (executionTime) {
                std::clog << "[simulation.cpp] CheckPoint 4-6" << std::endl;
                if (emptyTimes[unitIndex] < executionTime) {
                    executionTime -= emptyTimes[unitIndex];
                    emptyTimes[unitIndex] = 0.0;

                    unitIndex++;
                } else {
                    this->runnableExecutions[runnableId][cycle].endTime = (static_cast<double>(unitIndex) * unit) + executionTime;
                    emptyTimes[unitIndex] -= executionTime;
                    executionTime = 0.0;
                }
            }
            std::clog << "[simulation.cpp] CheckPoint 4-7" << std::endl;
        }
    }
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

    if (this->runnableInformations[thisRunnableId].status == 1) {  //precedence처럼 해보셈
        if (this->processExecutions.find(std::make_pair(inputRunnableId, thisRunnableId)) == this->processExecutions.end()) {
            std::vector<ExecutionInformation> tmpVector = { {this->runnableCommunications[inputRunnableId][inputCycle].startTime, this->runnableCommunications[thisRunnableId][thisCycle].endTime + thisHyperPeriodCount * this->hyperPeriod} };
            this->processExecutions.insert(std::make_pair(std::make_pair(inputRunnableId, thisRunnableId), tmpVector));
        } else {
            ExecutionInformation tmpInfo = {this->runnableCommunications[inputRunnableId][inputCycle].startTime, this->runnableCommunications[thisRunnableId][thisCycle].endTime + thisHyperPeriodCount * this->hyperPeriod};
            this->processExecutions[std::make_pair(inputRunnableId, thisRunnableId)].push_back(tmpInfo); //그냥 push back 말고 worst로 비교해서 넣기
        }
    } else {
        for (auto &outputRunnable : this->dag->GetRunnable(thisRunnableId)->GetOutputRunnables()) {
            int outputRunnableId = outputRunnable->GetId();

            int tmpCycle = 0;
            double hyperPeriodCount = 0;
            int outputRunnableReadTime = this->runnableCommunications[outputRunnableId][tmpCycle].endTime;

            // If thisRunnable has hyperPeriod Count, outputRunnable have same hyperPeriod start.
            while ((this->runnableCommunications[thisRunnableId][thisCycle].startTime) > outputRunnableReadTime) {
                tmpCycle++;

                if (tmpCycle == maxCycle || this->runnableCommunications[outputRunnableId][tmpCycle].endTime == -1.0) {
                    outputRunnableReadTime = this->runnableCommunications[outputRunnableId][0].endTime + this->hyperPeriod;

                    tmpCycle = 0;
                    hyperPeriodCount++;
                } else {
                    double inputStartTime = this->runnableCommunications[thisRunnableId][thisCycle].startTime;
                    double outputEndTime = this->runnableCommunications[outputRunnableId][tmpCycle].endTime;
                    double outputPeriod = this->runnableInformations[outputRunnableId].period;
                    tmpCycle += (((inputStartTime - outputEndTime) / outputPeriod) > 4) ? (static_cast<int>((inputStartTime - outputEndTime) / outputPeriod) - 3) : 0;

                    outputRunnableReadTime = this->runnableCommunications[outputRunnableId][tmpCycle].endTime;
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