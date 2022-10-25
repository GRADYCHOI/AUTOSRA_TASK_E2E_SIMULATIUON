#ifndef __RUNNABLE__HPP__

#define __RUNNABLE__HPP__

#include <vector>
#include <memory>
#include "simulation_types.hpp"


class TASK;

class RUNNABLE : public std::enable_shared_from_this<RUNNABLE> { 
private:
	// Dynamic characters
    int status_ = 0; // 0 = input runnable, 1 = output runnable, 2 = middle runnable
	long long int precedence_ = -1;
    int maxCycle_ = -1;

    // relative Runnables
    std::vector<std::weak_ptr<RUNNABLE>> inputRunnables_;
    std::vector<std::shared_ptr<RUNNABLE>> outputRunnables_;
    
    // mapped Task
    std::weak_ptr<TASK> task_;

    void SetStatus() { status_ = (inputRunnables_.size()) ? ((outputRunnables_.size()) ? 2 : 1) : 0; }
	
    void AddInputRunnable(const std::weak_ptr<RUNNABLE> inputRunnable) { this->inputRunnables_.push_back(inputRunnable); this->SetStatus(); }
    void AddOutputRunnable(const std::shared_ptr<RUNNABLE> outputRunnable) { this->outputRunnables_.push_back(outputRunnable); this->SetStatus(); }

public:
	// Inherent characters
    const int id_;
    const int realId_;
    const long long int executionTime_; // ns (nano second)

    // time line
    std::vector<RequiredTime> executionTimes_;

    RUNNABLE(int id, int realId, long long int executionTime) : id_(id), realId_(realId), executionTime_((executionTime <= 0) ? 1 : executionTime) {}
    ~RUNNABLE() {}

    const int GetId() const { return id_; }
    const int GetRealId() const { return realId_; }
    const long long int GetExecutionTime() const { return executionTime_; }
    const int GetStatus() const { return status_; }
    const int GetPrecedence() const { return precedence_; }
    const int GetMaxCycle() const { return maxCycle_; }

    const std::shared_ptr<TASK> GetTask() { return task_.lock(); }
	
    const int GetNumberOfInputRunnables() { return static_cast<int>(inputRunnables_.size()); }
    const int GetNumberOfOutputRunnables() { return static_cast<int>(outputRunnables_.size()); }

    const std::shared_ptr<RUNNABLE> GetInputRunnable(int index) { return inputRunnables_[index].lock(); }
    const std::shared_ptr<RUNNABLE> GetOutputRunnable(int index) const { return outputRunnables_[index]; }

    const std::vector<std::shared_ptr<RUNNABLE>>& GetOutputRunnables() const { return outputRunnables_; }
	
	void SetPrecedence(const int precedence) { precedence_ = precedence; }
    void SetMaxCycle(const int maxCycle) { maxCycle_ = maxCycle; }

    void SetTask(const std::weak_ptr<TASK> task) { task_ = task; }

    void LinkInputRunnable(const std::weak_ptr<RUNNABLE> inputRunnable);
    void LinkOutputRunnable(const std::shared_ptr<RUNNABLE> outputRunnable);

    std::shared_ptr<RUNNABLE> GetSharedPtr() { return shared_from_this(); }
};

#endif