#include <iostream>
#include <stdlib.h>
#include <vector>


void set_release_table(DAG& dag, vector<vector<double>> release_table);
void set_execution_table(DAG dag, vector<vector<double>> release_table, vector<vector<pair<double, double>>> execution_table);
void set_hyperperiod(DAG dag);
void get_release_time(double period, double offset, double hyperperiod, vector<double> *release_table_1task);
void get_reaction_time(DAG dag, vector<vector<pair<double, double>>> execution_table);
void get_data_age(DAG dag, vector<vector<pair<double, double>>> execution_table);
void get_worst_reaction_time();
void get_worst_data_age();

int main(int argc, char **argv) {
    vector<vector<double>> release_table;
    vector<vector<pair<double, double>>> execution_table;

}

void set_release_table(DAG& dag, vector<vector<double>> *release_table) {

    for (int task_id = 0; task_id < (dag.hyperperiod / dag.task[task_id].period); task_id++) {
        int period = dag.task[task_id].period;
        int offset = dag.task[task_id].offset;
        vector<double> release_times;

        for (int cycle = 0; cycle < dag.runnable[task].cycle; cycle++) {
            release_times.push_back(period * cycle + offset);
        }

        release_table->push_back(release_times);
        release_times.clear();
    }

}

void set_execution_table(DAG& dag, vector<vector<double> *release_table, vector<vector<pair<double, double>>> execution_table) {
    vector<pair<double,double>> empty_time;
    empty_time.push_back({0, dag.hypertime});

    for (int task = 0; task < dag.hyperperiod; task++) {
        int 
    }

}