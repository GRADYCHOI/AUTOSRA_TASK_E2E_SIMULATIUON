#include "RUNNABLE.hpp"

RUNNABLE::RUNNABLE() {
    this->execution_time = 0;
    this->status = -1;
}

RUNNABLE::RUNNABLE(double time, int status) {
    this->execution_time = time;
    this->status = status;
}
void RUNNABLE::get_status() {
    status = 0;
}

void RUNNABLE::set_precedence(int number) {
    if (this->precedence == -1) this->precedence = number;
}

void RUNNABLE::get_execution_time(double time) {
    this->execution_time = time;
}

RUNNABLE::~RUNNABLE() {
    cout << execution_time << endl;
    cout << ID << endl;
    this->execution_time = 0;
    this->precedence = 0;
    this->status = -1;
}