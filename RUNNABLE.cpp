#include "RUNNABLE.hpp"


RUNNABLE::RUNNABLE(double execution_time, int status) {
    this->execution_time = execution_time;
    this->status = status;
}

RUNNABLE::~RUNNABLE() {
    cout << execution_time << endl;
    cout << ID << endl;
}

double RUNNABLE::GetExecutionTime() {
    return this->executionTime;
}

int RUNNABLE::GetPrecedence() {
    return this->precedence;
}

int RUNNABLE::GetStatus() {
    return this->status;
}

void RUNNABLE::AddInputRunnable(const std::shared_ptr<RUNNABLE>& inputRunnable) {
    inputRunnables.push_back(inputRunnable);
}

void RUNNABLE::AddOutputRunnable(const std::shared_ptr<RUNNABLE>& outputRunnable) {
    outputRunnables.push_back(outputRunnable);
}

void RUNNABLE::SetPrecedence() {
    
}