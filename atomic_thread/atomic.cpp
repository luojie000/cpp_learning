#include<atomic>
#include<iostream>
#include<thread>
#include<vector>

class atomic_test
{
public:
std::atomic<int> cnt{0};
void inc(){cnt++;}
};

class noatomic_test
{
    public:
    int cnt=0;
    void inc(){cnt++;}
};

template<typename counter>
void run_test(const char*name)
{
    counter c;
    std::vector<std::thread> threads;
    for(int i=0;i<10;i++)
    {
        threads.emplace_back([&c]{
            for(int j=0;j<10000;j++)
            {
                c.inc();
            }
        });
    }
    for(auto&t:threads)t.join();
    std::cout<<name<<" result:"<<c.cnt<<"(expected 100000)"<<std::endl;
}
int main()
{
    while(1)
    {
    run_test<atomic_test>("atomic");
    run_test<noatomic_test>("noatomic");
}
}