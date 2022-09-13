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


int main(int argc, char *argv[]) {
    std::clog.setstate(std::ios_base::failbit);

    // clear screen (only linux)
    std::system("clear");

    srand(time(NULL));
    bool dag_file = false;
    int mappingStrategy = 0;

    std::shared_ptr<DAG> dag(new DAG());

    if (argc > 1) {
        try {
            dag->ParseDag(argv[1]);
        } catch (std::string error) {
            std::cout << error << std::endl;
            return 0;
        }

        if (argc > 2) {
            try {
                dag->ParseMapping(argv[2]);
            } catch (std::string error) {
                std::cout << error << std::endl;
                return 0;
            }
        }
    } else {
        dag->GenerateDag();
    }

    switch (mappingStrategy) {
        case 0 : {
            dag->SetMapping(std::unique_ptr<Mapping>(new InputMapping()));
            break;
        }

        case 1 : {
            break;
        }

        default : {
            std::cout << "Wrong Mapping Number." << std::endl;
            return 0;
        }
    }

    dag->SetTaskPriority();
    dag->SetRunnablePrecedence();
    dag->DoMapping();
    //dag->ResetMappedRunnablePriority1();
    dag->ResetMappedRunnablePriority2();

    std::unique_ptr<Simulation> simulation(new Simulation(dag));
    
    simulation->Simulate(RunnableImplicitMethod);
    
    std::clog << "[main.cpp] CheckPoint 1" << std::endl;
    simulation->SaveDag();
    std::clog << "[main.cpp] CheckPoint 2" << std::endl;
    simulation->SaveData();
    std::clog << "[main.cpp] CheckPoint 3" << std::endl;
    
    return 0;
}