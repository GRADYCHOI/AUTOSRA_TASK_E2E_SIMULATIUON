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

    for (int count = 0; count < this->runnables.size(); count++) {
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

int TASK::GetId() {
    return this->id;
}

int TASK::GetPriority() {
    return this->priority;
}
const std::shared_ptr<RUNNABLE> TASK::GetRunnable(int index) const {
    return this->runnables[index];
}

const std::vector<std::shared_ptr<RUNNABLE>> TASK::GetRunnables() const {
    return this->runnables;
}

void TASK::AddRunnable(const std::shared_ptr<RUNNABLE> runnable) {
    bool searchFlag = false;
    
    for (int count = 0; count < (int)this->runnables.size(); count++) {
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