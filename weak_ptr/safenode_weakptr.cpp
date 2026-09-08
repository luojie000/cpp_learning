#include<iostream>
#include<utility>
#include<memory>
class safenode{
public:
int data;
std::shared_ptr<safenode> next;
std::weak_ptr<safenode> prev;
public:
explicit safenode(int val):data(val)
{
std::cout<<"构造 safenode val:"<<data<<std::endl;
}
~safenode()
{
    std::cout<<"析构successfully"<<std::endl;
}

void printprev()
{
    if(auto sp=prev.lock())//lock尝试提升为shared_ptr
    {
        std::cout<<"prev value："<<sp->data<<std::endl;
    }
    else
    std::cout<<"prev is already dead"<<std::endl;
}
};

void test_weakcycle()
{
    std::cout<<"测试weak——ptr打破循环"<<std::endl;
    auto node1=std::make_shared<safenode>(1);
    auto node2=std::make_shared<safenode>(2);
    node1->next=node2;
    node2->prev=node1;
    std::cout<<"node1 use_count:"<<node1.use_count()<<std::endl;
    std::cout<<"node2 use_count:"<<node2.use_count()<<std::endl;
    node2->printprev();
    std::cout<<"函数结束，析构函数执行"<<std::endl;
}

int main()
{
    test_weakcycle();
    return 0;
}