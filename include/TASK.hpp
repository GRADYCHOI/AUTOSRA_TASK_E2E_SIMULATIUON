#ifndef __TASK_HPP__

#define __TASK_HPP__

#include "RUNNABLE.hpp"

#include <vector>
#include <memory>


class TASK {
private:
	// Inherent characters
    int id_ = -1;
    double period_ = -1.0;
    double offset_ = -1.0;
	
	// Dynamic characters
	double executionTime_ = -1.0;
    std::vector<std::shared_ptr<RUNNABLE>> runnables_;
	
	void SetExecutionTime();

public:
    TASK(int id, double period, double offset) : id_(id), period_(period), offset_(offset) {}
    ~TASK() {}

    const int GetId() const { return id_; }
    const double GetPeriod() const { return period_; }
    const double GetOffset() const { return offset_; }
    
    const double GetExecutionTime() { return executionTime_; }
    const int GetNumberOfRunnables() { return static_cast<int>(runnables_.size()); }

    const std::vector<std::shared_ptr<RUNNABLE>> GetRunnables() const { return runnables_; }
	const std::shared_ptr<RUNNABLE> GetRunnable(int index) const { return runnables_[index]; }

    void AddRunnable(const std::shared_ptr<RUNNABLE> runnable);
    void ClearMapping();
};

#endif