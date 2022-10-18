#include "mapping.hpp"
#include <iostream>
#include <cmath>


void RandomMapping::DoMapping(std::vector<std::shared_ptr<TASK>>& tasks, std::vector<std::shared_ptr<RUNNABLE>>& runnables) {
    for (auto &runnable : runnables) {
        tasks[std::rand() % static_cast<int>(tasks.size())]->AddRunnable(runnable);
    }
    int multi = -1;
    std::cout << "single-core? or multi-core? (0, 1) : ";
    std::cin >> multi;
    
    if (multi == 1) {
        for (auto &task : tasks) {
            int core = -1;
            std::cout << task->id_ << "task core(0, 1) : ";
            std::cin >> core;
            task->SetCore(core); 
        }
    } else {
        for (auto &task : tasks) {
            task->SetCore(0); 
        }
    }
}

void RateMapping::DoMapping(std::vector<std::shared_ptr<TASK>>& tasks, std::vector<std::shared_ptr<RUNNABLE>>& runnables) {
    double totalPeriod = 0;
    for (auto &task : tasks) totalPeriod += task->period_;

    for (auto &runnable : runnables) {
        bool map = true;
        while (map) {
            int ran = std::rand() % static_cast<int>(tasks.size());
            for (auto &task : tasks) {
                if (task->id_ == ran) {
                    if (task->GetNumberOfRunnables() <= std::ceil((((task->period_/totalPeriod) * runnables.size())))) {
                        task->AddRunnable(runnable);
                        map = false;
                    }
                }
            }
        }
    }
    
    int multi = -1;
    std::cout << "single-core? or multi-core? (0, 1) : ";
    std::cin >> multi;
    
    if (multi == 1) {
        for (auto &task : tasks) {
            int core = -1;
            std::cout << task->id_ << "task core(0, 1) : ";
            std::cin >> core;
            task->SetCore(core); 
        }
    } else {
        for (auto &task : tasks) {
            task->SetCore(0); 
        }
    }
}

void InputMapping::DoMapping(std::vector<std::shared_ptr<TASK>>& tasks, std::vector<std::shared_ptr<RUNNABLE>>& runnables) { // defind input task - smallest period task.
    int inputTask = 0;
    int inputTaskPeriod = 1000;
    for (auto &task : tasks) { 
        if (task->period_ < inputTaskPeriod) {
            inputTaskPeriod = task->period_;
            inputTask = task->id_;
        }
    }
    for (auto &runnable : runnables) {
        if (runnable->GetStatus() == 0) tasks[inputTask]->AddRunnable(runnable);
        else tasks[std::rand() % static_cast<int>(tasks.size())]->AddRunnable(runnable);
    }

    int multi = -1;
    std::cout << "single-core? or multi-core? (0, 1) : ";
    std::cin >> multi;
    
    if (multi == 1) {
        for (auto &task : tasks) {
            int core = -1;
            std::cout << task->id_ << "task core(0, 1) : ";
            std::cin >> core;
            task->SetCore(core); 
        }
    } else {
        for (auto &task : tasks) {
            task->SetCore(0); 
        }
    }
}

