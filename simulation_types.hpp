struct RunnableInformation {
    int taskId;
    int priority;
    double period;
    double offset;
    double executionTime;
};

struct ExecutionInformation {
    double startTime;
    double endTime;
};

struct CommunicationInformation {
    double readTime;
    double writeTime;
};