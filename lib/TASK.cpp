#include "TASK.hpp"


void TASK::SetExecutionTime() {
	int tmpExecutionTime = 0;
	
	for (auto &runnable : this->runnables_) {
		tmpExecutionTime += runnable->GetExecutionTime();
	}
	
	this->executionTime_ = tmpExecutionTime;
}

void TASK::SortRunnables(const std::vector<int>& sequence) {
    int index = 0;

    for (auto currentIter = this->runnables_.begin(); currentIter < this->runnables_.end(); currentIter++) {
        int runnableId = sequence[index];
        auto destIter = std::find_if(currentIter, this->runnables_.end(), [runnableId](std::shared_ptr<RUNNABLE> a) { return a->id_ == runnableId; });

        if (destIter == currentIter) {
            ++index;
            continue;
        } else if (destIter != this->runnables_.end()) {
            std::iter_swap(currentIter, destIter);
            ++index;
            continue;
        } else {
            throw std::invalid_argument("Call invalid Runnable in Task");
        }
    }
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

    this->SetExecutionTime();
}

void TASK::ClearMapping() {
    std::vector<std::shared_ptr<RUNNABLE>> tmpRunnables;
    tmpRunnables.swap(this->runnables_);
	
    this->priority_ = -1;
	this->executionTime_ = -1.0;
    this->core_ = -1;
}