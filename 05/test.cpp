#include <iostream>
#include <sstream>

struct Data
{
    uint64_t a;
    bool b;
    bool x;
    uint64_t c;
};

int main()
{
	Data a = {1, true, false, 0};

	std::cout << sizeof(a) << std::endl;
	return 0;
}