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
    int mappingStrategy = -1;
    int permutationMethod = -1;
    int simulateMethod = -1;

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
        std::system("clear");
        std::cout << "*** Select Mapping Strategy ***" << "\n";
        std::cout << " 0 : Input Mapping" << "\n";
        std::cout << " 1 : Rate Mapping" << "\n";
        std::cout << " 2 : Random Mapping" << "\n";
        std::cout << "\n" << "Enter Number : ";

        std::cin >> mappingStrategy;
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

    std::system("clear");
    std::cout << "*** Range of Cases ***" << "\n";
    std::cout << " 0 : All Case" << "\n";
    std::cout << " 1 : Precedence" << "\n";
    std::cout << "\n" << "Enter Number : ";

    std::cin >> permutationMethod;
    switch (permutationMethod) {
        case 0 : {
            dag->SetRunnableAllCase();
            break;
        }

        case 1 : {
            dag->SetPrecedenceRunnablePriority();
            break;
        }

        default : {
            std::cout << "Wrong Runnable's Priority Method." << std::endl;
            return 0;
        }
    }

    std::unique_ptr<Simulation> simulation(new Simulation(dag));
    
    std::system("clear");
    std::cout << "*** Communication Method ***" << "\n";
    std::cout << " 0 : Runnable Implicit" << "\n";
    std::cout << " 1 : Task Implicit" << "\n";
    std::cout << " 2 : LET" << "\n";
    std::cout << "\n" << "Enter Number : ";

    std::cin >> simulateMethod;
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
    std::clog << "[main.cpp] CheckPoint 4" << std::endl;
    simulation->SaveDataToCSV();
    return 0;
}