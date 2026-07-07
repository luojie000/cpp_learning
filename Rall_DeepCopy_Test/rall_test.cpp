#include<iostream>
#include<vector>
#include<chrono>
using namespace std;
class intarray{
    private:
    int *data;
    size_t size;
    public:
    explicit intarray(size_t n):size(n),data(new int[n])
    {
        cout<<"构造函数 n:"<<n<<endl;
        for(size_t i=0;i<n;++i)
        {
            data[i]=0;
        }
    }
    ~intarray()
    {
        cout<<"析构函数 n:"<<size<<endl;
        delete[]data;
    }

    //深拷贝
    intarray(const intarray&other):size(other.size),data(new int[other.size])
    {
        cout<<"深拷贝 n:"<<size<<endl;
        for(size_t i=0;i<size;++i)
        {
            data[i]=other.data[i];
        }
    }
    //深拷贝：赋值运算符
    intarray& operator=(const intarray&other)
    {
cout<<"拷贝赋值 n:"<<other.size<<endl;
if(this!=&other)
{
delete[]data;
size=other.size;
data=new int[other.size];
for(size_t i=0;i<size;++i)
{
data[i]=other.data[i];
}
}
return *this;
    }

    //获取大小
    size_t getsize()const {
        return size;
    }

    //移动构造
    intarray(intarray&&other)noexcept:size(other.size),data(other.data)
    {
        cout<<"移动构造:size"<<size<<endl;
        other.data=nullptr;
        other.size=0;
    }
    //移动构造赋值运算
    intarray&operator=(intarray&&other)noexcept
    {
        cout<<"移动赋值:size"<<other.size<<endl;
        if(this!=&other)
        {
            delete[]data;
            data=other.data;
            size=other.size;
            other.data=nullptr;
            other.size=0;
        }
        return *this;
    }
};

void testdeepcopy()
{
    const int n=100;
    const int array_size=10000;
    vector<intarray> vec;
    vec.reserve(n);
    auto start=chrono::steady_clock::now();
    for(int i=0;i<n;++i)
    {
        vec.push_back(intarray(array_size));
    }
    auto end=chrono::steady_clock::now();
    auto ms=chrono::duration_cast<chrono::milliseconds>(end-start).count();
    cout<<"深拷贝or移动耗时:"<<ms<<endl;

}
int main()
{
    //移动构造在深拷贝之前：noexcept保证可以移动构造
    testdeepcopy();
    return 0;
}