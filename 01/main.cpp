#include <iostream>
#include <sstream>
#include <string>

int op;
char ch;
std::stringstream ss;

int N();
int N1();

int N()
{
    int num;
    if (!(ss >> num)){
        exit(1);
    }
    if (!(ss >> ch)){
        return num;
    }
    switch (ch){
        case '+':
            return num + N();
        case '*':
            num *= N1();
            switch (op){
                case 1:
                    return num +  N();
                case -1:
                    return num - N();
                default:
                    return num;
            }
            break;
        case '/':
            num /= N1();
            switch (op){
                case 1:
                    return num + N();
                case -1:
                    return num - N();
                default:
                    return num;
            }
            break;
        case '-':
            return num - N();
        default:
            exit(1);
    }
}

int N1()
{
    int num;
    if (!(ss >> num)){
        exit(1);
    }
    if (!(ss >> ch)){
        return num;
    }
    switch(ch){
        case '*':
            return num * N1();
        case '/':
            return num / N1();
        case '+':
            op = 1; 
            return num;
        case '-':
            op = -1;
            return num;
        default:
            exit(1);
    }
         
}



int main(int argc, char** argv)
{
    if (argc < 1) {
        return 1;
    }
    ss << argv[1];
    std::cout <<  N()  << std::endl;    
    return 0;
}
