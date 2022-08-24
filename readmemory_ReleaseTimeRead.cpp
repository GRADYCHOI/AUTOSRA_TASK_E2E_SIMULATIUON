#include "readmemory.hpp"
#include <memory>
#include <vector>


class StartOfRunnableRead : public ReadMemory {
public:
    GetReadTable(double* runnableInformations, double* startTable, double* endTable, int numberOfRunnables, int maxCycle, double* readTable) {
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

        std::vector<std::pair<int, int>> runnablePriority(numberOfRunnables);
        int taskIndex = -1;
        double* readTime = new double[maxCycle];

        for (int runnableIndex = 0; runnableIndex < numberOfRunnables; runnableIndex++) {
            runnablePriority[runnableIndex] = std::make_pair(runnableIndex, (int)runnableInformations[runnableIndex * 5 + 1]);
        }

        std::sort(runnablePriority.begin(), runnablePriority.end(), [](std::pair<int, int> a, std::pair<int, int> b) { return a.second < b.second; });

        for (auto &runnableId : runnablePriority) {
            if (taskIndex != runnableInformations[(runnableId.first) * 5]) {
                taskIndex = runnableInformations[(runnableId.first) * 5];

                for (int cycle = 0; cycle < maxCycle; cycle++) {
                    // Period * Cycle + Offset
                    readTime[cycle] = runnableInformations[(runnableId.first) * 5 + 2] * cycle + runnableInformations[(runnableId.first) * 5 + 3];
                }
            }

            memcpy(readTable + (runnableId.first * maxCycle), readTime, sizeof(double) * maxCycle);
        }

        delete[] readTime;
    }
};