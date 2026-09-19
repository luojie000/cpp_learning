#include<iostream>
#include<memory>
#include<thread>
#include<vector>
#include<algorithm>
class student{
public:
std::string name;
int age;
int score;
student(std::string s,int a,int b)
{
    name=s;
    age=a;
    score=b;
}
};

int main()
{
    std::cout<<"----------lab 1--------------"<<std::endl;
    int x=10;
    std::unique_ptr<int> ptr=std::make_unique<int>(42);
    auto task=[x,p=std::move(ptr)]()mutable{
        x++;
        std::cout<<"x="<<x<<std::endl;
        std::cout<<"*p="<<*p<<std::endl;
    };
    std::thread t(std::move(task));
    t.join();
    std::cout<<"outer x:"<<x<<std::endl;
    std::cout<<"ptr null="<<(ptr==nullptr)<<std::endl;
std::cout<<"------------lab2--------------"<<std::endl;
std::vector<student> students={
{"a",12,80},
{"b",12,83},
{"c",13,70},
{"d",14,70},
{"e",15,93}
};
std::sort(students.begin(),students.end(),[](const student &a,const student &b){
if(a.score!=b.score)
return a.score<b.score;
return a.age<b.age;
});
std::cout<<"the sort of score(include the age)"<<std::endl;
for(int i=0;i<students.size();i++)
{
    std::cout<<students[i].name<<"  "<<students[i].age<<"  "<<students[i].score<<std::endl;
}
std::cout<<"------------lab3-----------"<<std::endl;
std::cout<<"find the specially score"<<std::endl;
auto it=find_if(students.begin(),students.end(),[](const student&a){
return a.score>=90;
});
if(it!=students.end())
{
    std::cout<<"find the student whose score >=90:"<<it->name<<"  "<<it->age<<"  "<<it->score<<std::endl;
}
else
{
    std::cout<<"didn`t find"<<std::endl;
}
    return 0;
}