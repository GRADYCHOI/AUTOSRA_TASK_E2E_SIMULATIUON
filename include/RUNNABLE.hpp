#ifndef __RUNNABLE__HPP__

#define __RUNNABLE__HPP__

#include <vector>
#include <memory>


class RUNNABLE : public std::enable_shared_from_this<RUNNABLE>
{ 
private:
	// Inherent characters
    int id_ = -1;
    int realId_ = -1;
    double executionTime_ = -1.0;
    int status_ = 0; // 0 = input runnable, 1 = output runnable, 2 = middle runnable
	
	// Dynamic characters
	int precedence_ = -1;
    std::vector<std::weak_ptr<RUNNABLE>> inputRunnables;
    std::vector<std::shared_ptr<RUNNABLE>> outputRunnables;

    void SetStatus() { status = (outputRunnables.size()) ? ((inputRunnables.size()) ? 2 : 1) : 0; }
	
    void AddInputRunnable(const std::weak_ptr<RUNNABLE> inputRunnable) { this->inputRunnables_.push_back(inputRunnable); this->SetStatus(); }
    void AddOutputRunnable(const std::shared_ptr<RUNNABLE> outputRunnable) { this->outputRunnables_.push_back(outputRunnable); this->SetStatus(); }

public:
    RUNNABLE(int id, int realId, double executionTime) : id(id), realId(realId), executionTime((executionTime == 0.0) ? 0.01 : executionTime) {}
    ~RUNNABLE() {}

    const int GetId() const { return id_; }
    const int GetRealId() const { return realId_; }
    double GetExecutionTime() const { return executionTime_; }
    const int GetStatus() const { return status_; }
	
	const int GetPrecedence() const { return precedence_; }
    const int GetNumberOfInputRunnables() { return static_cast<int>(inputRunnables.size()); }
    const int GetNumberOfOutputRunnables() { return static_cast<int>(outputRunnables.size()); }

    const std::shared_ptr<RUNNABLE> GetInputRunnable(int index) { return intputRunnable[index].lock(); }
    const std::vector<std::shared_ptr<RUNNABLE>> GetInputRunnables();

    const std::shared_ptr<RUNNABLE> GetOutputRunnable(int index) const { return outputRunnables[index]; }
    const std::vector<std::shared_ptr<RUNNABLE>>& GetOutputRunnables() const { return outputRunnables; }
	
	void SetPrecedence(const int precedence) { precedence_ = precedence; }

    void LinkInputRunnable(const std::weak_ptr<RUNNABLE> inputRunnable);
    void LinkOutputRunnable(const std::shared_ptr<RUNNABLE> outputRunnable);

    std::shared_ptr<RUNNABLE> GetSharedPtr();
};

#endif