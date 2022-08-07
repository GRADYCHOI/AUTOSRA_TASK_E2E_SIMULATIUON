#include "DAG.hpp"
#include<iostream>

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

    std::memset(taskPeriods, -1, sizeof(double) * taskSize);
    std::memset(taskOffsets, -1, sizeof(double) * taskSize);
    std::memset(taskExecutionTimes, -1, sizeof(double) * taskSize);
    std::memset(taskReleaseTable, -1, sizeof(double) * taskSize * maxCycle);
    std::memset(taskStartTable, -1, sizeof(double) * taskSize * maxCycle);
    std::memset(taskEndTable, -1, sizeof(double) * taskSize * maxCycle);

    this->GetTaskPeriods(taskSize, taskPeriods);
    this->GetTaskOffsets(taskSize, taskOffsets);
    this->GetTaskExecutionTimes(taskSize, taskExecutionTimes);
    this->GetReleaseTable(taskPeriods, taskOffsets, taskSize, maxCycle, taskReleaseTable);
    this->GetExecutionTable(taskReleaseTable, taskExecutionTimes, taskSize, maxCycle, taskStartTable, taskEndTable);

    delete[] taskEndTable;
    delete[] taskStartTable;
    delete[] taskReleaseTable;
    delete[] taskExecutionTimes;
    delete[] taskOffsets;
    delete[] taskPeriods;
}

void DAG::GetTaskPeriods(int taskSize, double* periods) {
    for (int rows = 0; rows < taskSize; rows++) {
        periods[rows] = this->task[rows]->GetPeriod();
    }
}

void DAG::GetTaskOffsets(int taskSize, double* offsets) {
    for (int rows = 0; rows < taskSize; rows++) {
        offsets[rows] = this->task[rows]->GetOffset();
    }
}

void DAG::GetTaskExecutionTimes(int taskSize, double* executions) {
    for (int rows = 0; rows < taskSize; rows++) {
        executions[rows] = 0;
        int numberOfRunnables = this->task[rows]->GetNumberOfRunnables();

        for (int index = 0; index < numberOfRunnables; index++) {
            executions[rows] += this->task[rows]->runnables[index]->GetExecutionTime();
        }
    }
}

void DAG::GetRunnablePeriods(int runnableSize, double* periods) {
    int index = 0;

    for (int rows = 0; rows < runnableSize; ) {
        int numberOfRunnables = this->task[index]->GetNumberOfRunnables();

        for (int tmpCount = 0; tmpCount < numberOfRunnables; tmpCount++) {
            periods[rows] = this->task[index]->GetPeriod();
            rows++;
        }

        index++;
    }
}

void DAG::GetRunnableOffsets(int runnableSize, double* offsets) {
    int index = 0;

    for (int rows = 0; rows < runnableSize; ) {
        int numberOfRunnables = this->task[index]->GetNumberOfRunnables();

        for (int tmpCount = 0; tmpCount < numberOfRunnables; tmpCount++) {
            offsets[rows] = this->task[index]->GetOffset();
            rows++;
        }

        index++;
    }
}

void DAG::GetRunnableExecutionTimes(int runnableSize, double* executions) {
    for (int rows = 0; rows < runnableSize; rows++) {
        executions[rows] = this->runnables[index]->GetExecutionTime();
    }
}

void DAG::GetReleaseTable(double* periods, double* offsets, int size, int maxCycle, double* releaseTable) {
    for (int rows = 0; rows < size; rows++) {
        double period = periods[rows];
        double offset = offsets[rows];
        int cycle = static_cast<int>(this->hyperPeriod / periods[rows]);

        for (int cols = 0; cols < cycle; cols++) {
            double releaseTime = period * cols + offset;
            releaseTable[rows * maxCycle + cols] = releaseTime;
        }
    }
}

void DAG::GetExecutionTable(double* periods, double* offsets, double* executions, int size, int maxCycle, double* startTable, double* endTable) {
    std::vector<std::pair<double,double>> empty_times;
    empty_times.push_back({0, this->hyperPeriod});

    for (int rows = 0; rows < size; rows++) {
        double period = periods[rows];
        double offset = offsets[rows];
        double execution = executions[rows];
        int cycle = static_cast<int>(this->hyperPeriod / periods[rows]);

        for (int cols = 0; cols < cycle; cols++) {
            double releaseTime = period * cols + offset;
        }
    }
}