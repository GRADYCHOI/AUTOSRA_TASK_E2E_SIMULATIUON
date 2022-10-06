#ifndef __PRECEDENCE__HPP__

#define __PRECEDENCE__HPP__

#include "RUNNABLE.hpp"
#include "TASK.hpp"
#include <cstdlib>
#include <iostream>


class Precedence {
public:
    Precedence() {}
    virtual ~Precedence() {}

    void ResetInputOutputRunnablePrecedence();

    virtual void CheckPrecedence(std::vector<std::shared_ptr<TASK>>& tasks, std::vector<std::shared_ptr<RUNNABLE>>& runnables) = 0;
    virtual void 
};

class InputToOutputPrecedence : public Precedence {
public:
    void CheckPrecedence(std::vector<std::shared_ptr<TASK>>& tasks, std::vector<std::shared_ptr<RUNNABLE>>& runnables);
};

class OutputToInputPrecedence : public Precedence {
public:
    void CheckPrecedence(std::vector<std::shared_ptr<TASK>>& tasks, std::vector<std::shared_ptr<RUNNABLE>>& runnables);
};

#endif