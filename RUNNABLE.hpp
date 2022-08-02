#include <iostream>
#include <stdlib.h>
#include <vector>
#include <memory>


class RUNNABLE 
{ 
private:
    int id;
    double executionTime = -1;
    int precedence = -1;
    int status = -1; // 0 = input runnable, 1 = middle runnable, 2 = output runnable
    std::vector<std::shared_ptr<RUNNABLE>> inputRunnables;
    std::vector<std::shared_ptr<RUNNABLE>> outputRunnables;

public:
    RUNNABLE();
    RUNNABLE(int id, double time);
    ~RUNNABLE();

    double GetExecutionTime();
    int GetPrecedence();
    int GetStatus();
    void AddInputRunnable(const std::shared_ptr<RUNNABLE> &inputRunnable);
    void AddOutputRunnable(const std::shared_ptr<RUNNABLE> &outputRunnable);

    void SetPrecedence();
};
