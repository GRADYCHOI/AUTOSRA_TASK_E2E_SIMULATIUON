#include "RUNNABLE.hpp"
#include<iostream>

using namespace std;

RUNNABLE::RUNNABLE(int id, int realId, double executionTime) {
    this->id = id;
    this->realId = realId;
    this->executionTime = executionTime;
    if (executionTime == 0) this->executionTime = 0.001;
}

RUNNABLE::~RUNNABLE() {
    //std::cout << "Delete Runnable ID : " << this->id << ", Execution Time : " << this->executionTime << "ms, Status : " << this->status << std::endl;
}
// 0 = input runnable, 1 = output runnable, 2 = middle runnable
void RUNNABLE::SetStatus() {
    this->status = -1;

    if (this->inputRunnables.size()) this->status += 2;
    if (this->outputRunnables.size()) this->status += 1;
}

void RUNNABLE::AddInputRunnable(const std::weak_ptr<RUNNABLE> inputRunnable) {
    this->inputRunnables.push_back(inputRunnable);
    this->SetStatus();
}

void RUNNABLE::AddOutputRunnable(const std::shared_ptr<RUNNABLE> outputRunnable) {
    this->outputRunnables.push_back(outputRunnable);
    this->SetStatus();
}

int RUNNABLE::GetRealId() const {
    return this->realId;
}

int RUNNABLE::GetId() const {
    return this->id;
}

double RUNNABLE::GetExecutionTime() const {
    return this->executionTime;
}

int RUNNABLE::GetStatus() const {
    return this->status;
}

int RUNNABLE::GetNumberOfOutputRunnables() {
    return (int)this->outputRunnables.size();
}

int RUNNABLE::GetNumberOfInputRunnables() {
    return (int)this->inputRunnables.size();
}

const std::vector<std::shared_ptr<RUNNABLE>> RUNNABLE::GetInputRunnable(int index) {
    return inputRunnables[index].lock();
}

const std::vector<std::shared_ptr<RUNNABLE>> RUNNABLE::GetInputRunnables() {
    std::vector<std::shared_ptr<RUNNABLE>> tmpRunnables;
    
    for (auto &inputRunnable : this->inputRunnables) {
        tmpRunnables.push_back(inputRunnable.lock());
    }

    return tmpRunnables;
}

const std::shared_ptr<RUNNABLE> RUNNABLE::GetOutputRunnable(int index) const {
    return this->outputRunnables[index];
}

const std::vector<std::shared_ptr<RUNNABLE>> RUNNABLE::GetOutputRunnables() const {
    return this->outputRunnables;
}

void RUNNABLE::LinkInputRunnable(const std::weak_ptr<RUNNABLE> inputRunnable) {
    bool searchFlag = false; 
    std::shared_ptr<RUNNABLE> tmpRunnable = inputRunnable.lock(); // weak_ptr 특성

    if (tmpRunnable) {
        int numberOfInputRunnable = this->GetNumberOfInputRunnables();

        for (int count = 0; count < numberOfInputRunnable; count++) {
            std::shared_ptr<RUNNABLE> tmpRunnable2 = this->inputRunnables[count].lock();
            if (tmpRunnable2->GetId() == tmpRunnable->GetId()) {
                searchFlag = true;
                break;
            }
        }

        if (!searchFlag) {
            this->AddInputRunnable(inputRunnable);
            tmpRunnable->AddOutputRunnable(this->GetSharedPtr());
        }
    }
}

void RUNNABLE::LinkOutputRunnable(const std::shared_ptr<RUNNABLE> outputRunnable) {
    bool searchFlag = false;
    int numberOfOutputRunnables = this->GetNumberOfOutputRunnables();
    
    for (int count = 0; count < numberOfOutputRunnables; count++) {
        if (this->outputRunnables[count]->GetId() == outputRunnable->GetId()) {
            searchFlag = true;
            break;
        }
    }

    if (!searchFlag) {
        this->AddOutputRunnable(outputRunnable);
        outputRunnable->AddInputRunnable(this->GetSharedPtr());
    }
}

void RUNNABLE::DisplayOutputRunnables() {
    for (const auto &runnable : this->outputRunnables) {
        std::cout << runnable->GetId() << " ";
    }
}

void RUNNABLE::SetStatus(int status) {
    if (this->status < status) {
        this->status = status;
    }
}

std::shared_ptr<RUNNABLE> RUNNABLE::GetSharedPtr() {
    return shared_from_this();
}