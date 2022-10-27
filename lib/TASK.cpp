#include "TASK.hpp"


void TASK::SetExecutionTime() {
	long long int tmpExecutionTime = 0;
	
	for (auto &runnable : this->runnables_) {
		tmpExecutionTime += runnable->GetExecutionTime();
	}
	
	this->executionTime_ = tmpExecutionTime;
}

void TASK::SetSequence(const std::vector<std::shared_ptr<RUNNABLE>>& sequence) {
    // 검증 부분이 매우 취약하지만, 속도를 위해 포기함.
    if (static_cast<int>(sequence.size()) == this->GetNumberOfRunnables()) {
        std::vector<std::shared_ptr<RUNNABLE>>().swap(this->runnablesInSequence_);
        this->runnablesInSequence_.assign(sequence.begin(), sequence.end());
    } else {
        throw std::invalid_argument("Wrong Runnable in a Task");
    }
}

void TASK::AddRunnable(const std::shared_ptr<RUNNABLE> runnable) { 
    auto runnableIter = std::find_if(this->runnables_.begin(), this->runnables_.end(), [&runnable](std::shared_ptr<RUNNABLE> a) { return a->id_ == runnable->id_; });

    if (runnableIter == this->runnables_.end()) {
        this->runnables_.push_back(runnable);
        runnable->SetTask(this->GetSharedPtr());

        this->SetExecutionTime();
    }
}

void TASK::ClearMapping() {
    std::vector<std::shared_ptr<RUNNABLE>>().swap(this->runnables_);
	
    this->priority_ = -1;
	this->executionTime_ = -1;
    this->core_ = -1;
}