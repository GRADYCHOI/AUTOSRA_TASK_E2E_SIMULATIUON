#include "DAG.hpp"
#include <iostream>

using namespace std;

/* C++ 17이 안되면 쓰기
int lcm(int a,int b) {
    int i,j;
    int temp; //공배수를 저장할 변수
    j = (a>b)?a:b; //둘중 큰수를 구함
    for(i=j;;i++) {
        if(i%a==0 && i%b==0){
            temp = i;
            break;
        }
    }
    return temp;
}
*/

DAG::DAG() {}

DAG::~DAG() {
    inputRunnables.clear();
    outputRunnabels.clear();
    tasks.clear();
    runnables.clear();
}

void DAG::SetHyperPeriod() {
    double tmpPeriod = 0.0;
    int numberOfTasks = this->GetNumberOfTasks();

    for(int tmpCount = 0; tmpCount < numberOfTasks; tmpCount++) {
        tmpPeriod = std::lcm(tmpPeriod, this->task[tmpCount]->GetPeriod());
    }

    this->hyperPeriod = tmpPeriod;
}

void DAG::AddRunnablePtr(const std::shared_ptr<RUNNABLE>& runnable) {
    this->runnables.push_back(runnable);
}

void DAG::GenerateRunnables(int num, int in, int out) {
    int status = -1;
    for (int i = 1; i <= num; i++) {
        shared_ptr<RUNNABLE> runnalbe(new RUNNABLE(i, (double)(rand() % 100) / 1000));
        AddRunnablePtr(runnalbe);
    }
}
void DAG::RandomEdge() { //Runnable edge random generation

}

void DAG::DisplayRunnablesPtr(){
    cout << runnables.size() << " " << runnables.capacity() << endl;
    for (const auto &runnable : runnables) {
        cout << runnable->GetExecutionTime()  << " " << runnable->GetId() << endl;
    }
}

int DAG::GetNumberOfTasks() {
    return this->tasks.size();
}

int DAG::GetNumberOfRunnables() {
    return this->runnnables.size();
}

double DAG::GetHyperPeriod() {
    return this->hyperPeriod;
}

void DAG::SetInputRunnableList() {
    // vector capacity control : swap technique
    std::vector<std::shared_ptr<RUNNABLE>> tmpList;
    int numberOfRunnables = this->GetNumberOfRunnables();

    for (int runnableIndex = 0; runnableIndex < numberOfRunnables; runnableIndex++) {
        if (this->runnables[runnableIndex]->GetStatus() == 0) {
            tmpList.push_back(this->runnables[runnableIndex]->GetSharedPtr());
        }
    }

    tmpList.swap(this->inputRunnables);
}

void DAG::SetOutputRunnableList() {
    std::vector<std::shared_ptr<RUNNABLE>> tmpList;
    int numberOfRunnables = this->GetNumberOfRunnables();

    for (int runnableIndex = 0; runnableIndex < numberOfRunnables; runnableIndex++) {
        if (this->runnables[runnableIndex]->GetStatus() == 1) {
            tmpList.push_back(this->runnables[runnableIndex]->GetSharedPtr());
        }
    }

    tmpList.swap(this->outputRunnables);
}

int DAG::GetNumberOfInputRunnables() {
    return this->intputRunnables.size();
}

int DAG::GetNumberOfOutputRunnables() {
    return this->outputRunnables.size();
}


/*
void DAG::AddTask(Task& task) {
    this->task.push_back(task);

    this->SetParamters();
}*/

void DAG::Simulate() {
    std::cin >> option >> std::endl;

    this->GetPriorityMap();

    switch (option) {
        case IMPLICIT_RUNNABLE:
            break;
        case IMPLICIT_TASK:
            break;
        default:
            std::cout << "Unknown Communication" << std::endl;
            break;
    }
    
}

void DAG::SimulateImplicitRunnable() {
}

int DAG::GetMaxCycle() {
    int minPeriod = INT_MAX;
    int numberOfTasks = this->GetNumberOfTasks();

    for (int taskIndex = 0; taskIndex < numberOfTasks; taskIndex++) {
        if (minPeriod > this->tasks[taskIndex]->GetPeriod()) {
            minPeriod = this->tasks[taskIndex]->GetPeriod();
        } 
    }

    return static_cast<int>(this->GetHyperPeriod() / minPeriod);
}

void DAG::SimulateImplicitTask() {
    int maxCycle = this->GetMaxCycle(); 
    int taskSize = this->GetNumberOfTasks();
    int runnableSize = this->GetNumberOfRunnables();

    int* runnablePriority = new int[runnableSize];
    double* taskPeriods = new double[taskSize];
    double* taskOffsets = new double[taskSize];
    double* taskExecutionTimes = new double[taskSize];
    double* taskReleaseTable = new double[taskSize * (maxCycle + 1)];
    double* taskStartTable = new double[taskSize * (maxCycle + 1)];
    double* taskEndTable = new double[taskSize * (maxCycle + 1)];
    double* taskReactionTime = new double[this->GetNumberOfOutputRunnables() * maxCycle * this->GetNumberOfInputRunnables()];
    double* taskDataAge = new double[this->GetNumberOfOutputRunnables() * maxCycle * this->GetNumberOfInputRunnables()];

    std::memset(runnablePriority, -1, sizeof(int) * runnableSize);
    std::memset(taskPeriods, -1.0, sizeof(double) * taskSize);
    std::memset(taskOffsets, -1.0, sizeof(double) * taskSize);
    std::memset(taskExecutionTimes, -1.0, sizeof(double) * taskSize);
    std::memset(taskReleaseTable, -1.0, sizeof(double) * taskSize * maxCycle);
    std::memset(taskStartTable, -1.0, sizeof(double) * taskSize * maxCycle);
    std::memset(taskEndTable, -1.0, sizeof(double) * taskSize * maxCycle);
    std::memset(taskReactionTime, -1.0, sizeof(double) * maxCycle);
    std::memset(taskDataAge, -1.0, sizeof(double) * maxCycle);

    // command set
    this->GetTaskInfo(taskSize, taskPeriods, taskOffsets, taskExecutionTimes);
    this->GetExecutionTable(taskPeriods, taskOffsets, taskExecutionTimes, taskSize, maxCycle, taskStartTable, taskEndTable);
    this->GetReactionTime(taskStartTable, taskEndTable, taskSize, maxCycle, taskReactionTime);
    this->GetDataAge(taskStartTable, taskEndTable, taskSize, maxCycle, taskDataAge);

    delete[] taskDataAge;
    delete[] taskReactionTime;
    delete[] taskEndTable;
    delete[] taskStartTable;
    delete[] taskReleaseTable;
    delete[] taskExecutionTimes;
    delete[] taskOffsets;
    delete[] taskPeriods;
}

void DAG::GetTaskPeriods(double* periods) {
    // --------------------------------------------------------------------------------------------------------------
    // periods : [2 X Task size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : Task ID
    // 2 : Task Period
    // --------------------------------------------------------------------------------------------------------------

    int numberOfTasks = this->GetNumberOfTasks();

    for (int taskIndex = 0; taskIndex < numberOfTasks; taskIndex++) {
        periods[this->taskPriority[taskIndex] * 2] = this->tasks[taskIndex]->GetId();
        periods[this->taskPriority[taskIndex] * 2 + 1] = this->tasks[taskIndex]->GetPeriod();
    }
}

void DAG::GetTaskOffsets(double* offsets) {
    // --------------------------------------------------------------------------------------------------------------
    // offsets : [2 X Task size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : Task ID
    // 2 : Task Offset
    // --------------------------------------------------------------------------------------------------------------

    int numberOfTasks = this->GetNumberOfTasks();

    for (int taskIndex = 0; taskIndex < numberOfTasks; taskIndex++) {
        offsets[this->taskPriority[taskIndex] * 2] = this->tasks[taskIndex]->GetId();
        offsets[this->taskPriority[taskIndex] * 2 + 1] = this->tasks[taskIndex]->GetOffset();
    }
}

void DAG::GetTaskExecutionTimes(double* executions) {
    // --------------------------------------------------------------------------------------------------------------
    // executions : [2 X Task size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : Task ID
    // 2 : Task Execution Time
    // --------------------------------------------------------------------------------------------------------------

    int numberOfTasks = this->GetNumberOfTasks();

    for (int taskIndex = 0; taskIndex < numberOfTasks; taskIndex++) {
        executions[this->taskPriority[taskIndex] * 2] = this->tasks[taskIndex]->GetId();
        executions[this->taskPriority[taskIndex] * 2 + 1] = 0;

        int numberOfRunnables = this->tasks[taskIndex]->GetNumberOfRunnables();

        for (int tmpCount = 0; tmpCount < numberOfRunnables; tmpCount++) {
            executions[this->taskPriority[taskIndex] * 2 + 1] += this->tasks[taskIndex]->runnables[tmpCount]->GetExecutionTime();
        }
    }
}

void DAG::GetRunnablePeriods(double* periods) {
    // --------------------------------------------------------------------------------------------------------------
    // periods : [2 X Runnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : Runnable ID
    // 2 : Runnable Period
    // --------------------------------------------------------------------------------------------------------------

    int numberOfTasks = this->GetNumberOfTasks();
    
    for (int taskIndex = 0; taskIndex < numberOfTasks; taskIndex++) {
        int numberOfRunnables = this->tasks[taskIndex]->GetNumberOfRunnables();

        for (int tmpCount = 0; tmpCount < numberOfRunnables; tmpCount++) {
            periods[this->runnablePriority[this->tasks[taskIndex]->GetRunnable(tmpCount)->GetId()] * 2] = this->tasks[taskIndex]->GetRunnable(tmpCount)->GetId();
            periods[this->runnablePriority[this->tasks[taskIndex]->GetRunnable(tmpCount)->GetId()] * 2 + 1] = this->tasks[taskIndex]->runnables[tmpCount]->GetPeriod();
        }
    }
}

void DAG::GetRunnableOffsets(double* offsets) {
    // --------------------------------------------------------------------------------------------------------------
    // offsets : [2 X Runnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : Runnable ID
    // 2 : Runnable Offset
    // --------------------------------------------------------------------------------------------------------------

    int numberOfTasks = this->GetNumberOfTasks();
    
    for (int taskIndex = 0; taskIndex < numberOfTasks; taskIndex++) {
        int numberOfRunnables = this->tasks[taskIndex]->GetNumberOfRunnables();  

        for (int tmpCount = 0; tmpCount < numberOfRunnables; tmpCount++) {
            offsets[this->runnablePriority[this->tasks[taskIndex]->runnables[tmpCount]->GetId()] * 2] = this->tasks[taskIndex]->runnables[tmpCount]->GetId();
            offsets[this->runnablePriority[this->tasks[taskIndex]->runnables[tmpCount]->GetId()] * 2 + 1] = this->tasks[taskIndex]->runnables[tmpCount]->GetOffset();
        }
    }
}

void DAG::GetRunnableExecutionTimes(double* executions) {
    // --------------------------------------------------------------------------------------------------------------
    // executions : [2 X Runnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : Runnable ID
    // 2 : Runnable Execution Time
    // --------------------------------------------------------------------------------------------------------------

    int numberOfRunnables = this->GetNumberOfRunnables();

    for (int runnableIndex = 0; runnableIndex < numberOfRunnables; runnableIndex++) {
        executions[this->runnablePriority[runnableIndex] * 2] = this->runnables[runnableIndex]->GetId();
        executions[this->runnablePriority[runnableIndex] * 2 + 1] = this->runnables[runnableIndex]->GetExecutionTime();
    }
}

void DAG::GetTaskInfo(double* periods, double* offsets, double* executions) {
    // --------------------------------------------------------------------------------------------------------------
    // periods : [2 X Runnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : Runnable ID
    // 2 : Runnable Period
    // --------------------------------------------------------------------------------------------------------------
    // offsets : [2 X Runnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : Runnable ID
    // 2 : Runnable Offset
    // --------------------------------------------------------------------------------------------------------------
    // executions : [2 X Runnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : Runnable ID
    // 2 : Runnable Execution Time
    // --------------------------------------------------------------------------------------------------------------

    this->GetTaskPeriods(periods);
    this->GetTaskOffsets(offsets);
    this->GetTaskExecutionTimes(executions);
}

void DAG::GetRunnableInfo(double* periods, double* offsets, double* executions) {
    // --------------------------------------------------------------------------------------------------------------
    // periods : [2 X Runnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : Runnable ID
    // 2 : Runnable Period
    // --------------------------------------------------------------------------------------------------------------
    // offsets : [2 X Runnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : Runnable ID
    // 2 : Runnable Offset
    // --------------------------------------------------------------------------------------------------------------
    // executions : [2 X Runnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : Runnable ID
    // 2 : Runnable Execution Time
    // --------------------------------------------------------------------------------------------------------------

    this->GetRunnablePeriods(periods);
    this->GetRunnableOffsets(offsets);
    this->GetRunnableExecutionTimes(executions);
}


// period와 offset이 1ms 단위인 경우만 한정
void DAG::GetExecutionTable(double* periods, double* offsets, double* executions, int size, int maxCycle, double* startTable, double* endTable) {
    // --------------------------------------------------------------------------------------------------------------
    // periods : [2 X (Runnable or Task) size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : (Runnable or Task) ID
    // 2 : (Runnable or Task) Period
    // --------------------------------------------------------------------------------------------------------------
    // offsets : [2 X (Runnable or Task) size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : (Runnable or Task) ID
    // 2 : (Runnable or Task) Offset
    // --------------------------------------------------------------------------------------------------------------
    // executions : [2 X (Runnable or Task) size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : (Runnable or Task) ID
    // 2 : (Runnable or Task) Execution Time
    // --------------------------------------------------------------------------------------------------------------
    // startTable : [(maxCycle + 1) X (Runnable or Task) size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : (Runnable or Task) ID
    // 2 : First Cycle
    // 3 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------
    // endTable : [(maxCycle + 1) X (Runnable or Task) size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : (Runnable or Task) ID
    // 2 : First Cycle
    // 3 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------

    double* emptyTimes = new double[this->GetHyperPeriod()];
    std::memset(emptyTimes, 1.0, sizeof(double) * this->GetHyperPeriod());

    for (int index = 0; index < size; index++) {
        double period = periods[index * 2 + 1];
        double offset = offsets[index * 2 + 1];
        double execution = executions[index * 2 + 1];

        int eachMaxCycle = static_cast<int>(this->GetHyperPeriod() / period);
        startTable[index * maxCycle] = periods[index * 2];

        for (int cycle = 0; cycle < eachMaxCycle; cycle++) {
            double releaseTime = period * cycle + offset;
            double deadTime = period * (cycle + 1) + offset;

            int integerReleaseTime = static_cast<int>(std::floor(releaseTime));
            int integerDeadTime = static_cast<int>(std::floor(deadTime));

            // Regard time-line
            while !(emptyTimes[integerReleaseTime]) integerReleaseTime++;

            // Set start table
            startTable[index * maxCycle + cycle + 1] = static_cast<double>(integerReleaseTime) + 1 - emptyTimes[integerReleaseTime];

            // Set end table
            while (execution) {
                if (emptyTimes[integerReleaseTime] < execution) {
                    integerReleaseTime += 1;
                    execution -= emptyTimes[integerReleaseTime];
                    emptyTimes[integerReleaseTime] = 0.0;
                } else {
                    endTable[index * maxCycle + cycle + 1] = releaseTime + execution;
                    emptyTimes[integerReleaseTime] -= execution;
                    execution = 0;
                }
            }
        }
    }

    delete[] emptyTimes;
}

/* TODO : period와 offset이 double형 값인 경우에 대응되는 방법 고안

방법1 : vector의 연결리스트로 결과값 서로 잇기

void DAG::GetExecutionTable(double* periods, double* offsets, double* executions, int size, int maxCycle, double* startTable, double* endTable) {

}

*/

void DAG::GetReadTable(double* startTable, int size, int maxCycle, double* readTable) {
    // --------------------------------------------------------------------------------------------------------------
    // startTable : [(maxCycle + 1) X (Runnable or Task) size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : (Runnable or Task) ID
    // 2 : First Cycle
    // 3 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------
    // readTable : [(maxCycle + 1) X Runnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their IDs
    // 1 : Runnable ID
    // 2 : First Cycle
    // 3 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------

    // 직관적인 방법이 있지만, 실행시간을 줄이기 위해 아래 방법 채택
    if (size == this->GetNumberOfRunnables()) {
        for (int runnableIndex = 0; runnableIndex < size; runnableIndex++) {
            std::memcpy(readTable[startTable[runnableIndex * maxCycle] * maxCycle], startTable[runnableIndex * maxCycle], sizeof(double) * (maxCycle + 1));
        }

    } else if (size == this->GetNumberOfTasks()) {
        for (int taskIndex = 0; taskIndex < size; taskIndex++) {
            int numberOfRunnables = this->tasks[taskIndex]->GetNumberOfRunnables();

            for (int tmpCount = 0; tmpCount < numberOfRunnables; tmpCount++) {
                readTable[this->tasks[taskIndex]->GetRunnable(tmpCount)->GetId() * maxCycle] = this->tasks[taskIndex]->GetRunnable(tmpCount)->GetId();
                std::memcpy(readTable[this->tasks[taskIndex]->GetRunnable(tmpCount)->GetId() * maxCycle + 1], startTable[this->taskPriority[taskIndex] * maxCycle + 1], sizeof(double) * maxCycle);
            }
        }
    }
}

// TASK 기준에서만 가능
void DAG::GetReleaseTimeReadTable(double* periods, double* offsets, int size, int maxCycle, double* readTable) {
    // --------------------------------------------------------------------------------------------------------------
    // periods : [2 X (Runnable or Task) size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : (Runnable or Task) ID
    // 2 : (Runnable or Task) Period
    // --------------------------------------------------------------------------------------------------------------
    // offsets : [2 X (Runnable or Task) size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : (Runnable or Task) ID
    // 2 : (Runnable or Task) Offset
    // --------------------------------------------------------------------------------------------------------------
    // readTable : [(maxCycle + 1) X (Runnable or Task) size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their IDs
    // 1 : (Runnable or Task) ID
    // 2 : First Cycle
    // 3 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------

    for (int taskIndex = 0; taskIndex < size; taskIndex++) {
        int numberOfRunnables = this->tasks[taskPriority[taskIndex]]->GetNumberOfRunnables();

        for (int tmpCount = 0; tmpCount < numberOfRunnables; tmpCount++) {
            int runnableId = this->tasks[taskIndex]->GetRunnable(tmpCount)->GetId();
            int taskIndexPriority = this->taskPriority[taskIndex];

            readTable[runnableId * maxCycle] = runnableId;

            for (int cycle = 0; cycle < maxCycle; cycle++) {
                readTable[runnableId * maxCycle + cycle + 1] = periods[taskIndexPriority * 2 + 1] * cycle + offsets[taskIndexPriority * 2 + 1];
            }
        }
    }
}

void DAG::GetWriteTable(double* endTable, int size, int maxCycle, double* writeTable) {
    // --------------------------------------------------------------------------------------------------------------
    // endTable : [(maxCycle + 1) X (Runnable or Task) size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : (Runnable or Task) ID
    // 2 : First Cycle
    // 3 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------
    // writeTable : [(maxCycle + 1) X Runnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their IDs
    // 1 : Runnable ID
    // 2 : First Cycle
    // 3 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------

    // 직관적인 방법이 있지만, 실행시간을 줄이기 위해 아래 방법 채택
    if (size == this->GetNumberOfRunnables()) {
        for (int runnableIndex = 0; runnableIndex < size; runnableIndex++) {
            std::memcpy(writeTable[startTable[runnableIndex * maxCycle] * maxCycle], endTable[runnableIndex * maxCycle], sizeof(double) * (maxCycle + 1));
        }

    } else if (size == this->GetNumberOfTasks()) {
        for (int taskIndex = 0; taskIndex < size; taskIndex++) {
            int numberOfRunnables = this->tasks[taskIndex]->GetNumberOfRunnables();

            for (int tmpCount = 0; tmpCount < numberOfRunnables; tmpCount++) {
                writeTable[this->tasks[taskIndex]->GetRunnable(tmpCount)->GetId() * maxCycle] = this->tasks[taskIndex]->GetRunnable(tmpCount)->GetId();
                std::memcpy(writeTable[this->tasks[taskIndex]->GetRunnable(tmpCount)->GetId() * maxCycle + 1], endTable[this->taskPriority[taskIndex] * maxCycle + 1], sizeof(double) * maxCycle);
            }
        }
    }
}

// TASK 기준에서만 가능
void DAG::GetReleaseTimeWriteTable(double* periods, double* offsets, int size, int maxCycle, double* writeTable) {
    // --------------------------------------------------------------------------------------------------------------
    // periods : [2 X (Runnable or Task) size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : (Runnable or Task) ID
    // 2 : (Runnable or Task) Period
    // --------------------------------------------------------------------------------------------------------------
    // offsets : [2 X (Runnable or Task) size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : (Runnable or Task) ID
    // 2 : (Runnable or Task) Offset
    // --------------------------------------------------------------------------------------------------------------
    // writeTable : [(maxCycle + 1) X (Runnable or Task) size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their IDs
    // 1 : (Runnable or Task) ID
    // 2 : First Cycle
    // 3 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------

    for (int taskIndex = 0; taskIndex < size; taskIndex++) {
        int numberOfRunnables = this->tasks[taskPriority[taskIndex]]->GetNumberOfRunnables();

        for (int tmpCount = 0; tmpCount < numberOfRunnables; tmpCount++) {
            int runnableId = this->tasks[taskIndex]->GetRunnable(tmpCount)->GetId();
            int taskIndexPriority = this->taskPriority[taskIndex];

            writeTable[runnableId * maxCycle] = runnableId;

            for (int cycle = 0; cycle < maxCycle; cycle++) {
                writeTable[runnableId * maxCycle + cycle + 1] = periods[taskIndexPriority * 2 + 1] * (cycle + 1) + offsets[taskIndexPriority * 2 + 1];
            }
        }
    }
}

// TASK 기준에서만 가능
void DAG::GetNextJobWriteTable(double* startTable, int size, int maxCycle, double* writeTable) {
    // --------------------------------------------------------------------------------------------------------------
    // startTable : [(maxCycle + 1) X (Runnable or Task) size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : (Runnable or Task) ID
    // 2 : First Cycle
    // 3 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------
    // writeTable : [(maxCycle + 1) X Runnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their IDs
    // 1 : Runnable ID
    // 2 : First Cycle
    // 3 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------

    for (int taskIndex = 0; taskIndex < size; taskIndex++) {
        int numberOfRunnables = this->task[taskIndex]->GetNumberOfRunnables();

        for (int tmpCount = 0; tmpCount < numberOfRunnables; tmpCount++) {
            writeTable[this->tasks[taskIndex]->GetRunnable(tmpCount)->GetId() * maxCycle] = this->tasks[taskIndex]->GetRunnable(tmpCount)->GetId();
            std::memcpy(writeTable[this->tasks[taskIndex]->GetRunnable(tmpCount)->GetId() * maxCycle + cycle + 1], startTable[this->taskPriority[taskIndex] * maxCycle + 1], sizeof(double) * (maxCycle - 1));
            writeTable[this->tasks[taskIndex]->GetRunnable(tmpCount)->GetId() * maxCycle + (maxCycle - 1)] = startTable[this->taskPriority[taskIndex] * maxCycle] + this->GetHyperPeriod();
        }
    }
}

void DAG::DataFlow(int thisRunnableId, int cycle, double intputTime, double* readTable, double* writeTable, int maxCycle, double* outputTimeTable) {
    // --------------------------------------------------------------------------------------------------------------
    // readTable : [(maxCycle + 1) X (Runnable or Task) size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their IDs
    // 1 : (Runnable or Task) ID
    // 2 : First Cycle
    // 3 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------
    // writeTable : [(maxCycle + 1) X (Runnable or Task) size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their IDs
    // 1 : (Runnable or Task) ID
    // 2 : First Cycle
    // 3 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------
    // reactionTime : [maxCycle X InputRunnable size X OutputRunnable size]     Output
    // --------------------------------------------------------------------------------------------------------------

    if (this->runnables[thisRunnableId]->GetStatus() == 1) {
        outputTimeTable[runnablePriority[thisRunnableId] * numberOfInputRunnables * maxCycle + inputRunnableIndex * maxCycle + cycle] = writeTable[runnablePriority[thisRunnableID] * maxCycle + cycle];
    } else {
        for (int outputRunnableIndex = 0; outputRunnableIndex < inputRunnable->GetNumberOfOutputRunnables(); outputRunnableIndex++) {
            if (inputTime <= readTable[runnablePriority[thisRunnableId]]) {
                this->ReactionTimeFlow(startTime, cycle, this->runnables[thisRunnableId]->GetOutputRunnableId(outputRunnableIndex), readTable, writeTable, maxCycle, reactionTime);
            }
        }
    }
}

void DAG::GetReactionTime(double* readTable, double* writeTable, int numberOfRunnables, int maxCycle, double* reactionTime) {
    // --------------------------------------------------------------------------------------------------------------
    // readTable : [(maxCycle + 1) X (Runnable or Task) size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their IDs
    // 1 : (Runnable or Task) ID
    // 2 : First Cycle
    // 3 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------
    // writeTable : [(maxCycle + 1) X (Runnable or Task) size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their IDs
    // 1 : (Runnable or Task) ID
    // 2 : First Cycle
    // 3 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------
    // reactionTime : [maxCycle X InputRunnable size X OutputRunnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    
    int numberOfInputRunnables = this->GetNumberOfInputRunnables();
    int numberOfOutputRunnables = this->GetNumberOfOutputRunnables();

    for (int inputRunnableIndex = 0; inputRunnableIndex < numberOfInputRunnables; inputRunnableIndex++) {
        for (int cycle = 0; cycle < maxCycle; cycle++) {
            double inputTime = readTable[runnablePriority[this->inputRunnables[inputRunnableIndex]->GetId()] * maxCycle];
            this->ReactionTimeFlow(this->inputRunnables[inputRunnableIndex]->GetId(), cycle, inputTime, inputTime, readTable, writeTable, maxCycle, reactionTime);
            reactionTime[outputRunnableIndex * numberOfInputRunnables * maxCycle + inputRunnableIndex * maxCycle + cycle] = ;
        }
    }
}

void DAG::GetDataAge(double* readTAble, double* writeTable, int maxCycle, double* dataAge) {

}