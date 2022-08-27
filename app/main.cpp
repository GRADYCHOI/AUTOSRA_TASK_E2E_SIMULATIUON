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
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

int main() {
    // clear screen (only linux)
    std::system("clear");

    srand(time(NULL));
    bool dag_file = false;
    int mappingStrategy = 0;

    std::unique_ptr<DAG> dag(new DAG());

    if (dag_file) {
        dag->ParseDag();
    } else {
        dag->GenerateDag();
    }

    switch (mappingStrategy) {
        case 0 : {
            dag->SetMapping(std::unique_ptr<Mapping>(new RandomMapping()));
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

    std::unique_ptr<Simulation> simulation(new Simulation(std::move(dag)));
    simulation->SetCommunication(std::unique_ptr<Communication>(new RunnableImplicit()));
    simulation->Simulate();

    return 0;
}