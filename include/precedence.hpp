#ifndef __PRECEDENCE__HPP__

#define __PRECEDENCE__HPP__

#include "RUNNABLE.hpp"
#include "TASK.hpp"
#include <cstdlib>
#include <iostream>


class Precedence {
protected:
    void SetInputOutputRunnablePrecedence(std::vector<std::shared_ptr<RUNNABLE>>& inputRunnables, std::vector<std::shared_ptr<RUNNABLE>>& outputRunnables);
    virtual void CheckPrecedence(const std::shared_ptr<RUNNABLE>& runnable, int precedence) = 0;

public:
    Precedence() {}
    virtual ~Precedence() {}

    virtual void SetPrecedence(std::vector<std::shared_ptr<RUNNABLE>>& runnables, std::vector<std::shared_ptr<RUNNABLE>>& inputRunnables, std::vector<std::shared_ptr<RUNNABLE>>& outputRunnables) = 0;
};

class InputToOutputPrecedence : public Precedence {
private:
    void CheckPrecedence(const std::shared_ptr<RUNNABLE>& runnable, int precedence);

public:
    void SetPrecedence(std::vector<std::shared_ptr<RUNNABLE>>& runnables, std::vector<std::shared_ptr<RUNNABLE>>& inputRunnables, std::vector<std::shared_ptr<RUNNABLE>>& outputRunnables);
};

class OutputToInputPrecedence : public Precedence {
private:
    void CheckPrecedence(const std::shared_ptr<RUNNABLE>& runnable, int precedence);

public:
    void SetPrecedence(std::vector<std::shared_ptr<RUNNABLE>>& runnables, std::vector<std::shared_ptr<RUNNABLE>>& inputRunnables, std::vector<std::shared_ptr<RUNNABLE>>& outputRunnables);
};

#endif