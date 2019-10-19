#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

char last_op;

int read(std::istream &ss);
int SM(std::istream &ss);
int MD(std::istream &ss);

class invalid_input 
{
    std::string ms;

public:
    
    invalid_input(std::string x):ms(x){}
    
    std::string what()
    {
        return ms;
    }
};
        

int read(std::istream &ss)
{
    int num = 0;
    char ch;

        if (!(ss >> ch)){
            throw invalid_input("Wrong input.");
        }
        switch (ch){
            case '-':
                return -read(ss);

            case '0': case '1': case '2': case '3': case '4': 
            case '5': case '6': case '7': case '8': case '9':
                ss.putback(ch);
                ss >> num;
                if (!(ss >> last_op)){
                    last_op = 'E';
                }
                return num;

            default:
                throw invalid_input("Wrong input.");
        }   
    return num;
}

int SM(std::istream &ss)
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

int MD(std::istream &ss)
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
                    throw invalid_input("Division by 0.");
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
    std::stringstream ss;
    switch (argc){
        case 2:
            ss << argv[1];
            break;
        default:
            std::cout << "Wrong arguments." << std::endl;
            return 1;
    }
    try{
        std::cout <<  SM(ss)  << std::endl;
    } catch (invalid_input  &x) {
        std::cout << x.what() << std::endl;
        return 1;
    }
    return 0;
}
