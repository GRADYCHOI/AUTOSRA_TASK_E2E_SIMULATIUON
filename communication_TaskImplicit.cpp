#include "communication.hpp"


class TaskImplicit : public Communication {
public:
    TaskImplicit() { this->readMemory = new StartOfTaskRead(); this->writeMemory = new EndOfTaskWrite(); }
};