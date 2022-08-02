#include "RUNNABLE.hpp"
#include<iostream>

using namespace std;

RUNNABLE::RUNNABLE(int id, double executionTime) {
    this->id = id;
    this->executionTime = executionTime;
}

RUNNABLE::~RUNNABLE() {
    std::cout << "Delete Runnable ID : " << this->id << ", Execution Time : " << this->executionTime << "ms."<< std::endl;
}

void RUNNABLE::SetStatus() {
    this->status = -1;

    if (this->inputRunnables.size()) this->status += 1;
    if (this->outputRunnables.size()) this->status += 2;
}

void RUNNABLE::AddInputRunnable(const std::weak_ptr<RUNNABLE>& inputRunnable) {
    this->inputRunnables.push_back(inputRunnable);
}

void RUNNABLE::AddOutputRunnable(const std::shared_ptr<RUNNABLE>& outputRunnable) {
    this->outputRunnables.push_back(outputRunnable);
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

void RUNNABLE::LinkInputRunnable(const std::weak_ptr<RUNNABLE>& inputRunnable) {
    bool searchFlag = false; 
    std::shared_ptr<RUNNABLE> tmpRunnable = inputRunnable.lock(); // weak_ptr 특성

    if (tmpRunnable) {
        for (int count = 0; 0 < this->inputRunnables.size(); count++) {
            if (this->inputRunnables[count].id == tmpRunnable->id) {
                searchFlag = true;
                break;
            }
        }

        if (!searchFlag) {
            this->AddInputRunnable(inputRunnable);
            tmpRunnable->AddOutputRunnable(this->GetSharedPtr());
        }

        this->SetStatus();
    }
}

void RUNNABLE::LinkOutputRunnable(const std::shared_ptr<RUNNABLE>& outputRunnable) {
    bool searchFlag = false;
    
    for (int count = 0; 0 < this->outputRunnables.size(); count++) {
        if (this->outputRunnables[count].id == outputRunnable->id) {
            searchFlag = true;
            break;
        }
    }

    if (!searchFlag) {
        this->AddOutputRunnable(outputRunnable);
        outputRunnable->AddInputRunnable(this->GetSharedPtr());
    }

    this->SetStatus();
}


void RUNNABLE::SetPrecedence() {
    
}
/*
std::shared_ptr<RUNNABLE> GetSharedPtr() {
    return shared_from_this();
}*/