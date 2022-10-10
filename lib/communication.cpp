#include "communication.hpp"


void RunnableImplicit::GetCommunicationTable(std::shared_ptr<DAG>& dag, std::vector<std::vector<RequiredTime>>& runnableTimeTable) {
    // Get unit time
    int unit = dag->GetTask(0)->period_;
    for (auto &task : dag->GetTasks()) {
        unit = std::gcd(unit, ((task->GetOffset() != 0) ? std::gcd(task->GetPeriod(), task->GetOffset()) : task->GetPeriod()));
    }

    // Get number of cores
    int numberOfCores = -1;
    for (auto &task : dag->GetTasks()) {
        numberOfCores = (numberOfCores > task->GetCore()) ? numberOfCores : task->GetCore();
    }

    // Set time table
    for (int coreIndex = 0; coreIndex < numberOfCores; coreIndex++) {
        // Set time vector
        std::vector<int> emptyTimes(static_cast<int>(dag->GetHyperPeriod() / unit), unit);

        // Set Time-Table
        for (auto &task : dag->GetTask()) {
            if (task->GetCore() == coreIndex) {
                int maxCycle = static_cast<int>(dag->GetHyperPeriod() / task->period_);
                int unitIndex = (task->period_ * cycle + task->offset_) / unit;

                for (int cycle = 0; cycle < maxCycle; cycle++) {
                    for (auto &runnable : task->GetRunnables()) {                  
                        // Regard time-line
                        while (emptyTimes[unitIndex] == 0) unitIndex++;

                        // Set start time
                        runnableTimeTable[runnable->id_][cycle].startTime = static_cast<long long int>(unitIndex) * static_cast<long long int>(unit) + static_cast<long long int>(unit - currentEmptyTimes[unitIndex]);

                        int executionTime = runnable->executionTime_;
                        while (executionTime) {
                            if (emptyTimes[unitIndex] < executionTime) {
                                executionTime -= emptyTimes[unitIndex];
                                emptyTimes[unitIndex] = 0;

                                unitIndex++;
                            } else {
                                runnableTimeTable[runnable->id_][cycle].endTime = static_cast<long long int>(unitIndex) * static_cast<long long int>(unit) + static_cast<long long int>(executionTime);
                                emptyTimes[unitIndex] -= executionTime;
                                executionTime = 0;
                            }
                        }
                    }
                }
            }
        }
    }
}

void TaskImplicit::GetCommunicationTable(std::shared_ptr<DAG>& dag, std::vector<std::vector<RequiredTime>>& runnableTimeTable) {
}

void LET::GetCommunicationTable(std::shared_ptr<DAG>& dag, std::vector<std::vector<RequiredTime>>& runnableTimeTable) {
}