#include <RUNNABLE.hpp>

RUNNABLE::RUNNABLE(double time, int status) {
    this.execution_time = time;
    this.status = status;
}

void RUNNABLE::set_precedence(int number) {
    if (this.precedene == -1) this.prcedence = number;
}