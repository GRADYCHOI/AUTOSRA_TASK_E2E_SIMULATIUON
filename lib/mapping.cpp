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
        bool map = true;
        while (map) {
            //std::cout << runnable->GetId() << std::endl;
            int ran = std::rand() % static_cast<int>(tasks.size());
            float rate = (double)taskRate[ran]/100;
            //std::cout << ran << std::endl;
            for (auto &task : tasks) {
                //std::cout << ran << std::endl;
                if (task->GetId() == ran) {
                    //std::cout << ran << std::endl;
                    //std::cout << task->GetId() << " task runnables : " << task->GetNumberOfRunnables() << " , rate : " << rate << " " << runnables.size() << " " << rate * runnables.size() << std::endl;
                    if (task->GetNumberOfRunnables() < std::ceil(rate * runnables.size())) {
                        std::cout << task->GetId() << " task runnables : " << task->GetNumberOfRunnables() << " , rate : " << taskRate[ran] << " " << runnables.size() << " " << rate * runnables.size() << std::endl;
                        task->AddRunnable(runnable);
                        map = false;
                    }
                }
            }
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