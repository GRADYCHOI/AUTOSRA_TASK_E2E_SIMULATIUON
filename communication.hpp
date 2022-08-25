#ifndef __COMMUNICATION__HPP__

#define __COMMUNICATION__HPP__

#include "DAG.hpp"
#include "readmemory.hpp"
#include "writememory.hpp"


class Communication {
public:
    ReadMemory * readMemory;
    WriteMemory * writeMemory;

    Communication();
    virtual ~Communication();

    void GetReadTable(double* runnableInformations, double* startTable, double* endTable, int numberOfRunnables, int maxCycle, double* readTable) { 
        this->readMemory.GetReadTable(runnableInformations, startTable, endTable, numberOfRunnables, maxCycle, readTable);
    }
    void GetWriteTable(double* runnableInformations, double* startTable, double* endTable, int numberOfRunnables, int maxCycle, double* writeTable) {
        this->writeMemory.GetWriteTable(runnableInformations, startTable, endTable, numberOfRunnables, maxCycle, writeTable);
    }

    void GetCommunicationTable(double* runnableInformations, double* startTable, double* endTable, int numberOfRunnables, int maxCycle, double* readTable, double* writeTable) {
        this->GetReadTable(runnableInformations, startTable, endTable, numberOfRunnables, maxCycle, readTable);
        this->GetWriteTable(runnableInformations, startTable, endTable, numberOfRunnables, maxCycle, writeTable);
    }

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
};

#endif