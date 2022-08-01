#include <vector>

class RUNNABLE //러너블 개수 = n, 러너블 목록[n], 엣지목록[n][n-1], 입력러너블[], 출력러너블[]
{ 
private:
    double execution_time;
    int precedence = -1;
    int status = -1; // 0 = input runnable, 1 = middle runnable, 2 = output runnable
    vector<int> input_runnable;
    vector<int> output_runnable;

public:
    RUNNABLE(double time, int status);

    int get_status();
    void set_precedence(int number);

    ~RUNNABLE();
};
