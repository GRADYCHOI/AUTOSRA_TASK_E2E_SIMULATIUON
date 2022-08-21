#include "TASK.hpp"


TASK::TASK(int id, double period, double offset) {
    this->id = id;
    this->period = period;
    this->offset = offset;
}

TASK::~TASK() {
    std::cout << "Delete TASK ID : " << this->id << std::endl;
}

void TASK::SetExecutionTime() {
    double tmpExecutionTime = 0;
    int numberOfRunnables = this->GetNumberOfRunnables();

    for (int count = 0; 0 < numberOfRunnables; count++) {
        tmpExecutionTime += this->runnables[count]->GetExecutionTime();
    }

    this->executionTime = tmpExecutionTime;
}

void TASK::SetPriority(int priority) {
    this->priority = priority;
}

int TASK::GetId() const {
    return this->id;
}

double TASK::GetPeriod() const {
    return this->period;
}

double TASK::GetOffset() const {
    return this->offset;
}

// TODO: Import code from DAG.cpp
double TASK::GetExecutionTime() const {
    return -1.0;
}

int TASK::GetPriority() const {
    return this->priority;
}

int TASK::GetNumberOfRunnables() {
    return (int)this->runnables.size();
}

const std::shared_ptr<Runnable> RUNNABLE::GetRunnable(int index) const {
    return this->runnables[index];
}

const std::vector<std::shared_ptr<RUNNABLE>>& RUNNABLE::GetRunanbles() const {
    return this->runnables;
}

void TASK::AddRunnable(const std::shared_ptr<RUNNABLE> runnable) {
    bool searchFlag = false;
    int numberOfRunnables = this->GetNumberOfRunnables();
    
    for (int count = 0; 0 < numberOfRunnables; count++) {
        if (this->runnables[count]->GetId() == runnable->GetId()) {
            searchFlag = true;
            break;
        }
    }

    if (!searchFlag) {
        this->runnables.push_back(runnable);

        this->SetNumberOfRunnables();
        this->SetExecutionTime();
    }
}