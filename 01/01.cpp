#include <iostream>
#include <sstream>

char last_op;

int read(std::istream *ss);
int SM(std::istream *ss);
int MD(std::istream *ss);

int read(std::istream *ss)
{
    int num = 0;
    char ch;

        if (!(*ss >> ch)){
            throw 1;
        }
        switch (ch){
            case '-':
                return -read(ss);

            case '0': case '1': case '2': case '3': case '4': 
            case '5': case '6': case '7': case '8': case '9':
                ss -> putback(ch);
                *ss >> num;
                if (!(*ss >> last_op)){
                    last_op = 'E';
                }
                return num;

            default:
                throw 1;
        }   
    return num;
}

int SM(std::istream *ss)
{
    int num = MD(ss);
    for (;;){
        switch (last_op){
            case '+':
                num += MD(ss);
                break;
            case '-':
                num -= MD(ss);
                break;
            default:
                return num;
        }
    }
}

int MD(std::istream *ss)
{
    int left = read(ss);
    for (;;){
        switch (last_op){
            case '*':
                left *= read(ss);
                break;
            case '/':
            {
                int d = read(ss);
                if (d == 0){
                    throw 1;
                }
                left /= d;
                break;
            }
            default:
                return left;
        }
    }
    return left;
}



int main(int argc, char** argv)
{
    std::istream *ss;
    switch (argc){
        case 2:
            ss = new std::istringstream(argv[1]);
            break;
        default:
            std::cout << "error" << std::endl;
            return 1;
    }
    try{
        std::cout <<  SM(ss)  << std::endl;
    } catch (int) {
        std::cout << "error" << std::endl;
        delete ss;
        return 1;
    }
    delete ss;
    return 0;
}
