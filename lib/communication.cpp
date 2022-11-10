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
    std::vector<int>(this->dag_->GetHyperPeriod() / this->unit_, this->unit_).swap(this->emptyTimes_);
}

void Communication::InitializeRunnables() {
    RequiredTime initializedRequiredTime = {-1LL, -1LL};

    for (auto &task : this->dag_->GetTasksInPriority()) {
        for (auto &runnable : task->GetRunnables()) {
            std::vector<RequiredTime>(runnable->GetMaxCycle(), initializedRequiredTime).swap(runnable->executionTimes_);
        }
    }
}

void Communication::InitializeEmptyTimes() {
    std::fill(this->emptyTimes_.begin(), this->emptyTimes_.end(), this->unit_);
}

void worker(int cycle, std::shared_ptr<TASK> task, std::vector<int>::iterator emptyTimesBegin, int unit) {
    int unitIndex = static_cast<int>(((static_cast<long long int>(task->period_) * static_cast<long long int>(cycle)) + static_cast<long long int>(task->offset_)) / static_cast<long long int>(unit));

    for (auto &runnable : task->GetRunnablesInSequence()) {                  
        // Regard time-line
        while (*(emptyTimesBegin + unitIndex) == 0) unitIndex++;

        // Set start time
        runnable->executionTimes_[cycle].startTime = (static_cast<long long int>(unitIndex) * static_cast<long long int>(unit)) + static_cast<long long int>(unit - *(emptyTimesBegin + unitIndex));

        int executionTime = runnable->executionTime_;
        while (executionTime) {
            if (*(emptyTimesBegin + unitIndex) < executionTime) {
                executionTime -= *(emptyTimesBegin + unitIndex);
                *(emptyTimesBegin + unitIndex) = 0;

                unitIndex++;
            } else {
                runnable->executionTimes_[cycle].endTime = (static_cast<long long int>(unitIndex) * static_cast<long long int>(unit)) + static_cast<long long int>(executionTime);
                *(emptyTimesBegin + unitIndex) -= executionTime;
                executionTime = 0;
            }
        }
    }
}


// TODO : 최대 thread 개수에 따른 실험 결과 비교 구축
void RunnableImplicit::SetTimeTable() {
    std::clock_t simulationStart = std::clock();

    // Seperate time-line by core
    for (int coreIndex = 0; coreIndex < this->numberOfCores_; coreIndex++) {
        // Reset time-line vector
        this->InitializeEmptyTimes();

        // Set Time-Table
        for (auto &task : this->dag_->GetTasksInPriority()) {
            if (task->GetCore() == coreIndex) {
                int maxCycle = static_cast<int>(this->dag_->GetHyperPeriod() / task->period_);

                std::clock_t simulationStart = std::clock();

                std::vector<std::future<void>> workers;
                for (int cycle = 0; cycle < maxCycle; cycle++) {
                    workers.emplace_back(std::async(std::launch::async, worker, cycle, task, this->emptyTimes_.begin(), this->unit_ ));
                }

                std::clock_t simulationEnd1 = std::clock();

                for (int cycle = 0; cycle < maxCycle; cycle++) {
                    workers[cycle].get();
                    //std::cout << "thread free : " << threadIndex << "\n";
                }

                std::clock_t simulationEnd2 = std::clock();

                std::cout << "Base Communication : " << static_cast<double>(simulationEnd1 - simulationStart) / CLOCKS_PER_SEC << "ms, Thread Communication : " << static_cast<double>(simulationEnd2 - simulationEnd1) / CLOCKS_PER_SEC << std::endl;
            }
        }
    }

    std::cout << "communication calc time : " << static_cast<double>(std::clock() - simulationStart) / CLOCKS_PER_SEC << " s." << "\n";
}


// void RunnableImplicit::SetTimeTable() {
//     std::clock_t simulationStart = std::clock();
    
//     // Seperate time-line by core
//     for (int coreIndex = 0; coreIndex < this->numberOfCores_; coreIndex++) {
//         // Reset time-line vector
//         this->InitializeEmptyTimes();

//         // Set Time-Table
//         for (auto &task : this->dag_->GetTasksInPriority()) {
//             if (task->GetCore() == coreIndex) {
//                 int maxCycle = static_cast<int>(this->dag_->GetHyperPeriod() / task->period_);

//                 for (int cycle = 0; cycle < maxCycle; cycle++) {
//                     int unitIndex = static_cast<int>(((static_cast<long long int>(task->period_) * static_cast<long long int>(cycle)) + static_cast<long long int>(task->offset_)) / static_cast<long long int>(this->unit_));

//                     for (auto &runnable : task->GetRunnablesInSequence()) {                  
//                         // Regard time-line
//                         while (this->emptyTimes_[unitIndex] == 0) unitIndex++;

//                         // Set start time
//                         runnable->executionTimes_[cycle].startTime = (static_cast<long long int>(unitIndex) * static_cast<long long int>(this->unit_)) + static_cast<long long int>(this->unit_ - this->emptyTimes_[unitIndex]);

//                         int executionTime = runnable->executionTime_;
//                         while (executionTime) {
//                             if (this->emptyTimes_[unitIndex] < executionTime) {
//                                 executionTime -= this->emptyTimes_[unitIndex];
//                                 this->emptyTimes_[unitIndex] = 0;

//                                 unitIndex++;
//                             } else {
//                                 runnable->executionTimes_[cycle].endTime = (static_cast<long long int>(unitIndex) * static_cast<long long int>(this->unit_)) + static_cast<long long int>(executionTime);
//                                 this->emptyTimes_[unitIndex] -= executionTime;
//                                 executionTime = 0;
//                             }
//                         }
//                     }
//                 }
//             }
//         }
//     }

//     std::cout << "communication calc time : " << static_cast<double>(std::clock() - simulationStart) / CLOCKS_PER_SEC << " s." << std::endl;
// }

void TaskImplicit::SetTimeTable() {  
    // Seperate time-line by core
    for (int coreIndex = 0; coreIndex < this->numberOfCores_; coreIndex++) {
        // Reset time-line vector
        this->InitializeEmptyTimes();

        // Set Time-Table
        for (auto &task : this->dag_->GetTasksInPriority()) {
            if (task->GetCore() == coreIndex) {
                int maxCycle = static_cast<int>(this->dag_->GetHyperPeriod() / task->period_);

                for (int cycle = 0; cycle < maxCycle; cycle++) {
                    int unitIndex = static_cast<int>(((static_cast<long long int>(task->period_) * static_cast<long long int>(cycle)) + static_cast<long long int>(task->offset_)) / static_cast<long long int>(this->unit_));

                    // Regard time-line
                    while (this->emptyTimes_[unitIndex] == 0) unitIndex++;

                    // Set start time
                    for (auto &runnable : task->GetRunnablesInSequence()) {    
                        runnable->executionTimes_[cycle].startTime = (static_cast<long long int>(unitIndex) * static_cast<long long int>(this->unit_)) + static_cast<long long int>(this->unit_ - this->emptyTimes_[unitIndex]);
                    }

                    // Set end time
                    long long int executionTime = task->GetExecutionTime();
                    while (executionTime) {
                        if (this->emptyTimes_[unitIndex] < executionTime) {
                            executionTime -= this->emptyTimes_[unitIndex];
                            this->emptyTimes_[unitIndex] = 0;

                            unitIndex++;
                        } else {
                            for (auto &runnable : task->GetRunnablesInSequence()) {  
                                runnable->executionTimes_[cycle].endTime = (static_cast<long long int>(unitIndex) * static_cast<long long int>(this->unit_)) + static_cast<long long int>(executionTime);
                            }
                            this->emptyTimes_[unitIndex] -= executionTime;
                            executionTime = 0;
                        }
                    }
                }
            }
        }
    }
}

void LET::SetTimeTable() {
    // Seperate time-line by core
    for (int coreIndex = 0; coreIndex < this->numberOfCores_; coreIndex++) {
        // Reset time-line vector
        this->InitializeEmptyTimes();

        // Set Time-Table
        for (auto &task : this->dag_->GetTasksInPriority()) {
            if (task->GetCore() == coreIndex) {
                int maxCycle = static_cast<int>(this->dag_->GetHyperPeriod() / task->period_);

                for (int cycle = 0; cycle < maxCycle; cycle++) {
                    // Set start & end time
                    for (auto &runnable : task->GetRunnablesInSequence()) {    
                        runnable->executionTimes_[cycle].startTime = (static_cast<long long int>(task->period_) * static_cast<long long int>(cycle)) + static_cast<long long int>(task->offset_);
                        runnable->executionTimes_[cycle].startTime = (static_cast<long long int>(task->period_) * static_cast<long long int>(cycle + 1)) + static_cast<long long int>(task->offset_);
                    }
                }
            }
        }
    }
}