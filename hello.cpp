#include <iostream>
#include<typeinfo>
#include<type_traits>
using namespace std;
void check(const int&x)
{
    cout<<"use left version"<<endl;
}
void check(int &&x)
{
    cout<<"use right version"<<endl;
}
template<typename T>
void printcategory(T&& arg)
{
    //类型退化
    using rawtype =decay_t<T>;
    cout<<"decltype推导类型:"<<typeid(rawtype).name()<<"修饰名："<<typeid(T).name()<<endl;
    if (is_lvalue_reference_v<T>)
    {
        cout<<"left"<<endl;
    }
    else if (is_rvalue_reference_v<T>)
    {
        cout<<"right"<<endl;
    }
    else
    {
        cout<<"纯 right"<<endl;
    }
}
int main() {
int a=10;
int*c=&a;
cout<<"直接传入变量a"<<endl;
check(a);
cout<<"传入字面变量10"<<endl;
check(10);
cout<<"传入move（a）"<<endl;
check(move(a));
cout<<"使用decltype and typeid"<<endl;
int b=20;
cout<<&b<<endl;
cout<<"变量a"<<endl;
printcategory(a);
cout<<"字面量10"<<endl;
printcategory(10);
cout<<"move(a)"<<endl;
printcategory(move(a));
cout<<"a+b"<<endl;
printcategory(a+b);
cout<<"point c"<<endl;
printcategory(c);
    return 0;
 }