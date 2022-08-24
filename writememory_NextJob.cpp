#include "writememory.hpp"
#include <memory>
#include <vector>


class NextJobWrite : public WriteMemory {
public:
    void GetWriteTable(double* runnableInformations, double* startTable, double* endTable, int numberOfRunnables, int maxCycle, double* writeTable) {
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
        // writeTable : [maxCycle X numberOfRunnables]     Output
        // --------------------------------------------------------------------------------------------------------------
        // ## The order of Runnable is based on their IDs
        // 1 : First Cycle
        // 2 : Second Cycle
        // ..
        // --------------------------------------------------------------------------------------------------------------

        std::vector<std::pair<int, int>> runnablePriority(numberOfRunnables);
        int taskIndex = -1;
        double* writeTime = new double[maxCycle];

        for (int runnableIndex = 0; runnableIndex < numberOfRunnables; runnableIndex++) {
            runnablePriority[runnableIndex] = std::make_pair(runnableIndex, (int)runnableInformations[runnableIndex * 5 + 1]);
        }

        std::sort(runnablePriority.begin(), runnablePriority.end(), [](std::pair<int, int> a, std::pair<int, int> b) { return a.second < b.second; });

        double hyperPeriod = runnableInformations[runnablePriority[0].first * 5 + 2] * maxCycle;

        for (auto &runnableId : runnablePriority) {
            if (taskIndex != runnableInformations[(runnableId.first) * 5]) {
                taskIndex = runnableInformations[(runnableId.first) * 5];
                memcpy(writeTime, startTable + (runnableId.first * maxCycle + 1), sizeof(double) * maxCycle - 1);
                writeTime[maxCycle] = startTable[runnableId.first * maxCycle] + hyperPeriod;
            }

            memcpy(writeTable + (runnableId.first * maxCycle), writeTime, sizeof(double) * maxCycle);
        }

        delete[] writeTime;
    }
};