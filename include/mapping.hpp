#ifndef __MAPPING__HPP__

#define __MAPPING__HPP__

#include "RUNNABLE.hpp"
#include "TASK.hpp"
#include <cstdlib>
#include <iostream>


class Mapping {
public:
    Mapping() {}
    virtual ~Mapping() { std::clog << "Delete Mapping Class" << std::endl; }

    virtual void DoMapping(std::vector<std::shared_ptr<TASK>>& tasks, std::vector<std::shared_ptr<RUNNABLE>>& runnables) = 0;
};

class RandomMapping : public Mapping {
public:
    void DoMapping(std::vector<std::shared_ptr<TASK>>& tasks, std::vector<std::shared_ptr<RUNNABLE>>& runnables);
};

class RateMapping : public Mapping {
public:
    void DoMapping(std::vector<std::shared_ptr<TASK>>& tasks, std::vector<std::shared_ptr<RUNNABLE>>& runnables);
};

class InputMapping : public Mapping {
public:
    void DoMapping(std::vector<std::shared_ptr<TASK>>& tasks, std::vector<std::shared_ptr<RUNNABLE>>& runnables);
};

#endif