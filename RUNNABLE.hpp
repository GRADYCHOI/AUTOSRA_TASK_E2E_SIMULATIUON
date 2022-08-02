#include <iostream>
#include <stdlib.h>
#include <vector>
#include <memory>


class RUNNABLE : public std::enable_shared_from_this<RUNNABLE>
{ 
private:
    int id;
    double executionTime = -1;
    int precedence = -1;
    int status = -1; // 0 = input runnable, 1 = output runnable, 2 = middle runnable
    std::vector<std::weak_ptr<RUNNABLE>> inputRunnables;
    std::vector<std::shared_ptr<RUNNABLE>> outputRunnables;

    void SetStatus();

    void AddInputRunnable(const std::weak_ptr<RUNNABLE>& inputRunnable);
    void AddOutputRunnable(const std::shared_ptr<RUNNABLE>& outputRunnable);

public:
    RUNNABLE();
    RUNNABLE(int id, double time);
    ~RUNNABLE();

    double GetExecutionTime();
    int GetPrecedence();
    int GetStatus();
    //void AddInputRunnable(const std::shared_ptr<RUNNABLE> &inputRunnable);
    //void AddOutputRunnable(const std::shared_ptr<RUNNABLE> &outputRunnable);

    void LinkInputRunnable(const std::weak_ptr<RUNNABLE>& inputRunnable);
    void LinkOutputRunnable(const std::shared_ptr<RUNNABLE>& outputRunnable);

    void SetPrecedence();

    std::shared_ptr<RUNNABLE> GetSharedPtr();
};
