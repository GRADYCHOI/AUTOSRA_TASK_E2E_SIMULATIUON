#include "communication.hpp"


void RunnableImplicit::GetCommunicationTable(std::vector<RunnableInformation>& runnableInformations, std::vector<std::vector<ExecutionInformation>>& runnableExecutions, int numberOfRunnables, int maxCycle, std::vector<std::vector<ExecutionInformation>>& runnableCommunications) {
    std::copy(runnableExecutions.begin(), runnableExecutions.end(), runnableCommunications.begin());
}

void TaskImplicit::GetCommunicationTable(std::vector<RunnableInformation>& runnableInformations, std::vector<std::vector<ExecutionInformation>>& runnableExecutions, int numberOfRunnables, int maxCycle, std::vector<std::vector<ExecutionInformation>>& runnableCommunications) {
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

void LET::GetCommunicationTable(std::vector<RunnableInformation>& runnableInformations, std::vector<std::vector<ExecutionInformation>>& runnableExecutions, int numberOfRunnables, int maxCycle, std::vector<std::vector<ExecutionInformation>>& runnableCommunications) {
    for (int runnableId = 0; runnableId < numberOfRunnables; runnableId++) {
        for (int cycle = 0; cycle < maxCycle; cycle++) {
            runnableCommunications[runnableId][cycle].startTime = runnableInformations[runnableId].period * cycle + runnableInformations[runnableId].offset;
            runnableCommunications[runnableId][cycle].endTime = runnableInformations[runnableId].period * (cycle + 1) + runnableInformations[runnableId].offset;
        }
    }
}