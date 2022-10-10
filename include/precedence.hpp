#ifndef __PRECEDENCE__HPP__

#define __PRECEDENCE__HPP__

#include "RUNNABLE.hpp"
#include "TASK.hpp"
#include <cstdlib>
#include <iostream>


class Precedence {
protected:
    void SetInputOutputRunnablePrecedence();
    virtual void CheckPrecedence(std::shared_ptr<RUNNABLE>& runnable, int precedence) = 0;

public:
    Precedence() {}
    virtual ~Precedence() {}

    virtual void SetPrecedence(std::vector<RUNNABLE>& runnables, std::vector<RUNNABLE>& inputRunnables, std::vector<RUNNABLE>& outputRunnables) = 0;
};

class InputToOutputPrecedence : public Precedence {
private:
    void CheckPrecedence(std::shared_ptr<RUNNABLE>& runnable, int precedence);

public:
    void SetPrecedence(std::vector<RUNNABLE>& runnables, std::vector<RUNNABLE>& inputRunnables, std::vector<RUNNABLE>& outputRunnables);
};

class OutputToInputPrecedence : public Precedence {
private:
    void CheckPrecedence(std::shared_ptr<RUNNABLE>& runnable, int precedence);

public:
    void SetPrecedence(std::vector<RUNNABLE>& runnables, std::vector<RUNNABLE>& inputRunnables, std::vector<RUNNABLE>& outputRunnables);
};

#endif