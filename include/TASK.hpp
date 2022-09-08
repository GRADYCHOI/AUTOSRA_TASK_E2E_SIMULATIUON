#ifndef __TASK_HPP__

#define __TASK_HPP__

#include "RUNNABLE.hpp"

#include <vector>
#include <memory>


class TASK : public std::enable_shared_from_this<TASK> {
private:
	// Inherent characters
    int id_ = -1;
    float period_ = -1.0;
    float offset_ = -1.0;
	
	// Dynamic characters
	int priority_ = -1;
	float executionTime_ = -1.0;
    std::vector<std::shared_ptr<RUNNABLE>> runnables_;
	
	void SetExecutionTime();

public:
    TASK(int id, float period, float offset) : id_(id), period_(period), offset_(offset) {}
    ~TASK() {}

    const int GetId() const { return id_; }
    const float GetPeriod() const { return period_; }
    const float GetOffset() const { return offset_; }
    
    const int GetPriority() const { return priority_; }
    const float GetExecutionTime() { return executionTime_; }
    const int GetNumberOfRunnables() { return static_cast<int>(runnables_.size()); }

    const std::vector<std::shared_ptr<RUNNABLE>>& GetRunnables() const { return runnables_; }
	const std::shared_ptr<RUNNABLE> GetRunnable(int index) const { return runnables_[index]; }
	
	void SetPriority(const int priority) { priority_ = priority; }

    void AddRunnable(const std::shared_ptr<RUNNABLE> runnable);
    void ClearMapping();

    std::shared_ptr<TASK> GetSharedPtr() { return shared_from_this(); }
};

#endif