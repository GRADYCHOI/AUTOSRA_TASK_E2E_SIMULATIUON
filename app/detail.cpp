#include <iostream>
#include <stdlib.h>
#include <random>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <sys/time.h>
#include "mapping.hpp"
#include "communication.hpp"
#include "DAG.hpp"
#include "simulation.hpp"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        
        
    }

    std::clog.setstate(std::ios_base::failbit);
    srand(time(NULL));

    std::shared_ptr<DAG> dag = std::make_shared<DAG>(argv[1], argv[2]);
    std::unique_ptr<Simulation> simulation = std::make_unique<Simulation>(dag);

    simulation->SetCommunicationCommand(std::move(communicationClass));
    simulation->SimulateDetail();
    
    return 0;
}