#include "communication.hpp"


int GetNumberOfPermutation(int number) {
    int tmpNumber = 1;

    for (int count = number; count > 1; count--) {
        tmpNumber *= count;
    }

    return tmpNumber;
}

void RunnableImplicit::GetCommunicationTable(std::shared_ptr<DAG>& dag, int numberOfRunnables, long long int hyperPeriod, std::vector<std::vector<std::vector<int>>>& runnablePermutation, std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableCommunications) {
    std::vector<std::vector<std::shared_ptr<RUNNABLE>>> sequence; // [Priority][Runnable]
	sequence.reserve(numberOfRunnables);

    // Seperate Runnables By Priority
    for (auto &task : dag->GetTasksPriority()) {
        // sort by precedence in same task
        int numberOfRunnablesInTask = task->GetNumberOfRunnables();
        std::clog << "[Communication.cpp] Number Of Runnables in " << task->GetId() << "th Task : " << numberOfRunnablesInTask << std::endl;

        // compress same precedence in vector
        std::vector<std::vector<std::shared_ptr<RUNNABLE>>> sequencePerTask;
        sequencePerTask.reserve(numberOfRunnablesInTask);

        for (auto &runnable : task->GetRunnablesByPriorityInTask()) {
            if (sequencePerTask.size()) {
                if (sequencePerTask.back()[0]->GetPriorityInTask() == runnable->GetPriorityInTask()) {
                    sequencePerTask.back().emplace_back(runnable);
                } else {
                    sequencePerTask.emplace_back(std::vector<std::shared_ptr<RUNNABLE>>(1, runnable));
                }
            } else {
                sequencePerTask.emplace_back(std::vector<std::shared_ptr<RUNNABLE>>(1, runnable));
            }
        }

        // add seqeunce
        sequence.insert(sequence.end(), sequencePerTask.begin(), sequencePerTask.end());
    }

    // Permutation in Same Priority Runnables
    int numberOfCase = 0;
    for (auto &samePriorityRunnables : sequence) {
        numberOfCase += GetNumberOfPermutation(static_cast<int>(samePriorityRunnables.size()));
    }

    std::vector<std::vector<std::shared_ptr<RUNNABLE>>> allCasePerPriority(numberOfCase);

    for (auto samePriorityRunnables : sequence) {
        //int numberOfPermutation = GetNumberOfPermutation(static_cast<int>(samePriorityRunnables.size()));

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
            runnablePermutation.emplace_back(std::vector<std::vector<int>>(1, std::vector<int>(1, samePriorityRunnables.front()->GetId())));

            allCasePerPriority.emplace_back(samePriorityRunnables);
        }
    }

    // Set Release Time in Runnables
    ExecutionInformation initialExecutionInformation = {-1, -1};
    std::vector<std::vector<ExecutionInformation>> runnableReleaseTimes(numberOfRunnables);

    for (auto &task : dag->GetTasks()) {
        int maxCycle = static_cast<int>(hyperPeriod / task->GetPeriod());
        int period = task->GetPeriod();
        int offset = task->GetOffset();

        for (auto &runnable : task->GetRunnables()) {
            int runnableId = runnable->GetId();
            std::vector<ExecutionInformation>(maxCycle, initialExecutionInformation).swap(runnableReleaseTimes[runnableId]);

            for (int cycle = 0; cycle < maxCycle; cycle++) {
                runnableReleaseTimes[runnableId][cycle].startTime = static_cast<long long int>(period) * static_cast<long long int>(cycle) + static_cast<long long int>(offset);
                runnableReleaseTimes[runnableId][cycle].endTime = static_cast<long long int>(period) * static_cast<long long int>(cycle + 1) + static_cast<long long int>(offset);
            }
        }
    }

    // Set Execution Times
    int unit = dag->GetTask(0)->GetPeriod();
    for (auto &task : dag->GetTasks()) {
        unit = std::gcd(unit, ((task->GetOffset() != 0) ? std::gcd(task->GetPeriod(), task->GetOffset()) : task->GetPeriod()));
    }

    std::vector<int> emptyTimes(static_cast<int>(hyperPeriod / unit), unit);

    runnableCommunications.resize(numberOfRunnables);
    for (auto &oneCaseSequence : allCasePerPriority) {
        for (auto &runnable : oneCaseSequence) {
            int runnableId = runnable->GetId();
            int maxCycle = static_cast<int>(runnableReleaseTimes[runnableId].size());
            runnableCommunications[runnableId].emplace_back(runnableReleaseTimes[runnableId]);

            for (int cycle = 0; cycle < maxCycle; cycle++) {
                int unitIndex = static_cast<int>(runnableReleaseTimes[runnableId][cycle].startTime / unit);

                // Regard time-line
                while (emptyTimes[unitIndex] == 0) unitIndex++;

                // Set start time
                runnableCommunications[runnableId].back()[cycle].startTime = static_cast<long long int>(unitIndex) * static_cast<long long int>(unit) + static_cast<long long int>(1 - emptyTimes[unitIndex]);

                // Set end time
                int executionTime = runnable->GetExecutionTime();

                while (executionTime) {
                    if (emptyTimes[unitIndex] < executionTime) {
                        executionTime -= emptyTimes[unitIndex];
                        emptyTimes[unitIndex] = 0;

                        unitIndex++;
                    } else {
                        runnableCommunications[runnableId].back()[cycle].endTime = static_cast<long long int>(unitIndex) * static_cast<long long int>(unit) + static_cast<long long int>(executionTime);
                        emptyTimes[unitIndex] -= executionTime;
                        executionTime = 0;
                    }
                }

                if (runnableReleaseTimes[runnableId][cycle].endTime < runnableCommunications[runnableId].back()[cycle].endTime) {
                    std::cout << "[Scheduling Error] : This sequence can't scheduling" << std::endl;
                    exit(0);
                }
            }
        }
    }
}

void TaskImplicit::GetCommunicationTable(std::shared_ptr<DAG>& dag, int numberOfRunnables, long long int hyperPeriod, std::vector<std::vector<std::vector<int>>>& runnablePermutation, std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableCommunications) {
    runnablePermutation.reserve(numberOfRunnables);

    int unit = dag->GetTask(0)->GetPeriod();
    for (auto &task : dag->GetTasks()) {
        unit = std::gcd(unit, ((task->GetOffset() != 0) ? std::gcd(task->GetPeriod(), task->GetOffset()) : task->GetPeriod()));
    }

    std::vector<int> emptyTimes(static_cast<int>(hyperPeriod / unit), unit);

    ExecutionInformation initialExecutionInformation = {-1, -1};
    runnableCommunications.resize(numberOfRunnables);

    for (auto &task : dag->GetTasksPriority()) {
        int maxCycle = static_cast<int>(hyperPeriod / task->GetPeriod());
        int period = task->GetPeriod();
        int offset = task->GetOffset();
        int executionTime = task->GetExecutionTime();

        std::vector<ExecutionInformation> taskExecutionInformation(maxCycle, initialExecutionInformation);

        for (int cycle = 0; cycle < maxCycle; cycle++) {
            long long int releaseTime = period * cycle + offset;
            long long int deadTime = period * (cycle + 1) + offset;

            int unitIndex = static_cast<int>(releaseTime / static_cast<long long int>(unit));

            // Regard time-line
            while (emptyTimes[(unitIndex)] == 0) unitIndex++;

            // Set start time
            long long int startTime = static_cast<long long int>(unitIndex) * static_cast<long long int>(unit) + static_cast<long long int>(1 - emptyTimes[unitIndex]);
            long long int endTime;

            // Set end time
            while (executionTime) {
                if (emptyTimes[unitIndex] < executionTime) {
                    executionTime -= emptyTimes[unitIndex];
                    emptyTimes[unitIndex] = 0;

                    unitIndex++;
                } else {
                    endTime = (unitIndex * unit) + executionTime;
                    emptyTimes[unitIndex] -= executionTime;
                    executionTime = 0;
                }
            }

            if (endTime > deadTime) {
                std::cerr << "[Scheduling Error] : This sequence can't scheduling";
            }

            taskExecutionInformation[cycle].startTime = startTime;
            taskExecutionInformation[cycle].endTime = endTime;
        }

        for (auto &runnable : task->GetRunnablesByPriorityInTask()) {
            int runnableId = runnable->GetId();

            runnablePermutation.emplace_back(std::vector<std::vector<int>>(1, std::vector<int>(1, runnableId)));
            runnableCommunications[runnableId].emplace_back(taskExecutionInformation);
        }
    }
}

void LET::GetCommunicationTable(std::shared_ptr<DAG>& dag, int numberOfRunnables, long long int hyperPeriod, std::vector<std::vector<std::vector<int>>>& runnablePermutation, std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableCommunications) {
    runnablePermutation.reserve(numberOfRunnables);

    ExecutionInformation initialExecutionInformation = {-1, -1};
    runnableCommunications.resize(numberOfRunnables);

    for (auto &task : dag->GetTasksPriority()) {
        int maxCycle = hyperPeriod / task->GetPeriod();
        int period = task->GetPeriod();
        int offset = task->GetOffset();

        for (auto &runnable : task->GetRunnables()) {
            int runnableId = runnable->GetId();

            runnablePermutation.emplace_back(std::vector<std::vector<int>>(1, std::vector<int>(1, runnableId)));
            runnableCommunications[runnableId].emplace_back(std::vector<ExecutionInformation>(maxCycle, initialExecutionInformation));

            for (int cycle = 0; cycle < maxCycle; cycle++) {
                runnableCommunications[runnableId][0][cycle].startTime = period * cycle + offset;
                runnableCommunications[runnableId][0][cycle].endTime = period * (cycle + 1) + offset;
            }
        }
    }
}