#ifndef __TASK_HPP__

#define __TASK_HPP__

#include "RUNNABLE.hpp"

#include <vector>
#include <memory>
#include <algorithm>


class TASK : public std::enable_shared_from_this<TASK> {
private:
	// Inherent characters
    int id_ = -1;
    int period_ = -1;
    int offset_ = -1;
	
	// Dynamic characters
	int priority_ = -1;
	int executionTime_ = -1.0;
    std::vector<std::shared_ptr<RUNNABLE>> runnables_;
	
	void SetExecutionTime();

public:
    TASK(int id, int period, int offset) : id_(id), period_(period), offset_(offset) {}
    ~TASK() {}

    const int GetId() const { return id_; }
    const int GetPeriod() const { return period_; }
    const int GetOffset() const { return offset_; }
    
    const int GetPriority() const { return priority_; }
    const int GetExecutionTime() { return executionTime_; }
    const int GetNumberOfRunnables() { return static_cast<int>(runnables_.size()); }

    const std::vector<std::shared_ptr<RUNNABLE>>& GetRunnables() const { return runnables_; }
	const std::shared_ptr<RUNNABLE> GetRunnable(int index) const { return runnables_[index]; }

    const std::vector<std::shared_ptr<RUNNABLE>> GetRunnablesByPriorityInTask();
	
	void SetPriority(const int priority) { priority_ = priority; }

    void AddRunnable(const std::shared_ptr<RUNNABLE> runnable);
    void ClearMapping();

    std::shared_ptr<TASK> GetSharedPtr() { return shared_from_this(); }

    void SortRunnables() { std::sort(runnables_.begin(), runnables_.end(), [](std::shared_ptr<RUNNABLE> a, std::shared_ptr<RUNNABLE> b) { return a->GetId() < b->GetId(); }); }
};

#endif