#include <DAG.hpp>

DAG::DAG() {}

DAG::~DAG() {}

void DAG::SetParameters() {
    this->SetNumberOfTasks();
    this->SetNumberOfRunnables();
    this->SetHyperPeriod();
}

void DAG::SetHyperPeriod() {
    double tmpPeriod = 0.0;

    for(int count = 0; count < this->numberOfTasks; count++) {
        if (tmpPeriod < this->task[count].period) {
            this->hyperPeriod = std::lcm(this->hyperPeriod, this->task[count].period);
            tmpPeriod = this->task[count].period;
        }
    }
}

void DAG::SetNumberOfTasks() {
    this->numberOfTasks = this->task.size();
}

void DAG::SetNumberOfRunnables() {
    this->numberOfRunnabels = 0;

    for (int count = 0; count < this->numberOfTasks; count++) {
        this->numberOfRunnables += this->task[count].numberOfRunnables;
    }
}

int DAG::GetNumberOfTasks() {
    return this->numberOfTasks;
}

int DAG::GetNumberOfRunnables() {
    return this->numberOfRunnables;
}

double DAG::GetHyperPeriod() {
    return this->hyperPeriod;
}

double DAG::GetReactionTime() {
    return this->reactionTime;
}

couble DAG::GetDataAge() {
    return this->dataAge;
}

void DAG::AddTask(Task& task) {
    this->task.push_back(task);

    this->SetParamters();
}

void DAG::Simulate() {
    
}