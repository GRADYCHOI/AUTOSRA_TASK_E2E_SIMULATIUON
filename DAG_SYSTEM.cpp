#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>
#include <iostream>
#include <vector>

using namespace std;

/* default options */
#define N_MAX 20


enum dag_type {A, B, C, D};
//enum property {DRP, WP, DSP};
//enum execution_type {TE, DE};

static int DAG_parse(int number, int p[], int e[], vector<vector<int>> run);
static int parse_options(int argc, char *argv[], int *dag_type, int e[]);
static int run_mapping(int dag_type, int e[], int p[], long secs, long usecs);
static int get_n(int dag_type);
static int Delay_C(int e[], int p[], int dag_type);
static int Delay_2(int e[], int p[], int dag_type);
static int Delay_R(int e[], int p[], int dag_type);
int lcm(int a,int b);
static int Response_c(int i, int e[], int p[], int n, int priority[], int value);

int main(int argc, char *argv[]) {

    int e[argc-2];
    int p[argc-2];
    int dag_type;
    int map = 0;
    int DAG = 0;
    int delay_c = 0, delay_2 = 0, delay_r = 0;
    long secs, usecs;


    map = parse_options(argc, argv, &dag_type, e);
    int n = get_n(dag_type);
    vector<vector<int>> runnable(n+1, vector<int> {});

    if (map == 0) {
        map = run_mapping(dag_type, e, p, secs, usecs);
    }

    if (DAG == 0) {
        DAG = DAG_parse(n, p, e, runnable);
    }

    if (delay_c == 0) {
        delay_2 = Delay_2(e, p, dag_type);  
        delay_r = Delay_R(e, p, dag_type);
        delay_c = Delay_C(e, p, dag_type);
    }

    else if (map <0) {
        return EXIT_FAILURE;
    }

}

static int parse_options(int argc, char *argv[], int *dag_type, int e[]) {
    //parse a parameters and setting an options.
    int i, n, opt;

    if (strcmp(argv[optind], "a") == 0 || strcmp(argv[optind], "A") == 0) {
        *dag_type = A;
        printf("DAG Type : %s\n", argv[optind]);
    }
    else if (strcmp(argv[optind], "b") == 0 || strcmp(argv[optind], "B") == 0) {
        *dag_type = B;
        printf("DAG Type : %s\n", argv[optind]);
    }
    else if (strcmp(argv[optind], "c") == 0 || strcmp(argv[optind], "C") == 0) {
        *dag_type = C;
        printf("DAG Type : %s\n", argv[optind]);
    }
    else if (strcmp(argv[optind], "d") == 0 || strcmp(argv[optind], "D") == 0) {
        *dag_type = D;
        printf("DAG Type : %s\n", argv[optind]);
    }
    else {
        fprintf(stderr, "DAG Type is not define");
        return -1;
    }
    optind++;
    n = get_n(*dag_type);
    if (n < 0) {
        fprintf(stderr, "DAG Type Error, %s is not define\n", argv[optind - 1]);
        return -1;
    }
    if (n > argc - optind) {
        fprintf(stderr, "%s Type requires %d execution tiems\n", argv[optind - 1], n);
        return -1;
    }
    else if (n < argc - optind) {
        fprintf(stderr, "%s Type requires %d execution tiems\n", argv[optind -1], n);
        return -1;
    }
    for (i = 1 ; i <= argc-2 ; i++) {
        e[i] = atof(argv[optind++]);
    }
    return 0;
}

static int get_n(int dag_type) { 
    //dag type convert to number.
    switch (dag_type) {
    case A:
        return 4;
    case B:
        return 5;
    case C:
        return 6;
    case D:
        return 7;
    default:
        return -1;
    }
}
// 주기도 가져오자!!
static int DAG_parse(int number, int p[], int e[], vector<vector<int>> run) {
    int count = 1;
    bool DAG = true;
    cout << "Runnable count : " << number << endl;

    while (count <= number) {
        int k;
        int exe;
        cout << "Please write " << count << " Runnable's DAG Number : ";
        cin >> k;
        for (int i = 0; i < k; i++) {
            int d;
            cout << "Where? : ";
            cin >> d;
            run[count].insert(run[count].begin()+i, d);
        }
        count++;
    }
    cout << endl;

    for (int i = 1; i <= number; i++) {
        cout << i << " Runnable's Execution Time : " << e[i] << endl;
    }
    for (int i = 1; i <= number; i++) {
        cout << i << " Runnable's Period Time    : " << p[i] << endl;
    }
    for (int i = 1; i <= number; i++) {
        cout << i << " Runnable's DAG            : ";
        for (auto v : run[i]) {
            cout << v << " ";
        }
        cout << endl;
    }

    return 0;

}
// 크리티컬패스 찾는 알고리즘 넣기


static int run_mapping(int dag_type, int e[], int p[], long secs, long usecs) {
    //Runnable mapped roughest period Task.
    printf("-----------------------------------\n");
    printf("initial runnable mapping in tasks\n");

    int i, map;
    int n = get_n(dag_type);
    double Schedulability = 0;
    double utilization = 0;
    double total_execution_time = 0;
    double total_period = 0;
    double update_period = 0;
    bool opt_period = true;
    int max_period = 1;
    int max_execution = 0;

    for (i = 1 ; i <= n ; i++) {
        if (e[i] <= 5) {
            p[i] = 5;
        }
        else if ((e[i] > 5) && (e[i] <= 10)) {
            p[i] = 10;
        }
        else if ((e[i] > 10) && (e[i] <= 20)) {
            p[i] = 20;
        }
        else if ((e[i] > 20) && (e[i] <= 30)) {
            p[i] = 30;
        }
    }

    for (int i = 1; i <= n; i++) {
        utilization += (float)e[i]/p[i];        
    }
   
    printf("Utilization : %f      \n", utilization);
    printf("-----------------------------------\n");

    while (opt_period) {
       
        if (utilization > 0.67){ // 유틸리제이션이 0.67 이상이면 period 업데이트 시작. RM을 위해.
            int max_num = 0;
            double run_util = 0;
            int num[n];
            int array_num = 0;
            for (int i = 1 ; i <= n ; i++) { 
                if ((float)e[i]/p[i] > run_util){
                    run_util = (float)e[i]/p[i];
                    for (int j = 0 ; j <= n ; j++) {
                        num[j] = 0;
                    }
                    num[i] = 1;
                }
                else if ((float)e[i]/p[i] == run_util) {
                    num[i] = 1;
                }
            }
            //printf("largest runnable utilization rate : %f\n", run_util);
            for (int i = 0 ; i <= n ; i++) { 
                if (num[i] == 1){
                    p[i] += 5;
                }
            }
            
            utilization = 0;
            for (int k = 1 ; k <= n ; k++) {
                utilization += (float)e[k]/p[k];
            } 
            printf("Utilization : %f \n", utilization);
            //Delay_C(e, p, dag_type);

            printf("-------------------------------------------\n");
        }
        else {
            printf("Utilization : %f , Optimazation Complet!\n", utilization);
            for (int j = 1 ; j <= n ; j++) {
                printf("%d task execution time : %dms,  period : %dms\n", j, e[j], p[j]);
            }
            opt_period = false;
        }
    }
    
    return 0;
}

static int Delay_C(int e[], int p[], int dag_type) {
    
    int n = get_n(dag_type);
    int count = 0;
    int worst_delay = 0;
    int data_age = 0;
    int priority[n];
    int flag[n];
    int data_seq[n];
    int task_exec[n];
    int start;
    int exec_seq[n];
    int seq = 1;

    for (int i = 0; i <= n; i++){
        priority[i] = 0;
        data_seq[i] = 0;
        flag[0] = 0; 
        exec_seq[0] = 0;
    }

    for (int i = 1; i <= n; i++) {    // priority setting
        int temp = 1;
        for (int j = 1; j <= n; j++) {
            if (i == j) {
                continue;
            }
            else {
                if (p[i] > p[j]) {
                    temp++;
                }
            }
        }
        if (priority[temp] == 0) {
            priority[temp] = i;
        }
        else if (priority[temp] != 0) {
            temp++;
            priority[temp] = i;
        }
        task_exec[i] = e[i];
    }


    while (count < 10000){
        int flag_sum = 0;
        int data_seq_sum = 0;
        
        for (int i = 1; i <= n; i++) {
            if (count % p[i] == 0) {
                //printf("%d task on\n", i);
                flag[i] = 1;
                task_exec[i] = e[i];
            }
        }
        for (int i = 1; i <= n; i++) {
            flag_sum = flag_sum + flag[i];
        }
        
        for (int i = 1; i <= n; i++) {
            if (flag[priority[i]] == 1) { 
                task_exec[priority[i]]--;
                //printf("%d task execute\n", priority[i]);
                count++;
                data_age++;

                if (task_exec[priority[i]] == 0) { //task의 동작이 끝난면
                    if (priority[i] == seq) {
                        if ((seq == 1) && (data_seq[2] == 0)) data_age = e[priority[seq]];
                        data_seq[seq] = 1;
                        seq++;
                    }
                    flag[priority[i]] = 0;
                    for (int j = 1; j <= n; j++) {
                        data_seq_sum += data_seq[j];
                    }
                }
                if (data_seq_sum == n) {
                    //printf("Delay : %dms \n", data_age);
                    if (data_age >= worst_delay) worst_delay = data_age;
                    data_age = 0;
                    seq = 1;
                    for (int j = 1; j <= n; j++) {
                        data_seq[j] = 0;
                    }
                }
                break;
            }
        }
        if (flag_sum == 0) {
            count++;
            if(data_seq[1] == 1) data_age++;
        }
    }
    printf("E2E Delay with RM simulator : %d ms\n", worst_delay);
    return 0;
}

static int Delay_2(int e[], int p[], int dag_type) {
    int sum = 0;
    int n = get_n(dag_type);
    for (int i = 1; i <= n; i++) {
        sum += p[i]*2;
    }
    printf("E2E Delay with Period X 2   : %d ms\n", sum);
    return sum;
}
 
static int Delay_R(int e[], int p[], int dag_type) {
    int n = get_n(dag_type);
    int priority[n];
    int response[n];
    int sum = 0;

    for (int i = 1; i <= n; i++) {    // priority setting
        int temp = 1;
        for (int j = 1; j <= n; j++) {
            if (i == j) {
                continue;
            }
            else {
                if (p[i] > p[j]) {
                    temp++;
                }
            }
        }
        priority[i] = temp;
        if (priority[i-1] == priority[i]) {
            priority[i]++;
        }
    }    
    
    for (int i = 1; i <= n; i++) {
        int value = e[priority[i]];
        response[i] = Response_c(i, e, p, n, priority, value);
        sum = sum + response[i] + p[priority[i]];
    }
    printf("E2E Delay with WCRT         : %d ms\n", sum);

    return 0;
}

int lcm(int a,int b) {
    int i,j;
    int temp; //공배수를 저장할 변수
    j = (a>b)?a:b; //둘중 큰수를 구함
    for(i=j;;i++) {
        if(i%a==0 && i%b==0){
            temp = i;
            break;
        }
    }
    return temp;
}

static int Response_c(int i, int e[], int p[], int n, int priority[], int value) {
    int total = 0;
    int sum = value; //value로 바꿔

    for (int j = 1; j < i; j++) {
        int upper = 0;
        if (sum % p[priority[j]] != 0){
            upper = (sum / p[priority[j]]) + 1;
        }
        else upper = sum / p[priority[j]];
        total = total + (upper * e[priority[j]]);
    }    
    total = total + e[priority[i]];

    if (value != total) Response_c(i, e, p, n, priority, total);

    return total;

}