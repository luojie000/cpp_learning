#include<iostream>
#include<utility>
void print(const int &x)
{
    std::cout<<"print(const int &):"<<x<<"左值"<<std::endl;
}
void print(int &&x)
{
    std::cout<<"print(int&&x):"<<x<<"右值"<<std::endl;
}
//万能转发
template<typename T>
void relay(T&&arg)
{
    std::cout<<"万能转发"<<std::endl;
    print(std::forward<T>(arg));
}

int main()
{
    int a=1;
    const int b=100;
    std::cout<<"传入左值变量"<<std::endl;
    relay(a);
    std::cout<<"传入move"<<std::endl;
    relay(std::move(a));
    std::cout<<"传入临时对象"<<std::endl;
    relay(10);
    std::cout<<"传入const左值"<<std::endl;
    relay(b);
    std::cout<<"直接调用print"<<std::endl;
    print(a);
    print(10);
    return 0;
}