#include <iostream>
#include <stdlib.h>
#include <vector>


void set_release_table(DAG dag, vector<vector<double>> release_table);
void set_execution_table(DAG dag, vector<vector<double>> release_table, vector<vector<pair<double, double>>> execution_table);
void get_reaction_time(DAG dag, vector<vector<pair<double, double>>> execution_table);
void get_data_age(DAG dag, vector<vector<pair<double, double>>> execution_table);
void get_worst_reaction_time();
void get_worst_data_age();

int main(int argc, char **argv) {
    vector<vector<double>> release_table;
    vector<vector<pair<double, double>>> execution_table;

}