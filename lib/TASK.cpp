#include "TASK.hpp"


void TASK::SetExecutionTime() {
	int tmpExecutionTime = 0;
	
	for (auto &runnable : this->runnables_) {
		tmpExecutionTime += runnable->GetExecutionTime();
	}
	
	this->executionTime_ = tmpExecutionTime;
}

void TASK::SetSequence(const std::vector<std::vector<std::shared_ptr<RUNNABLE>>>& sequenceMatrix) {
    std::vector<std::shared_ptr<RUNNABLE>> tmpSequenceMatrix;
    tmpSequenceMatrix.reserve(this->GetNumberOfRunnables());

    for (auto &samePrecedenceRunnables : sequenceMatrix) {
        for (auto &runnable : samePrecedenceRunnables) {
            auto destIter = std::find_if(this->runnables_.begin(), this->runnables_.end(), [&runnable](std::shared_ptr<RUNNABLE> a) { return a->id_ == runnable->id_; });

            if (destIter != this->runnables_.end()) {
                tmpSequenceMatrix.emplace_back(runnable);
            } else {
                throw std::invalid_argument("Call invalid Runnable in Task");
            }
        }
    }

    if (static_cast<int>(tmpSequenceMatrix.size()) == this->GetNumberOfRunnables()) {
        tmpSequenceMatrix.swap(this->runnables_);
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