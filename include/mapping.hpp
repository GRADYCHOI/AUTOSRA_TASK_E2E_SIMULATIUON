#ifndef __MAPPING__HPP__

#define __MAPPING__HPP__

#include "RUNNABLE.hpp"
#include "TASK.hpp"
#include <cstdlib>


class Mapping {
public:
    Mapping() {}
    virtual ~Mapping() {}

    virtual void DoMapping(std::vector<std::shared_ptr<TASK>>& tasks, std::vector<std::shared_ptr<RUNNABLE>>& runnables) = 0;
};

class RandomMapping : public Mapping {
public:
    void DoMapping(std::vector<std::shared_ptr<TASK>>& tasks, std::vector<std::shared_ptr<RUNNABLE>>& runnables);
};

#endif