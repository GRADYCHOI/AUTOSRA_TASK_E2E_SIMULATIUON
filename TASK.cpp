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

    for (int count = 0; 0 < this->runnables.size(); count++) {
        tmpExecutionTime += this->runnables[count]->GetExecutionTime();
    }

    this->executionTime = tmpExecutionTime;
}

void TASK::SetNumberOfRunnables() {
    this->numberOfRunnables = this->runnables.size();
}

void TASK::SetPriority(int priority) {
    this->priority = priority;
}

double TASK::GetPeriod() {
    return this->period;
}

double TASK::GetOffset() {
    return this->offset;
}

double TASK::GetExecutionTime() {
    return this->executionTime;
}

int TASK::GetNumberOfRunnables() {
    return this->numberOfRunnables;
}

int TASK::GetPriority() {
    return this->priority;
}

void TASK::AddRunnable(const std::shared_ptr<RUNNABLE>& runnable) {
    bool searchFlag = false;
    
    for (int count = 0; 0 < this->runnables.size(); count++) {
        if (this->runnables[count]->id == runnable->id) {
            searchFlag = true;
            break;
        }
    }

    if !(searchFlag) {
        this->runnables.push_back(runnable);

        this->SetNumberOfRunnables();
        this->SetExecutionTime();
    }
}