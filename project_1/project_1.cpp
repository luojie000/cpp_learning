#include<iostream>
#include<ostream>
class mystring
{
private:
char*data;
size_t size;
public:
mystring(const char*other)
{
    int i=0;
while(other[i]!='\0')
{
    i++;
}
size=i;
data=new char[size+1];
for(int i=0;i<size;++i)
{
    data[i]=other[i];
}
data[size]='\0';
}
~mystring()
{
    printf("析构函数执行\n");
    delete[]data;
    size=0;
}
//拷贝构造
mystring(const mystring&other):data(new char[other.size+1]),size(other.size)
{
    for(int i=0;i<size;++i)
    {
        data[i]=other.data[i];
    }
    data[size]='\0';
}
mystring&operator=(const mystring&other)
{
    if(this!=&other)
    {
        delete[]data;
        size=other.size;
        data=new char[size+1];
        for(int i=0;i<size;++i)
        {
data[i]=other.data[i];
        }
        data[size]='\0';
    }
    return*this;
}
//移动构造
mystring(mystring&&other)noexcept:data(other.data),size(other.size)
{
    other.data=new char[1];
    other.data[0]='\0';
    other.size=0;
}
mystring&operator=(mystring&&other)noexcept
{
    delete[]data;   
    data=other.data;
    other.data=new char[1];
    other.data[0]='\0';
    size=other.size;
    other.size=0;
    return *this;
}
//return the size
size_t size_() const
{
    return size;
}
const char* str()const
{
    return data;
}
};

int main()
{
    mystring a("hello");
    mystring b("world");
    std::cout<<a.str()<<std::endl;
    std::cout<<b.str()<<std::endl;
    mystring c=std::move(a);
    std::cout<<c.str()<<std::endl;
    b=c;
    std::cout<<b.str()<<std::endl;

    return 0;
}