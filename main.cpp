#include<iostream>
#include<stdlib.h>
#include<random>
#include<time.h>
#include<cstdlib>
#include<ctime>
#include<vector>
#include"DAG.hpp"
#include"simulation.hpp"
using namespace std;

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
            std::unique_ptr<Mapping> randomMapping(new RandomMapping());
            dag->SetMapping(std::move(randomMapping));
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
        std::unique_ptr<Communication> runnableImplicit(new RunnableImplicit());
        simulation->SetCommunication(std::move(runnableImplicit));
        simulation->Simulate();
    }

    return 0;
}
