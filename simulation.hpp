#ifndef __SIMULATION_HPP__

#define __SIMULATION_HPP__

#include "DAG.hpp"
#include <algorithm>
#include <vector>
#include <memory>
#include <cmath>
#include <limits.h>
#include <cstring>


void GetTaskInfo(double* periods, double* offsets, double* executions);
void GetRunnableInfo(double* periods, double* offsets, double* executions);

void GetExecutionTable(double* periods, double* offsets, double* executions, int size, int maxCycle, double* startTable, double* endTable);

void GetReadTable(double* startTable, int size, int maxCycle, double* readTable);
void GetReleaseTimeReadTable(double* periods, double* offsets, int size, int maxCycle, double* readTable);

void GetWriteTable(double* endTable, int size, int maxCycle, double* writeTable);
void GetReleaseTimeWriteTable(double* periods, double* offsets, int size, int maxCycle, double* writeTable);
void GetNextJobWriteTable(double* startTable, int size, int maxCycle, double* writeTable);

void SetArrivalTable(double* readTable, double* writeTable, int inputRunnableIndex, int inputCycle, int hyperPeriodCount, int thisRunnableId, int thisCycle, int maxCycle, double* arrivalTable);
void GetArrivalTable(double* readTable, double* writeTable, int maxCycle, double* arrivalTable);

void GetReactionTime(double* arrivalTable, double* readTable, int maxCycle, double* reactionTime);
void GetDataAge(double* arrivalTable, double* writeTable, int maxCycle, double* dataAge);

#endif