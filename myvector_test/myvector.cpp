#include<iostream>
#include<algorithm>
template<typename T>
class myvector
{
private:
T *start;
T*end;
T*finish;
public:
myvector():start(nullptr),end(nullptr),finish(nullptr)
{
    _reallocate(1);
}
void _reallocate(size_t newcapacity)
{
    T* newstart=new T[newcapacity];
    size_t oldsize=size();
    if(start)
    {
        std::copy(start,finish,newstart);
        delete[]start;
    }
    start=newstart;
    finish=start+oldsize;
    end=start+newcapacity;
}
~myvector()
{
    delete[]start;
    std::cout<<"Destructor called"<<std::endl;
start=end=finish=nullptr;
}
//push_back
void push_back(const T& value)
{
    if(finish==end)
    {
        size_t newcapacity=size()?2*size():1;
        _reallocate(newcapacity);
    }
    *finish=value;
    ++finish;
    }
size_t size()const
{
    return finish-start;
}
size_t capacity()const
{
    return end-start;
}
void printfarr()
{
    for(auto i=start;i<finish;++i)
    {
        std::cout<<*i<<std::endl;
    }
}
}
;

int main()
{
    myvector<int> v;
   std:: cout<<v.size()<<" "<<v.capacity()<<std::endl;
   for(int i=0;i<10;++i)
   {
   v.push_back(i);
   std::cout<<v.size()<<" "<<v.capacity()<<std::endl;
   }
   v.printfarr();
    return 0;
}