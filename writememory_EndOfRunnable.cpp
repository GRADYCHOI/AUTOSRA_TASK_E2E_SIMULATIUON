#include "writememory.hpp"
#include <memory>


class StartOfRunnableWrite : public WriteMemory {
public:
    void GetWriteTable(double* runnableInformations, double* startTable, double* endTable, int numberOfRunnables, int maxCycle, double* writeTable) = 0;
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

        memcpy(writeTable, endTable, sizeof(double) * numberOfRunnables * maxCycle);
};