#include "communication.hpp"


void Communication::InitializeMembers() {
    // Set unit time
    this->unit_ = this->dag_->GetTask(0)->period_;
    for (auto &task : this->dag_->GetTasksInPriority()) {
        this->unit_ = std::gcd(this->unit_, ((task->offset_ != 0) ? std::gcd(task->period_, task->offset_) : task->period_));
    }

    // Set number of cores
    this->numberOfCores_ = -1;
    for (auto &task : this->dag_->GetTasksInPriority()) {
        this->numberOfCores_ = (this->numberOfCores_ > task->GetCore()) ? this->numberOfCores_ : task->GetCore();
    }
    this->numberOfCores_ += 1;

    // Create time-line vector
    std::vector<int>(this->dag_->GetNumberOfRunnables(), this->unit_).swap(this->emptyTimes_);
}

void Communication::InitializeRunnables() {
    RequiredTime initializedRequiredTime = {-1LL, -1LL};

    for (auto &task : this->dag_->GetTasksInPriority()) {
        for (auto &runnable : task->GetRunnables()) {
            std::cout << "MaxCycle : " << runnable->GetMaxCycle() << "\n";
            std::vector<RequiredTime>(runnable->GetMaxCycle(), initializedRequiredTime).swap(runnable->executionTimes_);
        }
    }
}

void Communication::InitializeEmptyTimes() {
    std::fill(this->emptyTimes_.begin(), this->emptyTimes_.end(), this->unit_);
}

void RunnableImplicit::SetTimeTable() {
    // Seperate time-line by core
    for (int coreIndex = 0; coreIndex < this->numberOfCores_; coreIndex++) {
        // Reset time-line vector
        this->InitializeEmptyTimes();

        // Set Time-Table
        for (auto &task : this->dag_->GetTasksInPriority()) {
            if (task->GetCore() == coreIndex) {
                int maxCycle = static_cast<int>(this->dag_->GetHyperPeriod() / task->period_);

                for (int cycle = 0; cycle < maxCycle; cycle++) {
                    int unitIndex = ((task->period_ * cycle) + task->offset_) / this->unit_;

                    for (auto &runnable : task->GetRunnables()) {                  
                        // Regard time-line
                        while (this->emptyTimes_[unitIndex] == 0) unitIndex++;

                        // Set start time
                        runnable->executionTimes_[cycle].startTime = (static_cast<long long int>(unitIndex) * static_cast<long long int>(this->unit_)) + static_cast<long long int>(this->unit_ - this->emptyTimes_[unitIndex]);

                        int executionTime = runnable->executionTime_;
                        while (executionTime) {
                            if (this->emptyTimes_[unitIndex] < executionTime) {
                                executionTime -= this->emptyTimes_[unitIndex];
                                this->emptyTimes_[unitIndex] = 0;

                                unitIndex++;
                            } else {
                                runnable->executionTimes_[cycle].endTime = (static_cast<long long int>(unitIndex) * static_cast<long long int>(this->unit_)) + static_cast<long long int>(executionTime);
                                this->emptyTimes_[unitIndex] -= executionTime;
                                executionTime = 0;
                            }
                        }
                    }
                }
            }
        }
    }
}

void TaskImplicit::SetTimeTable() {
}

void LET::SetTimeTable() {
}