#include <RUNNABLE.hpp>

RUNNABLE::RUNNABLE(double execution_time, int status) {
    this->execution_time = execution_time;
    this->status = status;
}

void RUNNABLE::set_precedence(int number) {
    if (this.precedene == -1) this.prcedence = number;
}