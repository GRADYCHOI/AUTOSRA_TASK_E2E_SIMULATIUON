#include "TASK.hpp"


TASK::TASK(int id, double period, double offset) {
    this->id = id;
    this->period = period;
    this->offset = offset;
}

TASK::~TASK() {
    std::cout << "Delete TASK ID : " << this->id << std::endl;
}

void TASK::SetExecutionTime(double executionTime) {
    this->executionTime = executionTime;
}

void TASK::SetNumberOfRunnables() {
    this->numberOfRunnables = this->runnables.size();
}

void TASK::SetPriority(int priority) {

}

double TASK::GetPeriod() {

}

double TASK::GetOffset() {

}

double TASK::GetExecutionTime() {

}

int TASK::GetNumberOfRunnables() {

}

int TASK::GetPriority() {

}