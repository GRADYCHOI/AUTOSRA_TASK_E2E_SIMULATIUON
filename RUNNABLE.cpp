#include "RUNNABLE.hpp"
#include<iostream>

using namespace std;

RUNNABLE::RUNNABLE(int id, double executionTime) {
    this->id = id;
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


int RUNNABLE::GetId() {
    return this->id;
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

void RUNNABLE::LinkInputRunnable(const std::weak_ptr<RUNNABLE> inputRunnable) {
    bool searchFlag = false; 
    std::shared_ptr<RUNNABLE> tmpRunnable = inputRunnable.lock(); // weak_ptr 특성

    if (tmpRunnable) {
        for (int count = 0; count < (int)this->inputRunnables.size(); count++) {
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
    
    for (int count = 0; count < (int)this->outputRunnables.size(); count++) {
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

void RUNNABLE::DisplayRunnable() {
    for (const auto &runnable : outputRunnables) {
        cout << runnable->GetId() << " ";
    } 
    cout << "      ";/*
    for (const auto &runnable : inputRunnables) {
        shared_ptr<RUNNABLE> tmpRunnable = runnable.lock();
        cout << tmpRunnable->GetId() << " ";
    }*/

}

void RUNNABLE::SetPrecedence() { // Set Runnable's precedence. 
    
}

void RUNNABLE::SetStatus(int status) {
    if (this->status < status) this->status = status;
}

std::shared_ptr<RUNNABLE> RUNNABLE::GetSharedPtr() {
    return shared_from_this();
}