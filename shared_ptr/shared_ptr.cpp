#include<utility>
#include<iostream>
#include<algorithm>
#include<cassert>
template<typename T>
class shared_ptr
{
private:
T*ptr;
int*shared_num;

void release()
{
    if(shared_num)
    {
        --*shared_num;
        if(!*shared_num)
        {
            delete ptr;
            delete shared_num;
        }
    }
    ptr=nullptr;
    shared_num=nullptr;
}
public:
shared_ptr():ptr(nullptr),shared_num(nullptr){}
explicit shared_ptr(T*otherptr):ptr(otherptr),shared_num(new int(1)){}
shared_ptr(const shared_ptr&other):ptr(other.ptr),shared_num(other.shared_num)
{
    if(shared_num)
(*shared_num)++;
}
shared_ptr(shared_ptr&&other)noexcept:ptr(other.ptr),shared_num(other.shared_num)
{
    other.ptr=nullptr;
    other.shared_num=nullptr;
}
shared_ptr& operator=(const shared_ptr&other)
{
    if(this==&other)return *this;
    release();
    ptr=other.ptr;
    shared_num=other.shared_num;
    if(shared_num)
    {
        ++*shared_num;
    }
return *this;
}
shared_ptr&operator=(shared_ptr&&other)noexcept
{
if(this==&other)return *this;
release();
ptr=other.ptr;
shared_num=other.shared_num;
other.ptr=nullptr;
other.shared_num=nullptr;
return *this;
}
~shared_ptr()
{
    release();
}

//辅助函数
void reset(T*new_ptr=nullptr)
{
    release();
    if(new_ptr)
    {
        ptr=new_ptr;
        shared_num=new int(1);
    }
}

//获取裸指针
T*get()const
{
return ptr;
}
//引用计数
int use_count()const
{
    return shared_num?*shared_num:0;
}

//解引用
T&operator *()const{return *ptr;}
T*operator->()const{return ptr;}
//转换为bool
explicit operator bool()const{return ptr!=nullptr;}

void swap(shared_ptr&other)noexcept{
    std::swap(ptr,other.ptr);
    std::swap(shared_num,other.shared_num);
}
};

struct Test {
    int x;
    explicit Test(int v) : x(v) {}
    ~Test() { std::cout << "Test destroyed\n"; }
};

void test_shared_ptr() {
    shared_ptr<Test> sp1(new Test(42));
    assert(sp1.use_count() == 1);

    shared_ptr<Test> sp2(sp1);         
    assert(sp1.use_count() == 2);
    assert(sp2.use_count() == 2);

    shared_ptr<Test> sp3;
    sp3 = sp1;                         
    assert(sp1.use_count() == 3);

    shared_ptr<Test> sp4 = std::move(sp1); 
    assert(sp1.get() == nullptr);
    assert(sp1.use_count() == 0);
    assert(sp4.use_count() == 3);      

    sp2.reset();                       
    assert(sp4.use_count() == 2);

    sp3.reset(new Test(100));         
    assert(sp4.use_count() == 1);
    std::cout<<"all passed！"<<std::endl;
}

int main() {
    test_shared_ptr();
    return 0;
}