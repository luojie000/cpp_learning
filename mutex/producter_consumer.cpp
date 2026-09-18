#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <chrono>
#include <vector>

class tasksystem
{
private:
    std::queue<int> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    bool finished = false;
    std::atomic<int> completed{0};

public:
    void produce(int task)
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            tasks.push(task);
            std::cout << "produce the task:" << task << std::endl;
        }
        cv.notify_one();
    }

    void consume(int id)
    {
        while (true)
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]() {
                return !tasks.empty() || finished;
            });

            // 退出条件：没有任务且生产结束
            if (tasks.empty() && finished)
            {
                break;
            }

            int t = tasks.front();
            tasks.pop();
            std::cout << "consume the task:" << t << " by consumer " << id << std::endl;
            lock.unlock();

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            completed++;
        }
    }

    int getcompleted()
    {
        return completed.load();
    }

    void finish()
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            finished = true;
        }
        cv.notify_all();
    }
};

int main()
{
    tasksystem system;
    std::thread producer([&system]() {
        for (int i = 1; i <= 10; ++i) {
            system.produce(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        system.finish();
    });

    std::vector<std::thread> consumers;
    for (int i = 1; i <= 3; ++i) {
        consumers.emplace_back([&system, i]() {
            system.consume(i);
        });
    }

    producer.join();
    for (auto& t : consumers) {
        t.join();
    }

    std::cout << std::endl;
    std::cout << "====================" << std::endl;
    std::cout << "所有任务处理完成" << std::endl;
    std::cout << "完成任务数量: " << system.getcompleted() << std::endl;
    std::cout << "====================" << std::endl;

    return 0;
}