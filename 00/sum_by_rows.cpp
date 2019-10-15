#include <iostream>
#include <chrono>
#include "stdlib.h"
#include <time.h>

class Timer
{
    using clock_t = std::chrono::high_resolution_clock;
    using microseconds = std::chrono::microseconds;
public:
    Timer()
        : start_(clock_t::now())
    {
    }

    ~Timer()
    {
        const auto finish = clock_t::now();
        const auto us = 
            std::chrono::duration_cast<microseconds>
                (finish - start_).count();
        std::cout << us << " us" << std::endl;
    }

private:
    const clock_t::time_point start_;
};

const int SZ = 1000;

int main()
{
    srand(time(0));
    unsigned int dice_roll = rand();
    Timer t;
    int arr[SZ][SZ];
    for (int i = 0; i < SZ; ++i){
        for (int j = 0; j < SZ; ++j)
        {
            arr[i][j] = dice_roll;
            dice_roll = rand();
        }
    }

    unsigned long long sum = 0;
    for (int i = 0; i < SZ; ++i){
        for (int j = 0; j < SZ; ++j)
        {
            sum += arr[i][j];
        }
    }

    std::cout << "Сумма:" << std::endl;
    std::cout << sum << std::endl;


    return 0;         
}
