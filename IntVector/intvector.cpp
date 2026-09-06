#include<utility>
#include<algorithm>
#include<stdexcept>
#include <iostream>
#include <cassert>
class intvector
{
    private:
    int*arr;
    size_t sz;
    size_t cap;
    void reallocate(size_t new_cap)
    {
        if(new_cap<sz)
        new_cap=sz;
        int *temp=new int[new_cap];
        for(size_t i=0;i<sz;i++)
        {
            temp[i]=std::move(arr[i]);
        }
        delete[]arr;
        arr=temp;
        cap=new_cap;
    }
    public:
    intvector():arr(nullptr),sz(0),cap(0){}
    explicit intvector(size_t count,int value=int())
    {
        arr=new int[count];
        sz=cap=count;
        for(size_t i=0;i<count;i++)
        {
            arr[i]=value;
        }
    }

    //深拷贝
    intvector(const intvector&other):arr(new int[other.cap]),sz(other.sz),cap(other.cap)
    {
for(size_t i=0;i<sz;i++)
{
    arr[i]=other.arr[i];
}
    }

    //移动构造
    intvector(intvector&&other)noexcept:arr(other.arr),sz(other.sz),cap(other.cap)
    {
other.arr=nullptr;
other.sz=0;
other.cap=0;
    }

    //拷贝赋值
    intvector& operator=(const intvector&other)
    {
        if(this==&other)
        {
            return *this;
        }
        delete[]arr;
        arr=new int[other.cap];
        sz=other.sz;
        cap=other.cap;
        for(size_t i=0;i<sz;i++)
        {
            arr[i]=other[i];
        }
        return *this;
    }

    //移动赋值
    intvector& operator=(intvector&&other)noexcept
    {
        if(this==&other)
        {
            return *this;
        }
        delete[]arr;
        arr=other.arr;
        sz=other.sz;
        cap=other.cap;
        other.arr=nullptr;
        other.sz=0;
        other.cap=0;
        return *this;
    }

    ~intvector()
    {
        delete[]arr;
    }

    //函数调用
    void push_back(int value)
    {
     if(sz==cap)
     {
      size_t new_cap=(cap==0)?1:cap*2;
      reallocate(new_cap);
     }
     arr[sz]=value;
     sz++;
    }

    //pop_back
    void pop_back()
    {
        if(sz>0)
        sz--;
    }

    size_t size()const{return sz;}
    size_t capacity()const {return cap;}
    bool empty()const
    {
        if(sz==0)
        return 1;
        return 0;
    }
    int &operator[](size_t index)
    {
        return arr[index];
    }
    const int&operator[](size_t index)const
    {
        return arr[index];
    }

    int&at(size_t index)
    {
        if(index>=sz)
        {
            throw std::out_of_range("index out of range");
        }
        return arr[index];
    }
    const int&at(size_t index)const{
        if(index>=sz)
        {
            throw std::out_of_range("index out of range");
        }
        return arr[index];
    }

    //keep the capcity
    void reserve(size_t new_cap)
    {
        if(new_cap>cap)
        {
            reallocate(new_cap);
        }
    }

    //set the size
    void resize(size_t new_size,int value=0)
    {
        if(new_size>sz)
        {
            if(new_size>cap)
            {
                reallocate(new_size);
            }
            for(size_t i=sz;i<new_size;i++)
            {
                arr[i]=value;
            }
        }
        sz=new_size;
    }
};

void test_int_vector() {
    intvector v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    assert(v.size() == 3);
    assert(v.capacity() >= 3);

    intvector v2 = v;               // 拷贝构造
    assert(v2.size() == v.size());
    v2[0] = 100;
    assert(v[0] == 1);              // 深拷贝，互不影响

    intvector v3 = std::move(v);   // 移动构造
    assert(v.size() == 0);
    assert(v.empty());
    assert(v3.size() == 3);

    v3.pop_back();
    assert(v3.size() == 2);

    // 边界测试
    v3.reserve(100);
    assert(v3.capacity() >= 100);
    v3.push_back(4);
    assert(v3[2] == 4);

    std::cout << "All IntVector tests passed!\n";
}

int main()
{
    test_int_vector();
    return 0;
}