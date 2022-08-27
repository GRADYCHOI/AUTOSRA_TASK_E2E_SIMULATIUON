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
    //std::clog.setstate(std::ios_base::failbit);

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

    std::clog << "[main.cpp] CheckPoint 1" << std::endl;
    dag->DoMapping();
    std::clog << "[main.cpp] CheckPoint 2" << std::endl;
    dag->SetRunnablePrecedence();
    std::clog << "[main.cpp] CheckPoint 3" << std::endl;
    dag->SetTaskPriority();
    std::clog << "[main.cpp] CheckPoint 4" << std::endl;
    dag->SetRunnablePriorities();
    std::clog << "[main.cpp] CheckPoint 5" << std::endl;

    std::unique_ptr<Simulation> simulation(new Simulation(std::move(dag)));
    std::clog << "[main.cpp] CheckPoint 6" << std::endl;
    simulation->SetCommunication(std::unique_ptr<Communication>(new RunnableImplicit()));
    std::clog << "[main.cpp] CheckPoint 7" << std::endl;
    
    simulation->Simulate();
    std::clog << "[main.cpp] CheckPoint 8" << std::endl;
    

    return 0;
}