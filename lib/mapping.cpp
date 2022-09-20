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
            std::cout << task->GetId() << "task core(0, 1) : ";
            std::cin >> core;
            task->SetCore(core); 
        }
    }
}

void RateMapping::DoMapping(std::vector<std::shared_ptr<TASK>>& tasks, std::vector<std::shared_ptr<RUNNABLE>>& runnables) {
    double totalPeriod = 0;
    for (auto &task : tasks) totalPeriod += task->GetPeriod();

    for (auto &runnable : runnables) {
        bool map = true;
        while (map) {
            int ran = std::rand() % static_cast<int>(tasks.size());
            for (auto &task : tasks) {
                if (task->GetId() == ran) {
                    if (task->GetNumberOfRunnables() <= std::ceil((((task->GetPeriod()/totalPeriod) * runnables.size())))) {
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
            std::cout << task->GetId() << "task core(0, 1) : ";
            std::cin >> core;
            task->SetCore(core); 
        }
    }
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

    int multi = -1;
    std::cout << "single-core? or multi-core? (0, 1) : ";
    std::cin >> multi;
    
    if (multi == 1) {
        for (auto &task : tasks) {
            int core = -1;
            std::cout << task->GetId() << "task core(0, 1) : ";
            std::cin >> core;
            task->SetCore(core); 
        }
    }
}

