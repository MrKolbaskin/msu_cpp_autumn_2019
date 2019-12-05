#include <vector>
#include <thread>
#include <future>
#include <functional>
#include <queue>


class ThreadPool
{
	std::vector<std::thread> threadsP;
	size_t size;
	std::mutex m;
	std::queue<std::function<void()> > taskQueue;
	bool live;
	std::condition_variable notThread;

	template <class Func, class... Args>
    void ExecFunc(std::promise<void> *promise, Func func, Args... args) {
        promise->set_value();
        delete promise;
        func(args...);
    }

    template <class Promise_ptr, class Func, class... Args>
    void ExecFunc(Promise_ptr promise, Func func, Args... args) {
        promise->set_value(func(args...));
        delete promise;
    }

public:
    explicit ThreadPool(size_t poolSize):size(poolSize),live(true){
    	size = poolSize;
    	for (size_t i = 0; i < size; ++i){
    		threadsP.emplace_back(
    			[this](){
    				while(live){
    					std::unique_lock<std::mutex> lock(m);
    					if (!taskQueue.empty()){
    						auto task(std::move(taskQueue.front()));
    						taskQueue.pop();
    						lock.unlock();
    						task();
    					} else{
    						notThread.wait(lock);
    					}
    				}
    			}
    		);
    	}
    }

    ~ThreadPool()
    {
    	live = false;
    	notThread.notify_all();
    	for(size_t i = 0; i < size; ++i){
    		threadsP[i].join();
    	}
    }



    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))>{
    	using promise_type = std::promise<decltype(func(args...)) >;

    	auto promise = new promise_type();
    	auto res = promise->get_future();
    	auto t = [this](promise_type *promise, Func func, Args... args) {
            ExecFunc(promise, func, args...);
        };

        std::function<void ()> task = std::bind(t, promise, func, args...);
        m.lock();
        taskQueue.push(task);
        m.unlock();
        notThread.notify_one();
        return res;
    }
};


