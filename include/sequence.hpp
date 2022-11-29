#ifndef __SEQUENCE__HPP__

#define __SEQUENCE__HPP__

#include <random>
#include <math.h>
#include "DAG.hpp"


class Sequence {
protected:
    std::shared_ptr<DAG> dag_;

    int numberOfCase_ = -1;
    int numberOfRemainedCase_ = -1;

    std::vector<bool> visitedPermutationNumber_;
    std::map<std::shared_ptr<TASK>, std::vector<std::vector<std::shared_ptr<RUNNABLE>>>> sequenceMatrix_; // [task][precedence][runnable]

    void InitializeVisitedPermutationNumber();
    virtual void SetSequenceMatrix() = 0;

public:
    Sequence(std::shared_ptr<DAG> newDag) : dag_(newDag) {}
    virtual ~Sequence() { std::clog << "Delete Sequence Class" << std::endl; }

    virtual void SetSequence(int caseIndex);

    int GetNumberOfCase();
    int GetNumberOfRemainedCase() const { return numberOfRemainedCase_; }
    virtual int GetRandomEmptyIndex();
};

class SequenceByPrecedence : public Sequence {
protected:
    void SetSequenceMatrix();

public:
    SequenceByPrecedence(std::shared_ptr<DAG> newDag) : Sequence(newDag) { SetSequenceMatrix(); InitializeVisitedPermutationNumber(); }
};

class SequenceByAllcase : public Sequence {
protected:
    void SetSequenceMatrix();

public:
    SequenceByAllcase(std::shared_ptr<DAG> newDag) : Sequence(newDag) { SetSequenceMatrix(); InitializeVisitedPermutationNumber(); }
};

class OptimizeCase : public Sequence {
protected:
    void SetSequenceMatrix() {}

public:
    OptimizeCase(std::shared_ptr<DAG> newDag) : Sequence(newDag) {}

    void SetSequence(int caseIndex);
    int GetRandomEmptyIndex() { return numberOfRemainedCase_; }
};

class OneCase : public Sequence {
protected:
    void SetSequenceMatrix() {}

public:
    OneCase(std::shared_ptr<DAG> newDag) : Sequence(newDag) { }

    void SetSequence(int caseIndex);
    int GetRandomEmptyIndex() { return numberOfRemainedCase_; }
};

class CustomCase : public Sequence {
protected:
    void SetSequenceMatrix() {}

public:
    CustomCase(std::shared_ptr<DAG> newDag) : Sequence(newDag) { }

    void SetSequence(int caseIndex);
    int GetRandomEmptyIndex() { return numberOfRemainedCase_; }
};

#endif