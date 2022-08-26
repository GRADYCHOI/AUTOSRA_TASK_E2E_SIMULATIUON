#ifndef __MAPPING__HPP__

#define __MAPPING__HPP__

#include "RUNNABLE.hpp"
#include "TASK.hpp"


class Mapping {
public:
    Mapping() = default;
    virtual ~Mapping() = default;

    virtual void DoMapping(std::vector<std::shared_ptr<TASK>>& tasks, std::vector<std::shared_ptr<RUNNABLE>>& runnables) = 0;
};

#endif