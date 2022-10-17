#include "RUNNABLE.hpp"


const std::vector<std::shared_ptr<RUNNABLE>> RUNNABLE::GetInputRunnables() {
    std::vector<std::shared_ptr<RUNNABLE>> tmpRunnables;
    tmpRunnables.reserve(this->GetNumberOfInputRunnables());

    for (auto &inputRunnable : this->inputRunnables_) {
        tmpRunnables.push_back(inputRunnable.lock());
    }

    return tmpRunnables;
}

void RUNNABLE::LinkInputRunnable(const std::weak_ptr<RUNNABLE> inputRunnable) {
    bool searchFlag = false; 
    std::shared_ptr<RUNNABLE> tmpRunnable = inputRunnable.lock(); // weak_ptr 특성

    if (tmpRunnable) {
        for (auto &existInputRunnable : this->inputRunnables_) {
            if (tmpRunnable->GetId() == (existInputRunnable.lock())->GetId()) {
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
    
    for (auto &existOutputRunnable : this->outputRunnables_) {
        if (outputRunnable->GetId() == existOutputRunnable->GetId()) {
            searchFlag = true;
            break;
        }
    }

    if (!searchFlag) {
        this->AddOutputRunnable(outputRunnable);
        outputRunnable->AddInputRunnable(this->GetSharedPtr());
    }
}