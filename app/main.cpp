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
    srand(time(NULL));

    std::shared_ptr<DAG> dag;
    if (argc > 2) { // DAG.json + Mapping.json
        dag = std::make_shared<DAG>(argv[1], argv[2]);
    } else {
        std::cout << "\033[H\033[2J\033[3J";
        std::cout << "*** Select Mapping Strategy ***" << "\n";
        std::cout << " 0 : Input Mapping" << "\n";
        std::cout << " 1 : Rate Mapping" << "\n";
        std::cout << " 2 : Random Mapping" << "\n";
        std::cout << "\n" << "Enter Number : ";
        int mappingMethod = -1;
        std::cin >> mappingMethod;
        
        std::unique_ptr<Mapping> mappingClass;
        switch (mappingMethod) {
            case 0 : {
                mappingClass = std::make_unique<InputMapping>();
                break;
            }

            case 1 : {
                mappingClass = std::make_unique<RateMapping>();
                break;
            }

            case 2 : {
                mappingClass = std::make_unique<RandomMapping>();
                break;
            }

            default : {
                std::cout << "Wrong Mapping Strategy." << std::endl;
                return 0;
            }
        }

        // Random Generation
        std::cout << "\033[H\033[2J\033[3J";
        std::cout << "*** What do you want precedence strategy? ***" << "\n";
        std::cout << " 0 : Input Runnables to Output Runnables" << "\n";
        std::cout << " 1 : Output Runnables to Input Runnables" << "\n";
        std::cout << "\n" << "Enter Number : ";
        int precedenceMethod = -1;
        std::cin >> precedenceMethod;

        std::unique_ptr<Precedence> precedenceClass;
        switch (precedenceMethod) {
            case 0 : {
                precedenceClass = std::make_unique<InputToOutputPrecedence>();
                break;
            }

            case 1 : {
                precedenceClass = std::make_unique<OutputToInputPrecedence>();
                break;
            }
            
            default : {
                std::cout << "Wrong Precedence Strategy." << std::endl;
                return 0;
            }
        }

        dag = std::make_shared<DAG>(mappingClass, precedenceClass);
    }

    /*
    std::cout << "\033[H\033[2J\033[3J";
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
    */

    std::unique_ptr<Simulation> simulation = std::make_unique<Simulation>(dag);
    std::unique_ptr<Communication> communicationClass = std::make_unique<RunnableImplicit>(dag);
    simulation->SetCommunication(std::move(communicationClass));
    
    /*
    std::cout << "\033[H\033[2J\033[3J";
    std::cout << "*** Communication Method ***" << "\n";
    std::cout << " 0 : Runnable Implicit" << "\n";
    std::cout << " 1 : Task Implicit" << "\n";
    std::cout << " 2 : LET" << "\n";
    std::cout << "\n" << "Enter Number : ";
    std::cin >> simulateMethod;

    std::unique_ptr<Communication> communicationClass;
    switch (simulateMethod) {
        case 0 : {
            communicationClass = std::make_unique<RunnableImplicit>(dag);
            break;
        }

        case 1 : {
            communicationClass = std::make_unique<Task Implicit>(dag);
            break;
        }

        case 2 : {
            communicationClass = std::make_unique<Task Implicit>(dag);
            break;
        }

        default : {
            std::cout << "Wrong Simulate Method." << std::endl;
            return 0;
        }
    }
    */

    simulation->Simulate();
    
    return 0;
}