#include "communication.hpp"


class LET : public Communication {
public:
    LET();
    ~LET();

    void GetCommunicationTable(RunnableInformation* runnableInformations, ExecutionInformation* runnableExecutions, int numberOfRunnables, int maxCycle, CommunicationInformation* runnableCommunications) {
        for (int runnableId = 0; runnableId < numberOfRunnables; runnableId++) {
            for (int cycle = 0; cycle < maxCycle; cycle++) {
                runnableCommunications[runnableId * maxCycle + cycle].readTime = runnableInformations[runnableId].period * cycle + runnableInformations[runnableId].offset;
                runnableCommunications[runnableId * maxCycle + cycle].writeTime = runnableInformations[runnableId].period * (cycle + 1) + runnableInformations[runnableId].offset;
            }
        }
    }
};