#include "DAG.hpp"
#include <iostream>

using namespace std;

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

DAG::DAG() {}

DAG::~DAG() {}

void DAG::SetParameters() {
    this->SetNumberOfTasks();
    this->SetNumberOfRunnables();
    this->SetHyperPeriod();
}

void DAG::SetHyperPeriod() {
    double tmpPeriod = 0.0;

    for(int count = 0; count < this->numOfTasks; count++) {
        if (tmpPeriod < this->task[count]->GetPeriod()) {
            this->hyperPeriod = lcm(this->hyperPeriod, this->task[count]->GetPeriod());
            tmpPeriod = this->task[count]->GetPeriod();
        }
    }
}

void DAG::SetNumberOfTasks() {
    this->numOfTasks = this->task.size();
}

void DAG::SetNumberOfRunnables() {
    this->numOfRunnables = 0;

    for (int count = 0; count < this->numOfTasks; count++) {
        this->numOfRunnables += this->task[count]->GetNumberOfRunnables();
    }
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
    return this->numOfTasks;
}

int DAG::GetNumberOfRunnables() {
    return this->numOfRunnables;
}

double DAG::GetHyperPeriod() {
    return this->hyperPeriod;
}

double DAG::GetReactionTime() {
    return this->reactionTime;
}

double DAG::GetDataAge() {
    return this->dataAge;
}

/*
void DAG::AddTask(Task& task) {
    this->task.push_back(task);

    this->SetParamters();
}*/

void DAG::Simulate() {
    std::cin >> option >> std::endl;

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
    int maxCycle = static_cast<int>(this->hyperPeriod / this->task->runnables->getPeriod());

    int* runnableCyclePtr = new int[this->runnables.size()];
    double* runnableExecutionTable = new double[this->runnables.size() * maxCycle];



    delete[] runnableExecutionTable;
    delete[] runnableCyclePtr;
}

void DAG::SimulateImplicitTask() {
    int maxCycle = static_cast<int>(this->hyperPeriod / this->task[0]->getPeriod());
    int taskSize = this->task.size();

    double* taskPeriods = new double[taskSize];
    double* taskOffsets = new double[taskSize];
    double* taskExecutionTimes = new double[taskSize];
    double* taskReleaseTable = new double[taskSize * maxCycle];
    double* taskStartTable = new double[taskSize * maxCycle];
    double* taskEndTable = new double[taskSize * maxCycle];
    double* taskReactionTime = new double[maxCycle];
    double* taskDataAge = new double[maxCycle];

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

void DAG::GetTaskPeriods(int taskSize, double* periods) {
    for (int taskRows = 0; taskRows < taskSize; taskRows++) {
        periods[taskRows] = this->task[taskRows]->GetPeriod();
    }
}

void DAG::GetTaskOffsets(int taskSize, double* offsets) {
    for (int taskRows = 0; taskRows < taskSize; taskRows++) {
        periods[taskRows] = this->task[taskRows]->GetOffset();
    }
}

void DAG::GetTaskExecutionTimes(int taskSize, double* executions) {
    for (int taskRows = 0; taskRows < taskSize; taskRows++) {
        executions[taskRows] = 0;
        int numberOfRunnables = this->task[taskRows]->GetNumberOfRunnables();

        for (int tmpCount = 0; tmpCount < numberOfRunnables; tmpCount++) {
            executions[taskRows] += this->task[taskRows]->runnables[tmpCount]->GetExecutionTime();
        }
    }
}

void DAG::GetRunnablePeriods(int runnableSize, double* periods) {
    int mappingPtr = 0;

    for (int runnableRows = 0; runnableRows < runnableSize; ) {
        int numberOfRunnables = this->task[mappingPtr]->GetNumberOfRunnables();

        for (int tmpCount = 0; tmpCount < numberOfRunnables; tmpCount++) {
            periods[runnableRows] = this->task[mappingPtr]->GetPeriod();
            runnableRows++;
        }

        mappingPtr++;
    }
}

void DAG::GetRunnableOffsets(int runnableSize, double* offsets) {
    int mappingPtr = 0;

    for (int runnableRows = 0; runnableRows < runnableSize; ) {
        int numberOfRunnables = this->task[mappingPtr]->GetNumberOfRunnables();

        for (int tmpCount = 0; tmpCount < numberOfRunnables; tmpCount++) {
            offsets[runnableRows] = this->task[mappingPtr]->GetOffset();
            runnableRows++;
        }

        mappingPtr++;
    }
}

void DAG::GetRunnableExecutionTimes(int runnableSize, double* executions) {
    for (int rows = 0; rows < runnableSize; rows++) {
        executions[rows] = this->runnables[index]->GetExecutionTime();
    }
}

void DAG::GetTaskInfo(int taskSize, double* periods, double* offsets, double* executions) {
    this->GetTaskPeriods(taskSize, periods);
    this->GetTaskOffsets(taskSize, offsets);
    this->GetTaskExecutionTimes(taskSize, executions);
}

void DAG::GetRunnableInfo(int runnableSize, double* periods, double* offsets, double* executions) {
    this->GetRunnablePeriods(runnableSize, periods);
    this->GetRunnableOffsets(runnableSize, offsets);
    this->GetRunnableExecutionTimes(runnableSize, executions);
}


// period와 offset이 1ms 단위인 경우만 한정
void DAG::GetExecutionTable(double* periods, double* offsets, double* executions, int size, int maxCycle, double* startTable, double* endTable) {
    double* emptyTimes = new double[this->hyperPeriod];
    std::memset(emptyTimes, 1.0, sizeof(double) * this->hyperPeriod);

    for (int rows = 0; rows < size; rows++) {
        double period = periods[rows];
        double offset = offsets[rows];
        double execution = executions[rows];
        int cycle = static_cast<int>(this->hyperPeriod / periods[rows]);

        for (int cols = 0; cols < cycle; cols++) {
            double releaseTime = period * cols + offset;
            double deadTime = period * (cols + 1) + offset;

            int integerReleaseTime = static_cast<int>(std::floor(releaseTime));
            int integerDeadTime = static_cast<int>(std::floor(deadTime));

            // Regard time-line
            while !(emptyTimes[integerReleaseTime]) integerReleaseTime++;

            // Set start table
            startTable[rows * maxCycle + cols] = static_cast<double>(integerReleaseTime) + 1 - emptyTimes[integerReleaseTime];

            // Set end table
            while (execution) {
                if (emptyTimes[integerReleaseTime] < execution) {
                    integerReleaseTime += 1;
                    execution -= emptyTimes[integerReleaseTime];
                    emptyTimes[integerReleaseTime] = 0.0;
                } else {
                    endTable[rows * maxCycle + cols] = releaseTime + execution;

                    emptyTimes[integerReleaseTime] -= execution;
                    execution = 0;
                }
            }
        }
    }

    delete[] emptyTimes;
}

/* TODO : period와 offset이 double형 값인 경우에 대응되는 방법 고안
    vector의 연결리스트로 결과값 서로 잇기

void DAG::GetExecutionTable(double* periods, double* offsets, double* executions, int size, int maxCycle, double* startTable, double* endTable) {
    int numberOfEmptyTimesColumn = EMPTY_TIME_THRESHOLD * 2 + 1;
    double* emptyTimes = new double[this->hyperPeriod * numberOfEmptyTimesColumn]; // First Column is remained time capacity
    std::memset(emptyTimes, 1, sizeof(double) * numberOfEmptyTimesColumn);

    for (int rows = 0; rows < size; rows++) {
        double period = periods[rows];
        double offset = offsets[rows];
        double execution = executions[rows];
        int cycle = static_cast<int>(this->hyperPeriod / periods[rows]);

        for (int cols = 0; cols < cycle; cols++) {
            double releaseTime = period * cols + offset;
            double deadTime = period * (cols + 1) + offset;

            int integerReleaseTime = static_cast<int>(std::floor(releaseTime));
            int integerDeadTime = static_cast<int>(std::floor(deadTime));
            double mantissaReleaseTime = releaseTime - std::floor(releaseTime);
            double mantissaDeadTime = deadTime - std::floor(deadTime);
        }
    }
}

*/

void DAG::SetInputRunnableList() {
    // vector capacity control : swap technique
    std::vector<std::shared_ptr<RUNNABLE>> tmpList;
    int numberOfRunnables = this->task[index]->GetNumberOfRunnables();

    for (int tmpCount = 0; tmpCount < numberOfRunnables; tmpCount++) {
        if (this->runnables[tmp]->GetStatus() == 1) {
            tmpList.push_back(this->runnables[tmp]->GetSharedPtr());
        }
    }

    tmpList.swap(this->inputRunnables);
}

void DAG::SetOutputRunnableList() {
    std::vector<std::shared_ptr<RUNNABLE>> tmpList;
    int numberOfRunnables = this->task[index]->GetNumberOfRunnables();

    for (int tmpCount = 0; tmpCount < numberOfRunnables; tmpCount++) {
        if (this->runnables[tmp]->GetStatus() == 1) {
            tmpList.push_back(this->runnables[tmp]->GetSharedPtr());
        }
    }

    tmpList.swap(this->outputRunnables);
}

void DAG::GetInputTimeTable(double* startTable, int size, int maxCycle, int option, double* inputTable, ) {
    if (size == this->GetNumberOfRunnables()) { // Task가 Runnable 수와 같은 case 포함
        for (int tmpCount = 0; tmpCount < size; tmpCount++) {

        }
    } else if (size == this->GetNumberOfTasks()) { // Task Communication인 경우

    }
}

void DAG::GetOutputTable(double* endTable, int size, int maxCycle, double* outputTable) {
    if (size == this->GetNumberOfRunnables()) {
        std::memcpy(outputTable, endTable, sizeof(double) * size * maxCycle);

    } else if (size == this->GetNumberOfTasks()) {
        int runnableRows = 0;
        
        for (int taskRows = 0; taskRows < size; taskRows++) {
            int numberOfRunnables = this->task[taskRows]->GetNumberOfRunnables();

            for (int tmpCount = 0; tmpCount < numberOfRunnables; tmpCount++) {
                std::memcpy(outputTable[runnableRows], this->runnables[taskRows], sizeof(double) * maxCycle);
                runnableRows++;
            }
        }
    }
}

void DAG::GetReleaseTimeOutputTable(double* endTable, double* periods, double* offsets, int size, int maxCycle, double* outputTable) {
    if (size == this->GetNumberOfRunnables()) {
        for (int runnableRows = 0; runnableRows < size; runnableRows++) {
            for (int cycle = 0; cycle < maxCycle; cycle++) {
                outputTable[runnableRows * maxCycle + cycle] = periods[runnableRows] * (cycle + 1) + offsets[runnableRows];
            }
        }

    } else if (size == this->GetNumberOfTasks()) {
        int runnableRows = 0;

        for (int taskRows = 0; taskRows < size; taskRows++) {
            int numberOfRunnables = this->task[taskRows]->GetNumberOfRunnables();

            for (int tmpCount = 0; tmpCount < numberOfRunnables; tmpCount++) {

                for (int cycle = 0; cycle < maxCycle; cycle++) {
                outputTable[runnableRows * maxCycle + cycle] = periods[taskRows] * (cycle + 1) + offsets[taskRows];
            }
                outputTable[runnableRows] = periods[taskRows] * (cols + 1) + offsets[taskRows];
                runnableRows++;
            }


            std::memcpy(outputTable[tmpCount], this->runnables[tmpCount], sizeof(double) * maxCycle);
        }
    }
}

void DAG::GetReactionTime(double* inputTable, double* outputTable, int maxCycle, double* reactionTime) {
    int numberOfRunnables = this->task[index]->GetNumberOfRunnables();

    for (int cols = 0; cols < maxCycle; cols++) {
        endTable

        for (int rows = 1; rows < size; rows++) {
            reactionTime[rows] = ;
        }
    }
}