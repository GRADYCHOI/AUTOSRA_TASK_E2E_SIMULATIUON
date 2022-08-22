#include "DAG.hpp"
#include <iostream>

using namespace std;

double LCM(double a, double b) {
    int tmp1 = (int)(a * 1000);
    int tmp2 = (int)(b * 1000);

    while (tmp2 != 0) {
    	int tmp3 = tmp1 % tmp2;
        tmp1 = tmp2;
        tmp2 = tmp3;
    }

    return (double)(a * (b / (double)(tmp1 / 1000)));
}

DAG::DAG() {}

DAG::~DAG() {
    tasks.clear();
    runnables.clear();
}

void DAG::SetHyperPeriod() {
    if (this->GetNumberOfRunnables > 0) {
        double tmpPeriod = this->task[0]->GetPeriod();
        int numberOfTasks = this->GetNumberOfTasks();

        for(int tmpCount = 1; tmpCount < numberOfTasks; tmpCount++) {
            tmpPeriod = LCM(tmpPeriod, this->task[tmpCount]->GetPeriod());
        }

        this->hyperPeriod = tmpPeriod;
    }
}

void DAG::AddRunnablePtr(const std::shared_ptr<RUNNABLE>& runnable) {
    this->runnables.push_back(runnable);
}

void DAG::GenerateRunnables(int numumberOfRunnables) {
    std::cout << "Generate Runnables Start" << std::endl;

    for (int runnableIndex = 0; runnableIndex < numberOfRunnables; runnableIndex++) {
        std::shared_ptr<RUNNABLE> tmpRunnable(new RUNNABLE(runnableIndex, (double)((std::rand() % 100) / 1000)));
        std::cout << "Runnable ID : " << runnable->GetId() << ", Execution Time : " << runnable->GetExecutionTime() << std::endl;

        this->AddRunnablePtr(tmpRunnable);
    }

    //RandomEdge();
    //SetRunnablePrecedence();
    //DisplayRunnablesPtr();
}

void DAG::SetRandomEdge() { //Runnable edge random generation
    int rate = -1;
    int numberOfRunnables = this->GetNumberOfRunnables();

    std::cout << "Enter Edge rate(20~40) : ";
    std::cin >> rate; 

    for (int runnableIndex = 0; runnableIndex < numberOfRunnables; runnableIndex++) {
        for (int tmpRunnableIndex = runnableIndex+1; tmpRunnableIndex < numberOfRunnables; tmpRunnableIndex++) {
            if ((rand() % 100) < rate) {
                this->runnables[j]->LinkOutputRunnable(this->runnables[k]->GetSharedPtr());
            }
        }
    }
}

int DAG::CheckPrecedence(std::shared_ptr<RUNNABLE> runnable, int precedence) {
    precedence++;

    if (runnable->GetInputRunnable() == 0) {
        runnable->SetPrecedence(precedence);
        std::cout << runnable->GetId() <<"-th Runnable : input runnable complete" << std::endl;
        return 0;
    }

    if (runnable->GetOutputRunnable() == 0) { //이미 precedence가 셋팅되어있으면 안하는식으로 수정
        runnable->SetPrecedence(precedence);            
        std::cout << runnable->GetId() <<"-th Runnable : output runnable complete" << std::endl;
    } else {
        for (auto &OutputRunnable : runnable->GetOutputRunnables()) {
            this->CheckPrecedence(OutputRunnable, precedence);
        }
    }
    
    runnable->SetPrecedence(precedence);
}

void DAG::SetRunnablePrecedence() {
    for (auto &runnable : this->runnables) {
        int precedence = -1;
        this->CheckPrecedence(runnable, precedence);
        //std::cout << precedence << std::endl;
    }
}

void DAG::DisplayRunnables(){
    std::cout << "[Debugging] Display Start" << std::endl;
    std::cout << " - Runnable Vector Size : " << std::cout.width(5) << this->runnables.size() << std::endl;
    std::cout << " - Runnable Vector capacity : " << std::cout.width(5) << this->runnables.capacity() << std::endl;
    std::endl;

    for (const auto &runnable : this->runnables) {
        std::cout << "[" <<  runnable->GetId() << "th Runnable]" << std::endl;
        std::cout << " - Execution Time : " << std::cout.width(5) << runnable->GetExecutionTime() << " , ";
        std::cout << "Precedence : " << std::cout.width(5) << runnable->GetPrecedence() << " , ";
        if (runnable->GetStatus() == 0) std::cout << "Status : INPUT  , ";
        else if (runnable->GetStatus() == 1) std::cout << "Status : OUTPUT , ";
        else if (runnable->GetStatus() == 2) std::cout << "Status : MIDDLE , ";
        std::cout << "Output Runnable : " << runnable->DisplayOutputRunnables();
        std::endl;
    }
}

int DAG::GetNumberOfTasks() {
    return (int)this->tasks.size();
}

int DAG::GetNumberOfRunnables() {
    return (int)this->runnnables.size();
}

double DAG::GetHyperPeriod() const {
    return this->hyperPeriod;
}

void DAG::SetInputRunnableList() {
    // vector capacity control : swap technique
    std::vector<int> tmpList;
    int numberOfRunnables = this->GetNumberOfRunnables();

    for (int runnableIndex = 0; runnableIndex < numberOfRunnables; runnableIndex++) {
        if (this->runnables[runnableIndex]->GetStatus() == 0) {
            tmpList.push_back(this->runnables[runnableIndex]->GetId());
        }
    }

    tmpList.swap(this->inputRunnables);
}

void DAG::SetOutputRunnableList() {
    std::vector<int> tmpList;
    int numberOfRunnables = this->GetNumberOfRunnables();

    for (int runnableIndex = 0; runnableIndex < numberOfRunnables; runnableIndex++) {
        if (this->runnables[runnableIndex]->GetStatus() == 1) {
            tmpList.push_back(this->runnables[runnableIndex]->GetId());
        }
    }

    tmpList.swap(this->outputRunnables);
}

int DAG::GetNumberOfInputRunnables() {
    return (int)this->intputRunnables.size();
}

int DAG::GetNumberOfOutputRunnables() {
    return (int)this->outputRunnables.size();
}

void DAG::AddTaskPtr(const std::shared_ptr<Task>& task) {
    this->tasks.push_back(task);
}

void DAG::GenerateTasks(int numberOfTasks) {
    double tmpPeriod = -1.0;
    double tmpOffset = -1.0;

    for (int taskIndex = 0; taskIndex < numberOfTasks; taskIndex++) {
        std::cout << taskIndex << " -th Task Period : ";
        std::cin >> tmpPeriod;
        std::cout << taskIndex << " -th Task Offset : ";
        std::cin >> tmpOffset;

        shared_ptr<TASK> task(new TASK(taskIndex, tmpPeriod, tmpOffset));
        this->AddTaskPtr(task);

        std::cout << "Task ID : " << task->GetId() << ", Period : " << task->GetPeriod() << std::endl;
    }
}

// TODO: Random Mapping
void DAG::DoTaskMapping() {
    int numberOfRunnables = this->GetNumberOfRunnables();

    for (int RunnableIndex = 0; runnableIndex < numberOfRunnables; runnableIndex++) {
        
    }
}

void DAG::Simulate() {
    std::cin >> option >> std::endl;

    /* 0 : IMPLICIT_RUNNABLE
       1 : IMPLICIT_TASK
       2 : LET              */
    switch (option) {
        case 0:
            break;
        case 1:
            break;
        case 2:
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

void DAG::SimulateTaskImplicitTask() {
    int maxCycle = this->GetMaxCycle();
    int taskSize = this->GetNumberOfTasks();
    int runnableSize = this->GetNumberOfRunnables();

    double* taskPeriods = new double[taskSize];
    double* taskOffsets = new double[taskSize];
    double* taskExecutionTimes = new double[taskSize];
    double* taskStartTable = new double[taskSize * (maxCycle + 1)];
    double* taskEndTable = new double[taskSize * (maxCycle + 1)];
    double* taskReactionTime = new double[this->GetNumberOfOutputRunnables() * maxCycle * this->GetNumberOfInputRunnables()];
    double* taskDataAge = new double[this->GetNumberOfOutputRunnables() * maxCycle * this->GetNumberOfInputRunnables()];

    std::memset(taskPeriods, -1.0, sizeof(double) * taskSize);
    std::memset(taskOffsets, -1.0, sizeof(double) * taskSize);
    std::memset(taskExecutionTimes, -1.0, sizeof(double) * taskSize);
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
    // ## The order of Task is based on their priority
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
    // ## The order of Task is based on their priority
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
    // ## The order of Task is based on their priority
    // 1 : Task ID
    // 2 : Task Execution Time
    // --------------------------------------------------------------------------------------------------------------

    int numberOfTasks = this->GetNumberOfTasks();

    for (int taskIndex = 0; taskIndex < numberOfTasks; taskIndex++) {
        executions[this->taskPriority[taskIndex] * 2] = this->tasks[taskIndex]->GetId();
        executions[this->taskPriority[taskIndex] * 2 + 1] = 0;

        int numberOfRunnables = this->tasks[taskIndex]->GetNumberOfRunnables();

        for (int tmpCount = 0; tmpCount < numberOfRunnables; tmpCount++) {
            executions[this->taskPriority[taskIndex] * 2 + 1] += this->tasks[taskIndex]->GetRunnable(tmpCount)->GetExecutionTime();
        }
    }
}

void DAG::GetRunnablePeriods(double* periods) {
    // --------------------------------------------------------------------------------------------------------------
    // periods : [2 X Runnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their priority
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
    // ## The order of Runnable is based on their priority
    // 1 : Runnable ID
    // 2 : Runnable Offset
    // --------------------------------------------------------------------------------------------------------------

    int numberOfTasks = this->GetNumberOfTasks();
    
    for (int taskIndex = 0; taskIndex < numberOfTasks; taskIndex++) {
        int numberOfRunnables = this->tasks[taskIndex]->GetNumberOfRunnables();  

        for (int tmpCount = 0; tmpCount < numberOfRunnables; tmpCount++) {
            offsets[this->runnablePriority[this->tasks[taskIndex]->GetRunnable(tmpCount)->GetId()] * 2] = this->tasks[taskIndex]->runnables[tmpCount]->GetId();
            offsets[this->runnablePriority[this->tasks[taskIndex]->GetRunnable(tmpCount)->GetId()] * 2 + 1] = this->tasks[taskIndex]->runnables[tmpCount]->GetOffset();
        }
    }
}

void DAG::GetRunnableExecutionTimes(double* executions) {
    // --------------------------------------------------------------------------------------------------------------
    // executions : [2 X Runnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their priority
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
    // ## The order of Task is based on their priority
    // 1 : Runnable ID
    // 2 : Runnable Period
    // --------------------------------------------------------------------------------------------------------------
    // offsets : [2 X Runnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Task is based on their priority
    // 1 : Runnable ID
    // 2 : Runnable Offset
    // --------------------------------------------------------------------------------------------------------------
    // executions : [2 X Runnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Task is based on their priority
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
    // ## The order of Runnable is based on their priority
    // 1 : Runnable ID
    // 2 : Runnable Period
    // --------------------------------------------------------------------------------------------------------------
    // offsets : [2 X Runnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their priority
    // 1 : Runnable ID
    // 2 : Runnable Offset
    // --------------------------------------------------------------------------------------------------------------
    // executions : [2 X Runnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their priority
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

// Both of Runnable Table & Task Table
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
    // readTable : [maxCycle X Runnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their IDs
    // 1 : First Cycle
    // 2 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------

    if (size == this->GetNumberOfRunnables()) {
        for (int runnableIndex = 0; runnableIndex < size; runnableIndex++) {
            std::memcpy(readTable[startTable[runnableIndex * maxCycle] * maxCycle], startTable[runnableIndex * maxCycle + 1], sizeof(double) * maxCycle);
        }

    } else if (size == this->GetNumberOfTasks()) {
        for (int taskIndex = 0; taskIndex < size; taskIndex++) {
            int numberOfRunnables = this->tasks[taskIndex]->GetNumberOfRunnables();

            for (int tmpCount = 0; tmpCount < numberOfRunnables; tmpCount++) {
                std::memcpy(readTable[this->tasks[taskIndex]->GetRunnable(tmpCount)->GetId() * maxCycle], startTable[this->taskPriority[taskIndex] * maxCycle + 1], sizeof(double) * maxCycle);
            }
        }
    }
}

// Only Task Table
void DAG::GetReleaseTimeReadTable(double* periods, double* offsets, int size, int maxCycle, double* readTable) {
    // --------------------------------------------------------------------------------------------------------------
    // periods : [2 X Task size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : Task ID
    // 2 : Task Period
    // --------------------------------------------------------------------------------------------------------------
    // offsets : [2 X Task size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Task is based on their priority
    // 1 : Task ID
    // 2 : Task Offset
    // --------------------------------------------------------------------------------------------------------------
    // readTable : [maxCycle X Runnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their IDs
    // 1 : First Cycle
    // 2 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------

    for (int taskIndex = 0; taskIndex < size; taskIndex++) {
        int numberOfRunnables = this->tasks[taskPriority[taskIndex]]->GetNumberOfRunnables();

        for (int tmpCount = 0; tmpCount < numberOfRunnables; tmpCount++) {
            int runnableId = this->tasks[taskIndex]->GetRunnable(tmpCount)->GetId();
            int taskIndexPriority = this->taskPriority[taskIndex];

            for (int cycle = 0; cycle < maxCycle; cycle++) {
                readTable[runnableId * maxCycle + cycle] = periods[taskIndexPriority * 2 + 1] * cycle + offsets[taskIndexPriority * 2 + 1];
            }
        }
    }
}

// Both of Runnable Table & Task Table
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
    // writeTable : [maxCycle X Runnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : First Cycle
    // 2 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------

    if (size == this->GetNumberOfRunnables()) {
        for (int runnableIndex = 0; runnableIndex < size; runnableIndex++) {
            std::memcpy(writeTable[startTable[runnableIndex * maxCycle] * maxCycle], endTable[runnableIndex * maxCycle + 1], sizeof(double) * maxCycle);
        }

    } else if (size == this->GetNumberOfTasks()) {
        for (int taskIndex = 0; taskIndex < size; taskIndex++) {
            int numberOfRunnables = this->tasks[taskIndex]->GetNumberOfRunnables();

            for (int tmpCount = 0; tmpCount < numberOfRunnables; tmpCount++) {
                std::memcpy(writeTable[this->tasks[taskIndex]->GetRunnable(tmpCount)->GetId() * maxCycle], endTable[this->taskPriority[taskIndex] * maxCycle + 1], sizeof(double) * maxCycle);
            }
        }
    }
}

// Only Task Table
void DAG::GetReleaseTimeWriteTable(double* periods, double* offsets, int size, int maxCycle, double* writeTable) {
    // --------------------------------------------------------------------------------------------------------------
    // periods : [2 X Task size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their priority
    // 1 : Task ID
    // 2 : Task Period
    // --------------------------------------------------------------------------------------------------------------
    // offsets : [2 X Task size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Task is based on their priority
    // 1 : Task ID
    // 2 : Task Offset
    // --------------------------------------------------------------------------------------------------------------
    // writeTable : [maxCycle X Runnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable or Task is based on their IDs
    // 1 : First Cycle
    // 2 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------

    for (int taskIndex = 0; taskIndex < size; taskIndex++) {
        int numberOfRunnables = this->tasks[taskPriority[taskIndex]]->GetNumberOfRunnables();

        for (int tmpCount = 0; tmpCount < numberOfRunnables; tmpCount++) {
            int runnableId = this->tasks[taskIndex]->GetRunnable(tmpCount)->GetId();
            int taskIndexPriority = this->taskPriority[taskIndex];

            for (int cycle = 0; cycle < maxCycle; cycle++) {
                writeTable[runnableId * maxCycle + cycle] = periods[taskIndexPriority * 2 + 1] * (cycle + 1) + offsets[taskIndexPriority * 2 + 1];
            }
        }
    }
}

// Only Task Table
void DAG::GetNextJobWriteTable(double* startTable, int size, int maxCycle, double* writeTable) {
    // --------------------------------------------------------------------------------------------------------------
    // startTable : [(maxCycle + 1) X Task size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Task is based on their priority
    // 1 : Task ID
    // 2 : First Cycle
    // 3 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------
    // writeTable : [maxCycle X Runnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : First Cycle
    // 2 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------

    for (int taskIndex = 0; taskIndex < size; taskIndex++) {
        int numberOfRunnables = this->task[taskIndex]->GetNumberOfRunnables();

        for (int tmpCount = 0; tmpCount < numberOfRunnables; tmpCount++) {
            std::memcpy(writeTable[this->tasks[taskIndex]->GetRunnable(tmpCount)->GetId() * maxCycle + cycle], startTable[this->taskPriority[taskIndex] * maxCycle + 1], sizeof(double) * (maxCycle - 1));
            writeTable[this->tasks[taskIndex]->GetRunnable(tmpCount)->GetId() * maxCycle + (maxCycle - 1)] = startTable[this->taskPriority[taskIndex] * maxCycle] + this->GetHyperPeriod();
        }
    }
}

void DAG::SetArrivalTable(double* readTable, double* writeTable, int inputRunnableIndex, int inputCycle, int hyperPeriodCount, int thisRunnableId, int thisCycle, int maxCycle, double* arrivalTable) {
    // --------------------------------------------------------------------------------------------------------------
    // readTable : [maxCycle X Runnable size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : First Cycle
    // 2 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------
    // writeTable : [maxCycle X Runnable size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : First Cycle
    // 2 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------
    // arrivalTable : [maxCycle X InputRunnable size X OutputRunnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : First Cycle
    // 2 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------

    if (this->runnables[thisRunnableId]->GetStatus() == 1) {
        arrivalTable[(find(this->outputRunnables.begin(), this->outputRunnables.end(), thisRunnableId) - this->outputRunnables.begin()) * this->GetnumberOfInputRunnables() * maxCycle + inputRunnableIndex * maxCycle + inputCycle] = writeTable[thisRunnableId * maxCycle + thisCycle] + this->GetHyperPeriod() * hyperPeriodCount;
    } else {
        for (int tmpCount = 0; tmpCount < this->runnables[thisRunnableId]->GetNumberOfOutputRunnables(); tmpCount++) {
            int tmpCycle = 0;
            int outputRunnableId = this->runnables[thisRunnableId]->GetOutputRunnableId(tmpCount);
            int readTime = readTable[outputRunnableId * maxCycle + tmpCycle];

            // hyperPeriod가 서로 같으니 생략
            while (writeTable[thisRunnableId * maxCycle + thisCycle] > readtime) {
                tmpCycle++;

                if (tmpCycle == maxCycle || readTable[outputRunnableId * maxCycle + tmpCycle] == -1.0) {
                    readtime = readTable[outputRunnableId * maxCycle] + this->GetHyperPeriod();
                    tmpCycle = 0;
                    hyperPeriodCount++;
                } else {
                    readtime = readTable[outputRunnableId * maxCycle + tmpCycle];
                }
            }

            this->SetArrivalTable(readTable, writeTable, inputRunnableIndex, hyperPeriodCount, inputCycle, outputRunnableId, tmpCycle, maxCycle, reactionTime);
        }
    }
}

void DAG::GetArrivalTable(double* readTable, double* writeTable, int maxCycle, double* arrivalTable) {
    // --------------------------------------------------------------------------------------------------------------
    // readTable : [maxCycle X Runnable size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : First Cycle
    // 2 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------
    // writeTable : [maxCycle X Runnable size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : First Cycle
    // 2 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------
    // arrivalTime : [maxCycle X InputRunnable size X OutputRunnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : First Cycle
    // 2 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------

    int numberOfInputRunnables = this->GetNumberOfInputRunnables();

    for (int inputRunnableIndex = 0; inputRunnableIndex < numberOfInputRunnables; inputRunnableIndex++) {
        for (int cycle = 0; cycle < maxCycle; cycle++) {
            this->SetArrivalTable(readTable, writeTable, inputRunnableIndex, cycle, 0, inputRunnableIndex, cycle, maxCycle, arrivalTable);
        }
    }
}

void DAG::GetReactionTime(double* arrivalTable, double* readTable, int maxCycle, double* reactionTime) {
    // --------------------------------------------------------------------------------------------------------------
    // arrivalTime : [maxCycle X InputRunnable size X OutputRunnable size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : First Cycle
    // 2 : Second Cycle
    // ..    
    // --------------------------------------------------------------------------------------------------------------
    // readTable : [(maxCycle + 1) X Runnable size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : Runnable ID
    // 2 : First Cycle
    // 3 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------
    // reactionTime : [maxCycle X InputRunnable size X OutputRunnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : First Cycle
    // 2 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------

    int numberOfInputRunnables = this->GetNumberOfInputRunnables();
    int numberOfOutputRunnables = this->GetNumberOfOutputRunnables();

    for (int inputRunnableIndex = 0; inputRunnableIndex < numberOfInputRunnables; inputRunnableIndex++) {
        for (int cycle = 0; cycle < maxCycle; cycle++) {
            for (int outputRunnableIndex = 0; outputRunnableIndex < numberOfOutputRunnables; outputRunnableIndex++) {
                double outputTime = arrivalTable[outputRunnableIndex * numberOfInputRunnables * maxCycle + inputRunnableIndex * maxCycle + cycle];

                if (outputTime > 0.0) {
                    reactionTime[outputRunnableIndex * numberOfInputRunnables * maxCycle + inputRunnableIndex * maxCycle + cycle] = outputTime - readTable[inputRunnables[inputRunnableIndex] * maxCycle + cycle];
                }
            }
        }
    }
}

void DAG::GetDataAge(double* arrivalTable, double* writeTable, int maxCycle, double* dataAge) {
    // --------------------------------------------------------------------------------------------------------------
    // arrivalTime : [maxCycle X InputRunnable size X OutputRunnable size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : First Cycle
    // 2 : Second Cycle
    // ..    
    // --------------------------------------------------------------------------------------------------------------
    // writeTable : [(maxCycle + 1) X Runnable size]     Input
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : Runnable ID
    // 2 : First Cycle
    // 3 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------
    // reactionTime : [maxCycle X InputRunnable size X OutputRunnable size]     Output
    // --------------------------------------------------------------------------------------------------------------
    // ## The order of Runnable is based on their IDs
    // 1 : First Cycle
    // 2 : Second Cycle
    // ..
    // --------------------------------------------------------------------------------------------------------------

    int numberOfInputRunnables = this->GetNumberOfInputRunnables();
    int numberOfOutputRunnables = this->GetNumberOfOutputRunnables();
}