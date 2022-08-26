#include <iostream>
#include <stdlib.h>
#include <random>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "mapping.hpp"
#include "communication.hpp"
#include "DAG.hpp"
#include "simulation.hpp"

#include "mapping_RandomMapping.cpp"
#include "communication_LET.cpp"
#include "communication_RunnableImplicit.cpp"
#include "communication_TaskImplicit.cpp"

int main() {
    bool dag_file = false;
    int numberOfRunnables = 0;
    int numberOfTasks = 0;
    int mappingStrategy = 0;

    std::unique_ptr<DAG> dag(new DAG());

    if (dag_file) {
        /* parsing으로 dag 입력 */
    } else {
        std::cout << "Number of Runnables : ";
        std::cin >> numberOfRunnables;
        dag->GenerateRunnables(numberOfRunnables);

        std::cout << "Number of Tasks : ";
        std::cin >> numberOfTasks;
        dag->GenerateTasks(numberOfTasks);
    }

    switch (mappingStrategy) {
        case 0 : {
            //std::unique_ptr<Mapping> mapping(new RandomMapping());
            //dag->SetMapping(std::move(mapping));
            dag->SetMapping(std::unique_ptr<Mapping> (new RandomMapping()));
            break;
        }

        case 1 : {
            break;
        }

        default : {
            std::cout << "Wrong Number." << std::endl;
            break;
        }
    }

    dag->DoMapping();
    dag->SetRunnablePrecedence();
    dag->SetTaskPriority();
    dag->SetRunnablePriorities();

    for (int numberOfCase = 0; numberOfCase < dag->GetNumberOfSequenceCase(); numberOfCase++) {
        dag->SetRunnablePriority(numberOfCase);
        std::unique_ptr<Simulation> simulation(new Simulation(std::move(dag)));
        //std::unique_ptr<Communication> communication(new RunnableImplicit());
        //simulation->SetCommunication(std::move(communication));
        simulation->SetCommunication(std::unique_ptr<Communication>(new RunnableImplicit()));
        simulation->Simulate();
    }

    return 0;
}

// g++ main.cpp DAG.cpp TASK.cpp RUNNABLE.cpp simulation.cpp communication_LET.cpp communication_RunnableImplicit.cpp communication_TaskImplicit.cpp mapping_RandomMapping.cpp -o main -Wall