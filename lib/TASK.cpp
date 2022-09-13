#include "TASK.hpp"


void TASK::SetExecutionTime() {
	int tmpExecutionTime = 0;
	
	for (auto &runnable : this->runnables_) {
		tmpExecutionTime += runnable->GetExecutionTime();
	}
	
	this->executionTime_ = tmpExecutionTime;
}

const std::vector<std::shared_ptr<RUNNABLE>> TASK::GetRunnablesByPriorityInTask() {
    std::vector<std::shared_ptr<RUNNABLE>> tmpRunnables(this->GetNumberOfRunnables());
    std::copy(this->runnables_.begin(), this->runnables_.end(), tmpRunnables.begin());

    std::sort(tmpRunnables.begin(), tmpRunnables.end(), [](std::shared_ptr<RUNNABLE> a, std::shared_ptr<RUNNABLE> b) { return a->GetPriorityInTask() < b->GetPriorityInTask(); });

    return tmpRunnables;
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