#include "communication.hpp"


class RunnableImplicit : public Communication {
public:
    void GetCommunicationTable(RunnableInformation* runnableInformations, ExecutionInformation* runnableExecutions, int numberOfRunnables, int maxCycle, CommunicationInformation* runnableCommunications) {
        memcpy(runnableExecutions, runnableCommunications, sizeof(ExecutionInformation) * numberOfRunnables * maxCycle);
    }
};