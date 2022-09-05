#include "communication.hpp"


void RunnableImplicit::GetCommunicationTable(Simulation* simulationSelf, std::vector<std::vector<std::vector<int>>>& runnablePermutation, std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableCommunications) {
    std::vector<std::vector<std::shared_ptr<RUNNABLE>>> sequence; // [Priority][Runnable]
	sequence.reserve(simulationSelf->numberOfRunnables_);

    // Same Priority Runnables
    for (auto &task : simulationSelf->dag_->GetTaskPriority()) {
        // sort by precedence in same task
        int numberOfRunnables = task->GetNumberOfRunnables();
        std::vector<std::shared_ptr<RUNNABLE>> runnableList(numberOfRunnables);

        for (auto &runnable : task->GetRunnables()) {
            runnableList.emplace_back(runnable);
        }

        std::sort(runnableList.begin(), runnableList.end(), [](std::shared_ptr<Runnable> a, std::shared_ptr<Runnable> b) { return a->GetPrecedence() < b->GetPprecedence(); });

        // compress same precedence in vector
        std::vector<std::vector<std::shared_ptr<RUNNABLE>>> sequencePerTask;

        for (auto &runnable : runnableList) {
            if (sequencePerTask.back()[0]->GetPrecedence() == runnable->GetPrecedence()) {
                sequencePerTask.back().emplace_back(runnable);
            } else {
                sequencePerTask.emplace_back(std::vector<std::shared_ptr<RUNNABLE>>(1, runnable));
            }
        }

        // add seqeunce
        sequence.insert(sequence.end(), sequencePerTask.begin(), sequencePerTask.end());
    }

    // Permutation in Same Priority Runnables
    int numberOfCase = 1;
    std::vector<int> numberOfCasePerPriority(static_cast<int>(sequence.size()));
    std::vector<std::vector<std::shared_ptr<RUNNABLE>>> allCasePerPriority(numberOfPermutation);

    for (auto samePriorityRunnables : sequence) {
        int numberOfPermutation = simulationSelf->GetNumberOfPermutation(static_cast<int>(samePriorityRunnables.size()));

        std::sort(samePriorityRunnables.begin(), samePriorityRunnables.end(), [](std::shared_ptr<RUNNABLE> a, std::shared_ptr<RUNNABLE> b) { return a->GetId() < b->GetId(); });

        if (static_cast<int>(samePriorityRunnables.size()) > 1) {
            std::vector<std::vector<int>> tmpRunnablePermutation;

            do {
                std::vector<int> tmpRunnables;
                for (auto &runnable : samePriorityRunnables) {
                    tmpRunnables.emplace_back(runnable->GetId());
                }
                tmpRunnablePermutation.emplace_back(tmpRunnables);

                allCasePerPriority.emplace_back(samePriorityRunnables);
            } while (std::next_permutation(samePriorityRunnables.begin(), samePriorityRunnables.end(), [](std::shared_ptr<RUNNABLE> a, std::shared_ptr<RUNNABLE> b) { return a->GetId() < b->GetId(); }));

            runnablePermutation.emplace_back(tmpRunnablePermutation);
        } else {
            std::vector<int> tmpRunnables;
            for (auto &runnable : samePriorityRunnables) {
                tmpRunnables.emplace_back(runnable->GetId());
            }
            runnablePermutation.emplace_back(std::vector<std::vector<int>>(1, tmpRunnables));

            allCasePerPriority.emplace_back(samePriorityRunnables);
        }
    }

    

    // Set Release Time in Runnables
    ExecutionInformation initialExecutionInformation = {-1.0, -1.0};
    std::vector<std::vector<ExecutionInformation>> runnableReleaseTimes(simulationSelf->numberOfRunnables_);

    for (auto &task : simulationSelf->dag_->GetTasks()) {
        int maxCycle = simulationSelf->hyperPeriod_ / task->GetPeriod();
        double period = task->GetPeriod();
        double offset = task->GetOffset();

        for (auto &runnable : task->GetRunnables()) {
            int runnableId = runnable->GetId();
            runnableReleaseTimes[runnableId].swap(std::vector<ExecutionInformation>(maxCycle, initialExecutionInformation));

            for (int cycle = 0; cycle < maxCycle; cycle++) {
                runnableReleaseTimes[runnableId][cycle].startTime = period * cycle + offset;
                runnableReleaseTimes[runnableId][cycle].endTime = period * (cycle + 1) + offset;
            }
        }
    }

    // Set Execution Times
    double unit = simulationSelf->dag_->GetTask(0)->GetPeriod();
    for (auto &task : simulationSelf->dag_->GetTasks()) {
        unit = std::gcd(unit, ((task->GetOffset() != 0.0) ? GCD(task->GetPeriod(), task->GetOffset()) : task->GetPeriod()));
    }

    std::vector<double> emptyTimes((static_cast<int>(simulationSelf->GetHyperPeriod() / unit)), unit);

    runnableCommunicatioins.resize(this->numberOfRunnables_);
    for (auto &oneCaseSequence : allCasePerPriority) {
        for (auto &runnable : oneCaseSequence) {
            int runnableId = runnable->GetId();
            int maxCycle = static_cast<int>(runnableReleaseTimes[runnableId].size());
            runnableCommunications[runnableId].emplace_back(runnableReleaseTimes[runnableId]);

            for (int cycle = 0; cycle < maxCycle; cycle++) {
                int unitIndex = static_cast<int>(std::floor((runnableReleaseTimes[runnableId].back())[cycle] / unit));

                // Regard time-line
                while (emptyTimes[(unitIndex)] == 0.0) unitIndex++;

                // Set start time
                (runnableReleaseTimes[runnableId].back())[cycle].startTime = (static_cast<double>(unitIndex) * unit) + (1 - emptyTimes[unitIndex]);

                // Set end time
                double executionTime = runnable->GetExecutionTime();

                while (executionTime) {
                    if (emptyTimes[unitIndex] < executionTime) {
                        executionTime -= emptyTimes[unitIndex];
                        emptyTimes[unitIndex] = 0.0;

                        unitIndex++;
                    } else {
                        (runnableReleaseTimes[runnableId].back())[cycle].endTime = (static_cast<double>(unitIndex) * unit) + executionTime;
                        emptyTimes[unitIndex] -= executionTime;
                        executionTime = 0.0;
                    }
                }

                if (cycle != (maxCycle - 1)) {
                    if ((runnableReleaseTimes[runnableId].back())[cycle + 1].startTime < (runnableReleaseTimes[runnableId].back())[cycle].endTime) {
                        std::cerr << "[Scheduling Error] : This sequence can't scheduling";
                        throw runnable;
                    }
                }
            }
        }
    }
}

void TaskImplicit::GetCommunicationTable(Simulation* simulationSelf, std::vector<std::vector<std::vector<int>>>& runnablePermutation, std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableCommunications) {
    double unit = simulationSelf->dag_->GetTask(0)->GetPeriod();
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
            double startTime = (static_cast<double>(unitIndex) * unit) + (1 - emptyTimes[unitIndex]);
            double endTime;

            // Set end time
            while (executionTime) {
                if (emptyTimes[unitIndex] < executionTime) {
                    executionTime -= emptyTimes[unitIndex];
                    emptyTimes[unitIndex] = 0.0;

                    unitIndex++;
                } else {
                    endTime = (static_cast<double>(unitIndex) * unit) + executionTime;
                    emptyTimes[unitIndex] -= executionTime;
                    executionTime = 0.0;
                }
            }

            if (endTime > deadTime) {
                std::cerr << "[Scheduling Error] : This sequence can't scheduling";
                throw runnable;
            }

            taskExecutionInformation[cycle].startTime = startTime;
            taskExecutionInformation[cycle].endTime = endTime;
        }

        for (auto &runnable : task->GetRunnables()) {
            int runnableId = runnable->GetId();
            runnableCommunicatioins[runnableId].emplace_back(taskExecutionInformation);
        }
    }
}

void LET::GetCommunicationTable(Simulation* simulationSelf, std::vector<std::vector<std::vector<int>>>& runnablePermutation, std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableCommunications) {
    ExecutionInformation initialExecutionInformation = {-1.0, -1.0};
    runnableCommunicatioins.resize(simulationSelf->numberOfRunnables_);

    for (auto &task : simulationSelf->dag_->GetTasks()) {
        int maxCycle = simulationSelf->hyperPeriod_ / task->GetPeriod();
        double period = task->GetPeriod();
        double offset = task->GetOffset();

        for (auto &runnable : task->GetRunnables()) {
            int runnableId = runnable->GetId();
            runnableCommunications[runnableId].emplace_back(std::vector<ExecutionInformation>(maxCycle, initialExecutionInformation));

            for (int cycle = 0; cycle < maxCycle; cycle++) {
                runnableCommunications[runnableId][0][cycle].startTime = period * cycle + offset;
                runnableCommunications[runnableId][0][cycle].endTime = period * (cycle + 1) + offset;
            }
        }
    }
}