#include<iostream>
#include<vector>
#include<algorithm>
#include<memory>
#include<string>

template<typename T,typename compare>
void quicksortimpl(std::vector<T> &v,int left,int right,compare comp){
if(left>=right)return;
int i=left,j=right;
T priot=v[(left+right)/2];
while(i<=j)
{
while(comp(v[i],priot))
{
    i++;
}
while(comp(priot,v[j]))
{
    j--;
}
if(i<=j)
{
    std::swap(v[i],v[j]);
    i++;
    j--;
}
}
if(left<j)
quicksortimpl(v,left,j,comp);
if(i<right)
quicksortimpl(v,i,right,comp);
}

template<typename T,typename compare>
void quicksort(std::vector<T>&v,compare comp)
{
    if(v.empty())
    return;
quicksortimpl(v,0,static_cast<int>(v.size())-1,comp);
}

class student{
public:
std::string name;
int age;
int score;
student(std::string a,int b,int c):name(a),age(b),score(c){}
};
int main()
{
std::vector<student> studs={
    {"a",10,90},
    {"b",11,89},
    {"c",13,79},
    {"d",12,68},
    {"e",15,93}
};
quicksort(studs,[](const student&a,const student&b){
    if(a.score!=b.score)
    return a.score<b.score;
return a.age<b.age;
});
for(int i=0;i<studs.size();i++)
{
    std::cout<<studs[i].name<<"  "<<studs[i].age<<"  "<<studs[i].score<<std::endl;
}
    return 0;
}