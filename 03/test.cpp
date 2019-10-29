#include <iostream>
#include "matrix.h"

int main()
{
    Matrix m (10, 10);

    for (int i = 0; i < 10; ++i){
        for (int j = 0; j < 10; ++j){
            m[i][j] = i * j;
        }
    }

    for (int i = 0; i < 10; ++i){
        for (int j = 0; j < 10; ++j){
            std::cout << m[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
