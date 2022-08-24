#include "communication.hpp"


class LET : public Communication {
public:
    LET() { this->readMemory = new ReleaseTimeRead(); this->writeMemory = new ReleaseTimeWrite(); }

    void GetCommunicationTable(double* runnableInformations, double* startTable, double* endTable, int numberOfRunnables, int maxCycle, double* readTable, double* writeTable) {
        // --------------------------------------------------------------------------------------------------------------
        // runnableInformations : [5 X numberOfRunnables]     Input
        // --------------------------------------------------------------------------------------------------------------
        // ## The order of Runnable is based on their IDs
        // 1 : (Mapped) Task ID
        // 2 : Priority
        // 3 : Period
        // 4 : Offset
        // 5 : Execution Time
        // --------------------------------------------------------------------------------------------------------------
        // startTable : [maxCycle X numberOfRunnables]     Input
        // --------------------------------------------------------------------------------------------------------------
        // ## The order of Runnable is based on their IDs
        // 1 : First Cycle
        // 2 : Second Cycle
        // ..
        // --------------------------------------------------------------------------------------------------------------
        // endTable : [maxCycle X numberOfRunnables]     Input
        // --------------------------------------------------------------------------------------------------------------
        // ## The order of Runnable is based on their IDs
        // 1 : First Cycle
        // 2 : Second Cycle
        // ..
        // --------------------------------------------------------------------------------------------------------------
        // readTable : [maxCycle X numberOfRunnables]     Output
        // --------------------------------------------------------------------------------------------------------------
        // ## The order of Runnable is based on their IDs
        // 1 : First Cycle
        // 2 : Second Cycle
        // ..
        // --------------------------------------------------------------------------------------------------------------
        // writeTable : [maxCycle X numberOfRunnables]     Output
        // --------------------------------------------------------------------------------------------------------------
        // ## The order of Runnable is based on their IDs
        // 1 : First Cycle
        // 2 : Second Cycle
        // ..
        // --------------------------------------------------------------------------------------------------------------

        std::vector<std::pair<int, int>> runnablePriority(numberOfRunnables);
        int taskIndex = -1;
        double* taskStartTime = new double[maxCycle];
        double* taskEndTime = new double[maxCycle];

        for (int runnableIndex = 0; runnableIndex < numberOfRunnables; runnableIndex++) {
            runnablePriority[runnableIndex] = std::make_pair(runnableIndex, (int)runnableInformations[runnableIndex * 5 + 1]);
        }

        std::sort(runnablePriority.begin(), runnablePriority.end(), [](std::pair<int, int> a, std::pair<int, int> b) { return a.second < b.second; });

        for (auto &runnableId : runnablePriority) {
            if (taskIndex != runnableInformations[(runnableId.first) * 5]) {
                taskIndex = runnableInformations[(runnableId.first) * 5];

                for (int cycle = 0; cycle < maxCycle; cycle++) {
                    taskStartTime[cycle] = runnableInformations[(runnableId.first) * 5 + 2] * cycle + runnableInformations[(runnableId.first) * 5 + 3];
                    taskEndTime[cycle] = runnableInformations[(runnableId.first) * 5 + 2] * (cycle + 1) + runnableInformations[(runnableId.first) * 5 + 3];
                }
            }

            memcpy(readTable + (runnableId.first * maxCycle), taskStartTime, sizeof(double) * maxCycle);
            memcpy(writeTable + (runnableId.first * maxCycle), taskEndTime, sizeof(double) * maxCycle);
        }

        delete[] taskEndTime;
        delete[] taskStartTime;
    }
};