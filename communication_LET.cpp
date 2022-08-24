#include "communication.hpp"


class LET : public Communication {
public:
    LET() { this->readMemory = new ReleaseTimeRead(); this->writeMemory = new ReleaseTimeWrite(); }
};