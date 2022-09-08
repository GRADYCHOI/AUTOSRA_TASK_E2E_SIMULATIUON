#include "mapping.hpp"
#include <iostream>
#include <cmath>


void RandomMapping::DoMapping(std::vector<std::shared_ptr<TASK>>& tasks, std::vector<std::shared_ptr<RUNNABLE>>& runnables) {
    for (auto &runnable : runnables) {
        tasks[std::rand() % static_cast<int>(tasks.size())]->AddRunnable(runnable);
    }
}

void RateMapping::DoMapping(std::vector<std::shared_ptr<TASK>>& tasks, std::vector<std::shared_ptr<RUNNABLE>>& runnables) {
    int *taskRate = new int[tasks.size()]();
    int totalPeriod = 0;
    for (auto &task : tasks) totalPeriod += task->GetPeriod();
    for (auto &task : tasks) {
        taskRate[task->GetId()] = (task->GetPeriod()/totalPeriod)*100;
    }

    for (auto &runnable : runnables) {
        while (true) {
            int ran = std::rand() % static_cast<int>(tasks.size());
            if (tasks[ran] ->GetNumberOfRunnables() <= (int)(taskRate[ran] * runnables.size())) {
                tasks[ran]->AddRunnable(runnable);
                break;
            }
            std::cout << "!!!" << std::endl;
        }
    }

    delete[] taskRate;
}

void InputMapping::DoMapping(std::vector<std::shared_ptr<TASK>>& tasks, std::vector<std::shared_ptr<RUNNABLE>>& runnables) { // defind input task - smallest period task.
    int inputTask = 0;
    int inputTaskPeriod = 1000;
    for (auto &task : tasks) { 
        if (task->GetPeriod() < inputTaskPeriod) {
            inputTaskPeriod = task->GetPeriod();
            inputTask = task->GetId();
        }
    }
    for (auto &runnable : runnables) {
        if (runnable->GetStatus() == 0) tasks[inputTask]->AddRunnable(runnable);
        else tasks[std::rand() % static_cast<int>(tasks.size())]->AddRunnable(runnable);
    }
}