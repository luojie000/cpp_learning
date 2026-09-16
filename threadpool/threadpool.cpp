#include<vector>
#include<queue>
#include<thread>
#include<atomic>
#include<mutex>
#include<condition_variable>
#include<functional>
#include<future>
#include<type_traits>
#include<memory>
#include<stdexcept>
#include<chrono>
#include<iostream>

class threadpool{
    private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>>tasks;
    bool stop=false;
    std::mutex queue_mutex;
    std::condition_variable condition;
public:
explicit threadpool(size_t threads=std::thread::hardware_concurrency())
{
    if(threads==0)
    {
threads=1;
    }
    for(size_t i=0;i<threads;++i)
{workers.emplace_back([this]{
    while(true)
    {
        std::function<void()>task;
        {
            std::unique_lock<std::mutex>lock(queue_mutex);
            condition.wait(lock,[this]{
                return stop||!tasks.empty();
            });
        if(stop&&tasks.empty())
        return ;
    task=std::move(tasks.front());
    tasks.pop();
        }
    task();
    }
});
}
}

template<class F,class... Args>
auto submit(F&&f,Args&&...args)->std::future<typename std::invoke_result_t<F,Args...>>
{
    using return_type=typename std::invoke_result_t<F,Args...>;
    auto task=std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f),std::forward<Args>(args)...)
    );
    std::future<return_type>res=task->get_future();
    {
        std::unique_lock<std::mutex>lock(queue_mutex);
        if(stop)throw std::runtime_error("submit on stopped threadpool");
        tasks.emplace([task](){(*task)();});
    }
    condition.notify_one();
    return res;
}

~threadpool()
{
    {
    std::unique_lock<std::mutex>lock(queue_mutex);
    stop=true;
    }
    condition.notify_all();
    for(std::thread&worker:workers)
    {
        if(worker.joinable())worker.join();
    }
}
};

int main() { 
    threadpool pool(4); 
 
    std::vector<std::future<int>> results; 
    for (int i = 0; i < 10; ++i) { 
        results.emplace_back(pool.submit([i] { 
            std::this_thread::sleep_for(std::chrono::milliseconds(50)); 
            return i * i; 
        })); 
    } 
 
    for (auto& res : results) { 
        std::cout << res.get() << ' '; 
    } 
    std::cout << std::endl; 
 
    // 异常传播测试 
    auto bad = pool.submit([]() -> int { 
        throw std::runtime_error("task failed"); 
        return 0; 
    }); 
    try { 
        bad.get(); 
    } catch (const std::exception& e) { 
        std::cout << "Caught: " << e.what() << std::endl; 
    } 
 
    return 0; 
}