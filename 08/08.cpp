#include <iostream>
#include "threadpool.h"

struct A {};

int foo(const A&) {return 7;}

int main()
{

	ThreadPool pool(8);

	auto task1 = pool.exec(foo, A());
	std::cout << task1.get() << std::endl;

	auto task2 = pool.exec([]() { return 1; });
	std::cout << task2.get() << std::endl;

    std::vector< std::future<int> > results;

    for(int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.exec([i] {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                return i*i;
            })
        );
    }

    for(auto && result: results)
        std::cout << result.get() << ' ';
    std::cout << std::endl;

	std::cout << "Done" << std::endl;

	return 0;
}