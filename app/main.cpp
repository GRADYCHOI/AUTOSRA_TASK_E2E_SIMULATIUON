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
    //std::clog.setstate(std::ios_base::failbit);

    // clear screen (only linux)
    std::system("clear");

    srand(time(NULL));
    bool dag_file = false;
    int mappingStrategy = 2;
    int simulateMethod = 0;

    std::shared_ptr<DAG> dag(new DAG());

    // Runnable & Task
    if (argc > 1) {
        try {
            dag->ParseDag(argv[1]);
        } catch (std::string error) {
            std::cout << error << std::endl;
            return 0;
        }
    } else {
        dag->GenerateDag();
    }

    // Mapping
    if (argc > 2) {
        try {
            dag->ParseMapping(argv[2]);
        } catch (std::string error) {
            std::cout << error << std::endl;
            return 0;
        }
    } else {
        switch (mappingStrategy) {
            case 0 : {
                dag->SetMapping(std::unique_ptr<Mapping>(new InputMapping()));
                break;
            }

            case 1 : {
                dag->SetMapping(std::unique_ptr<Mapping>(new RateMapping()));
                break;
            }

            case 2 : {
                dag->SetMapping(std::unique_ptr<Mapping>(new RandomMapping()));
                break;
            }

            default : {
                std::cout << "Wrong Mapping Strategy." << std::endl;
                return 0;
            }
        }

        dag->SetTaskPriority();
        dag->SetRunnablePrecedence();
        dag->DoMapping();
    }

    dag->ResetMappedRunnablePriority1(); // 케이스 1개 짜리
    //dag->ResetMappedRunnablePriority2(); // 케이스 경우의수
    //dag->AllCaseRunnablePriority();
    std::unique_ptr<Simulation> simulation(new Simulation(dag));
    
    switch (simulateMethod) {
        case 0 : {
            simulation->Simulate(RunnableImplicitMethod);
            break;
        }

        case 1 : {
            simulation->Simulate(TaskImplicitMethod);
            break;
        }

        case 2 : {
            simulation->Simulate(LETMethod);
            break;
        }

        default : {
            std::cout << "Wrong Simulate Method." << std::endl;
            return 0;
        }
    }
    
    std::clog << "[main.cpp] CheckPoint 1" << std::endl;
    simulation->SaveDag();
    std::clog << "[main.cpp] CheckPoint 2" << std::endl;
    simulation->SaveData();
    std::clog << "[main.cpp] CheckPoint 3" << std::endl;
    simulation->SaveMapping();
    return 0;
}