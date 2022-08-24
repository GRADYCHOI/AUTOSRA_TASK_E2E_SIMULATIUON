#include "communication.hpp"


class RunnableImplicit : public Communication {
public:
    RunnableImplicit() { this->readMemory = new StartOfRunnableRead(); this->writeMemory = new EndOfRunnableWrite(); }
};