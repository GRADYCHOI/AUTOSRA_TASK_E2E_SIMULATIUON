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
    std::vector<std::pair<int, int>> runnablePriority(numberOfRunnables);
    std::vector<int> sameTaskMappedRunnables;
    std::vector<ExecutionInformation> tmpInformation;

    for (int runnableId = 0; runnableId < numberOfRunnables; runnableId++) {
        runnablePriority[runnableId] = std::make_pair(runnableId, runnableInformations[runnableId].priority);
    }

    std::sort(runnablePriority.begin(), runnablePriority.end(), [](std::pair<int, int> a, std::pair<int, int> b) { return a.second < b.second; });

    int taskId = runnableInformations[runnablePriority[0].first].taskId;
    int preRunnableId = runnablePriority[0].first;

    // First Runnable's ReadTime
    std::copy(runnableExecutions[runnablePriority[0].first].begin(), runnableExecutions[runnablePriority[0].first].end(), tmpInformation.begin());

    for (auto &runnableId : runnablePriority) {
        if (taskId != runnableInformations[runnableId.first].taskId) {
            // Set Runnable Write Time
            for (int cycle = 0; cycle < maxCycle; cycle++) {
                tmpInformation[cycle].endTime = runnableExecutions[preRunnableId][cycle].endTime;
            }

            // Save R/W Time
            for (auto &index : sameTaskMappedRunnables) {
                std::copy(tmpInformation.begin(), tmpInformation.end(), runnableCommunications[index].begin());
            }
            sameTaskMappedRunnables.clear();

            // Set Runnable Read Time
            std::copy(runnableExecutions[runnableId.first].begin(), runnableExecutions[runnableId.first].end(), tmpInformation.begin());

            taskId = runnableInformations[runnableId.first].taskId;
        } else {
            sameTaskMappedRunnables.push_back(runnableId.first);
        }

        preRunnableId = runnableId.first;
    }

    // Memcpy last Task's Runnables
    for (int cycle = 0; cycle < maxCycle; cycle++) {
        tmpInformation[cycle].endTime = runnableExecutions[preRunnableId][cycle].endTime;
    }
    for (auto &index : sameTaskMappedRunnables) {
        std::copy(tmpInformation.begin(), tmpInformation.end(), runnableCommunications[index].begin());
    }
}

void LET::GetCommunicationTable(Simulation* simulationSelf, std::vector<std::vector<ExecutionInformation>>& runnableCommunications) {
    for (int runnableId = 0; runnableId < numberOfRunnables; runnableId++) {
        for (int cycle = 0; cycle < maxCycle; cycle++) {
            runnableCommunications[runnableId][cycle].startTime = runnableInformations[runnableId].period * cycle + runnableInformations[runnableId].offset;
            runnableCommunications[runnableId][cycle].endTime = runnableInformations[runnableId].period * (cycle + 1) + runnableInformations[runnableId].offset;
        }
    }
}