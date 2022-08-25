#include "DAG.hpp"


class Mapping {
public:
    Mapping();
    ~Mapping();

    virtual void DoMapping(DAG * dag) = 0;
};