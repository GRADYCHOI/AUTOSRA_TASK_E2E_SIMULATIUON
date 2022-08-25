#include "communication.hpp"


class TaskImplicit : public Communication {
public:
    TaskImplicit();
    ~TaskImplicit();

    void GetCommunicationTable(RunnableInformation* runnableInformations, ExecutionInformation* runnableExecutions, int numberOfRunnables, int maxCycle, CommunicationInformation* runnableCommunications) {
        std::vector<std::pair<int, int>> runnablePriority(numberOfRunnables);
        std::vector<int> sameTaskMappedRunnables;
        CommunicationInformation* tmpInformation = new CommunicationInformation[maxCycle];

        for (int runnableId = 0; runnableId < numberOfRunnables; runnableId++) {
            runnablePriority[runnableId] = std::make_pair(runnableId, runnableInformations[runnableId].priority);
        }

        std::sort(runnablePriority.begin(), runnablePriority.end(), [](std::pair<int, int> a, std::pair<int, int> b) { return a.second < b.second; });

        int taskId = 0;
        int preRunnableId = 0;
        memcpy(tmpInformation, runnableExecutions + (runnablePriority[0].first * maxCycle), sizeof(ExecutionInformation) * maxCycle);

        for (auto &runnableId : runnablePriority) {
            if (taskId != runnableInformations[runnableId.first].taskId) {
                // Set Runnable Write Time
                for (int cycle = 0; cycle < maxCycle; cycle++) {
                    tmpInformation[cycle].writeTime = runnableExecutions[preRunnableId * maxCycle + cycle].endTime;
                }

                // Save R/W Time
                for (auto &index : sameTaskMappedRunnables) {
                    memcpy(runnableCommunications + (index * maxCycle), tmpInformation, sizeof(CommunicationInformation) * maxCycle);
                }
                sameTaskMappedRunnables.clear();

                // Set Runnable Read Time
                memcpy(tmpInformation, runnableExecutions + (runnableId.first * maxCycle), sizeof(ExecutionInformation) * maxCycle);

                taskId = runnableInformations[runnableId.first].taskId;
            } else {
                sameTaskMappedRunnables.push_back(runnableId.first);
            }

            preRunnableId = runnableId.first;
        }

        // Memcpy last Task's Runnables
        for (int cycle = 0; cycle < maxCycle; cycle++) {
            tmpInformation[cycle].writeTime = runnableExecutions[preRunnableId * maxCycle + cycle].endTime;
        }
        for (auto &index : sameTaskMappedRunnables) {
            memcpy(runnableCommunications + (index * maxCycle), tmpInformation, sizeof(CommunicationInformation) * maxCycle);
        }

        delete[] tmpInformation;
    }
};