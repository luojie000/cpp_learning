#include<iostream>
#include<utility>
#include<vector>
#include <cstring>
class mystring
{
    char*data;
    public:
    mystring(const char*s=""){
data=new char[std::strlen(s)+1];
std::strcpy(data,s);
    }
    mystring(mystring&&other)noexcept:data(other.data)
    {
        other.data=nullptr;
    }
    mystring&operator=(mystring&&other)noexcept
    {
        if(this!=&other)
        {
     delete[]data;
     data=other.data;
     other.data=nullptr;       
        }
        return *this;
    }
    ~mystring()
    {
        delete[]data;
    }
    //友元输出运算符
    friend std::ostream& operator<<(std::ostream&os,const mystring&s)
    {
        os<<s.data;
        return os;
    }
};
//move函数的代码实现
template<typename T>
typename std::remove_reference_t<T>&&move(T&&t)noexcept{
    return static_cast<typename std::remove_reference_t<T>&&>(t);
}
int main()
{
    mystring s("hello");
    mystring s2=std::move(s);
    s=mystring("world");
    std::cout<<s2<<std::endl;
    std::cout<<s<<std::endl;
    return 0;
}