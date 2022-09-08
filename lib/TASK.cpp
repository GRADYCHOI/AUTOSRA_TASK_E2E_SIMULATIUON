#include "TASK.hpp"


void TASK::SetExecutionTime() {
	int tmpExecutionTime = 0;
	
	for (auto &runnable : this->runnables_) {
		tmpExecutionTime += runnable->GetExecutionTime();
	}
	
	this->executionTime_ = tmpExecutionTime;
}

void TASK::AddRunnable(const std::shared_ptr<RUNNABLE> runnable) {
    bool searchFlag = false;
    
    for (auto &existRunnable : this->runnables_) {
        if (runnable->GetId() == existRunnable->GetId()) {
            searchFlag = true;
            break;
        }
    }

    if (!searchFlag) {
        this->runnables_.push_back(runnable);
        runnable->SetTask(this->GetSharedPtr());
    }

    SetExecutionTime();
}

void TASK::ClearMapping() {
    std::vector<std::shared_ptr<RUNNABLE>> tmpRunnables;
    tmpRunnables.swap(this->runnables_);
	
	this->executionTime_ = -1.0;
}