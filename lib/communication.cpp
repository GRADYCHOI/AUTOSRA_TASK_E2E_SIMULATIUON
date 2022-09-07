#include "communication.hpp"


void RunnableImplicit::GetCommunicationTable(std::shared_ptr<DAG>& dag, int numberOfRunnables, double hyperPeriod, std::vector<std::vector<std::vector<int>>>& runnablePermutation, std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableCommunications) {
    std::vector<std::vector<std::shared_ptr<RUNNABLE>>> sequence; // [Priority][Runnable]
	sequence.reserve(numberOfRunnables);

    // Same Priority Runnables
    for (auto &task : dag->GetTasksPriority()) {
        // sort by precedence in same task
        int numberOfRunnablesInTask = task->GetNumberOfRunnables();
        std::vector<std::shared_ptr<RUNNABLE>> runnableList;
        runnableList.reserve(numberOfRunnablesInTask);
        std::clog << "[Communication.cpp] Number Of Runnables in " << task->GetId() << "th Task : " << numberOfRunnablesInTask << std::endl;

        for (auto &runnable : task->GetRunnables()) {
            runnableList.emplace_back(runnable);
        }

        std::sort(runnableList.begin(), runnableList.end(), [](std::shared_ptr<RUNNABLE> a, std::shared_ptr<RUNNABLE> b) { return a->GetPrecedence() < b->GetPrecedence(); });

        // compress same precedence in vector
        std::vector<std::vector<std::shared_ptr<RUNNABLE>>> sequencePerTask;
        sequencePerTask.reserve(numberOfRunnablesInTask);

        for (auto &runnable : runnableList) {
            if (sequencePerTask.size()) {
                if (sequencePerTask.back()[0]->GetPrecedence() == runnable->GetPrecedence()) {
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
        int samePriorityRunnablesCase = 1;

        for (int count = static_cast<int>(samePriorityRunnables.size()); count > 1; count--) {
            samePriorityRunnablesCase *= count;
        }

        numberOfCase += samePriorityRunnablesCase;
    }

    std::vector<std::vector<std::shared_ptr<RUNNABLE>>> allCasePerPriority(numberOfCase);

    for (auto samePriorityRunnables : sequence) {
        int numberOfPermutation = 1;

        for (int count = static_cast<int>(samePriorityRunnables.size()); count > 1; count--) {
            numberOfPermutation *= count;
        }

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
    std::vector<std::vector<ExecutionInformation>> runnableReleaseTimes(numberOfRunnables);

    for (auto &task : dag->GetTasks()) {
        int maxCycle = static_cast<int>(hyperPeriod / task->GetPeriod());
        double period = task->GetPeriod();
        double offset = task->GetOffset();

        for (auto &runnable : task->GetRunnables()) {
            int runnableId = runnable->GetId();
            std::vector<ExecutionInformation>(maxCycle, initialExecutionInformation).swap(runnableReleaseTimes[runnableId]);

            for (int cycle = 0; cycle < maxCycle; cycle++) {
                runnableReleaseTimes[runnableId][cycle].startTime = period * cycle + offset;
                runnableReleaseTimes[runnableId][cycle].endTime = period * (cycle + 1) + offset;
            }
        }
    }

    // Set Execution Times
    double unit = dag->GetTask(0)->GetPeriod();
    for (auto &task : dag->GetTasks()) {
        unit = static_cast<double>(std::gcd(static_cast<int>(unit * 100000), ((task->GetOffset() != 0.0) ? std::gcd(static_cast<int>(task->GetPeriod() * 100000), static_cast<int>(task->GetOffset() * 100000)) : static_cast<int>(task->GetPeriod() * 100000))) / 100000);
    }

    std::vector<double> emptyTimes((static_cast<int>(hyperPeriod / unit)), unit);

    runnableCommunications.resize(numberOfRunnables);
    for (auto &oneCaseSequence : allCasePerPriority) {
        for (auto &runnable : oneCaseSequence) {
            int runnableId = runnable->GetId();
            int maxCycle = static_cast<int>(runnableReleaseTimes[runnableId].size());
            runnableCommunications[runnableId].emplace_back(runnableReleaseTimes[runnableId]);

            for (int cycle = 0; cycle < maxCycle; cycle++) {
                int unitIndex = static_cast<int>(std::floor(runnableCommunications[runnableId].back()[cycle].startTime / unit));

                // Regard time-line
                while (emptyTimes[(unitIndex)] == 0.0) unitIndex++;

                // Set start time
                runnableCommunications[runnableId].back()[cycle].startTime = (static_cast<double>(unitIndex) * unit) + (1 - emptyTimes[unitIndex]);

                // Set end time
                double executionTime = runnable->GetExecutionTime();

                while (executionTime) {
                    if (emptyTimes[unitIndex] < executionTime) {
                        executionTime -= emptyTimes[unitIndex];
                        emptyTimes[unitIndex] = 0.0;

                        unitIndex++;
                    } else {
                        runnableCommunications[runnableId].back()[cycle].endTime = (static_cast<double>(unitIndex) * unit) + executionTime;
                        emptyTimes[unitIndex] -= executionTime;
                        executionTime = 0.0;
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

void TaskImplicit::GetCommunicationTable(std::shared_ptr<DAG>& dag, int numberOfRunnables, double hyperPeriod, std::vector<std::vector<std::vector<int>>>& runnablePermutation, std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableCommunications) {
    runnablePermutation.reserve(numberOfRunnables);

    double unit = dag->GetTask(0)->GetPeriod();
    for (auto &task : dag->GetTasks()) {
        unit = static_cast<double>(std::gcd(static_cast<int>(unit * 100000), ((task->GetOffset() != 0.0) ? std::gcd(static_cast<int>(task->GetPeriod() * 100000), static_cast<int>(task->GetOffset() * 100000)) : static_cast<int>(task->GetPeriod() * 100000))) / 100000);
    }

    std::vector<double> emptyTimes((static_cast<int>(hyperPeriod / unit)), unit);

    ExecutionInformation initialExecutionInformation = {-1.0, -1.0};
    runnableCommunications.resize(numberOfRunnables);

    for (auto &task : dag->GetTasksPriority()) {
        int maxCycle = static_cast<int>(hyperPeriod / task->GetPeriod());
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
            }

            taskExecutionInformation[cycle].startTime = startTime;
            taskExecutionInformation[cycle].endTime = endTime;
        }

        for (auto &runnable : task->GetRunnables()) {
            int runnableId = runnable->GetId();

            runnablePermutation.emplace_back(std::vector<std::vector<int>>(1, std::vector<int>(1, runnableId)));

            runnableCommunications[runnableId].emplace_back(taskExecutionInformation);
        }
    }
}

void LET::GetCommunicationTable(std::shared_ptr<DAG>& dag, int numberOfRunnables, double hyperPeriod, std::vector<std::vector<std::vector<int>>>& runnablePermutation, std::vector<std::vector<std::vector<ExecutionInformation>>>& runnableCommunications) {
    runnablePermutation.reserve(numberOfRunnables);

    ExecutionInformation initialExecutionInformation = {-1.0, -1.0};
    runnableCommunications.resize(numberOfRunnables);

    for (auto &task : dag->GetTasksPriority()) {
        int maxCycle = static_cast<int>(hyperPeriod / task->GetPeriod());
        double period = task->GetPeriod();
        double offset = task->GetOffset();

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