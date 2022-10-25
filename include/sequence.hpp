#ifndef __SEQUENCE__HPP__

#define __SEQUENCE__HPP__

#include "DAG.hpp"


class Sequence {
protected:
    std::shared_ptr<DAG> dag_;

    std::vector<std::vector<std::vector<std::shared_ptr<RUNNABLE>>>> sequenceMatrix_; // [task][precedence][runnable]

    virtual void SetSequenceMatrix();

public:
    Sequence(std::shared_ptr<DAG> newDag) : dag_(newDag) { SetSequenceMatrix(); }
    ~Sequence() {}

    void SetSequence(int caseIndex);
}

class SequenceByPrecedence : public Sequence {
private:
    void SetSequenceMatrix();

public:
    using Sequence::Sequence;

}

class SequenceByAllcase : public Sequence {
private:
    void SetSequenceMatrix();

public:
    using Sequence::Sequence;

}