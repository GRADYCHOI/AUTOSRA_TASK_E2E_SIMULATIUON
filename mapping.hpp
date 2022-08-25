#ifndef __MAPPING__HPP__

#define __MAPPING__HPP__

#include "DAG.hpp"


class Mapping {
public:
    Mapping();
    ~Mapping();

    virtual void DoMapping() = 0;
};

#endif