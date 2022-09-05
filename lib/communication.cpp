#include "communication.hpp"


void RunnableImplicit::GetCommunicationTable(Simulation* simulationSelf, std::vector<std::vector<ExecutionInformation>>& runnableCommunications) {
    std::vector<std::vector<std::shared_ptr<RUNNABLE>>> sequence; // [Priority][Runnable]
	sequence.reserve(simulationSelf->numberOfRunnables_);

    for (auto &task : simulationSelf->dag->GetTaskPriority()) {
        // sort by precedence in same task
        int numberOfRunnables = task->GetNumberOfRunnables();
        std::vector<std::shared_ptr<RUNNABLE>> runnableList(numberOfRunnables);

        for (auto &runnable : task->GetRunnables()) {
            runnableList.push_back(runnable);
        }

        std::sort(runnableList.begin(), runnableList.end(), [](std::shared_ptr<Runnable> a, std::shared_ptr<Runnable> b) { return a->GetPrecedence() < b->GetPprecedence(); });

        // compress same precedence in vector
        std::vector<std::vector<std::shared_ptr<RUNNABLE>>> sequencePerTask;

        for (auto &runnable : runnableList) {
            if (sequencePerTask.back()[0]->GetPrecedence() == runnable->GetPrecedence()) {
                sequencePerTask.back().push_back(runnable);
            } else {
                sequencePerTask.push_back(std::vector<std::shared_ptr<RUNNABLE>>(runnable, 1));
            }
        }

        // add seqeunce
        sequence.append(sequencePerTask);
    }

    int numberOfCase = 1;
    std::vector<int> numberOfCasePerPriority(static_cast<int>(sequence.size()));
    std::vector<std::shared_ptr<RUNNABLE>> allCasePerPriority(numberOfPermutation);

    for (auto samePriorityRunnables : sequence) {
        int numberOfPermutation = simulationSelf->GetNumberOfPermutation(static_cast<int>(samePriorityRunnables.size()));
        
        numberOfCase *= numberOfPermutation;
        numberOfCasePerPriority.push_back(numberOfPermutation);

        std::sort(samePriorityRunnables.begin(), samePriorityRunnables.end(), [](std::shared_ptr<RUNNABLE> a, std::shared_ptr<RUNNABLE> b) { return a->GetId() < b->GetId(); });

        do {
            allCasePerPriority.push_back(samePriorityRunnables);
        } while (std::next_permutation(samePriorityRunnables.begin(), samePriorityRunnables.end(), [](std::shared_ptr<RUNNABLE> a, std::shared_ptr<RUNNABLE> b) { return a->GetId() < b->GetId(); }));
    }
	
    std::vector<std::vector<std::vector<double>>> executionInformationPerPriority;
    
    std::copy(runnableExecutions.begin(), runnableExecutions.end(), runnableCommunications.begin());
}

void TaskImplicit::GetCommunicationTable(Simulation* simulationSelf, std::vector<std::vector<ExecutionInformation>>& runnableCommunications) {
    double unit = simulationSelf->runnableInformations[0].period;
    for (auto &task : simulationSelf->dag_->GetTasks()) {
        unit = std::gcd(unit, ((task->GetOffset() != 0.0) ? GCD(task->GetPeriod(), task->GetOffset()) : task->GetPeriod()));
    }

    std::vector<double> emptyTimes((static_cast<int>(this->hyperPeriod / unit)), unit);

    ExecutionInformation initialExecutionInformation = {-1.0, -1.0};
    runnableCommunicatioins.resize(simulationSelf->numberOfRunnables_);

    for (auto &task : simulationSelf->dag_->GetTasks()) {
        int maxCycle = simulationSelf->hyperPeriod_ / task->GetPeriod();
        double period = task->GetPeriod();
        double offset = task->GetOffset();
        double executionTime = task->GetExecutionTime();

        std::vector<ExecutionInformation> taskExecutionInformation(maxCycle, initialExecutionInformation);

        for (int cycle = 0; cycle < maxCycle; cycle++) {
            double releaseTime = period * cycle + offset;
            double deadTime = period * (cycle + 1) + offset;

            int unitIndex = static_cast<int>(std::floor(releaseTime / unit));

            // Regard time-line
            while (emptyTimes[(unitIndex)] == 0.0) unitIndex++;

            // Set start time
            this->runnableExecutions[runnableId][cycle].startTime = (static_cast<double>(unitIndex) * unit) + (1 - emptyTimes[unitIndex]);
            if (this->runnableExecutions[runnableId][cycle].startTime > deadTime) {
                std::cerr << "[Scheduling Error] : This sequence can't scheduling";
                throw runnable;
            }
        }

        for (auto &runnable : task->GetRunnables()) {
            int runnableId = runnable->GetId();
            runnableCommunications[runnableId].push_back(std::vector<ExecutionInformation>(maxCycle, initialExecutionInformation));

            for (int cycle = 0; cycle < maxCycle; cycle++) {
                runnableCommunications[runnableId][0][cycle].startTime = taskStartTime;
                runnableCommunications[runnableId][0][cycle].endTime = taskEndTime;
            }
        }
    }

    // 참고용
    for (auto &runnable : this->dag->GetOrderOfPriorityRunnables()) {
        int runnableId = runnable->GetId();
        int runnableMaxCycle = static_cast<int>(this->hyperPeriod) / this->runnableInformations[runnableId].period;

        for (int cycle = 0; cycle < runnableMaxCycle; cycle++) {
            double releaseTime = this->runnableInformations[runnableId].period * cycle + this->runnableInformations[runnableId].offset;
            double deadTime = this->runnableInformations[runnableId].period * (cycle + 1) + this->runnableInformations[runnableId].offset;

            int unitIndex = static_cast<int>(std::floor(releaseTime / unit));

            // Regard time-line
            while (emptyTimes[(unitIndex)] == 0.0) unitIndex++;

            // Set start time
            this->runnableExecutions[runnableId][cycle].startTime = (static_cast<double>(unitIndex) * unit) + (1 - emptyTimes[unitIndex]);
            if (this->runnableExecutions[runnableId][cycle].startTime > deadTime) {
                std::cerr << "[Scheduling Error] : This sequence can't scheduling";
                throw runnable;
            }

            // Set end time
            double executionTime = this->runnableInformations[runnableId].executionTime;

            while (executionTime) {
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
        }
    }
}

void LET::GetCommunicationTable(Simulation* simulationSelf, std::vector<std::vecot<int>>& numberOfCases, std::vector<std::vector<std::vector<<ExecutionInformation>>>& runnableCommunications) {
    ExecutionInformation initialExecutionInformation = {-1.0, -1.0};
    runnableCommunicatioins.resize(simulationSelf->numberOfRunnables_);

    for (auto &task : simulationSelf->dag_->GetTasks()) {
        int maxCycle = simulationSelf->hyperPeriod_ / task->GetPeriod();
        double period = task->GetPeriod();
        double offset = task->GetOffset();

        for (auto &runnable : task->GetRunnables()) {
            int runnableId = runnable->GetId();
            runnableCommunications[runnableId].push_back(std::vector<ExecutionInformation>(maxCycle, initialExecutionInformation));

            for (int cycle = 0; cycle < maxCycle; cycle++) {
                runnableCommunications[runnableId][0][cycle].startTime = period * cycle + offset;
                runnableCommunications[runnableId][0][cycle].endTime = period * (cycle + 1) + offset;
            }
        }
    }
}