#include<iostream>
#include<string>
#include<fstream>
#include<regex>
#include<vector>
#include<chrono>
#include<algorithm>
#include<iterator>

class LogEntry
{
    public:
   std::string timestamp;
   std::string level;
   std::string message;
   LogEntry(const std::string& a,const std::string& b,const std::string& c):timestamp(a),level(b),message(c){}
   LogEntry()=default;
};

int main()
{
auto start=std::chrono::steady_clock::now();
    std::vector<LogEntry> logs;
    std::ifstream file("log.txt");
    if(!file.is_open())
    {
        std::cerr<<"the file can`t be opened!\n";
        return 1;
    }
     std::cout<<"---------show all the message in text--------"<<std::endl;
    std::string line;
     std::regex pattern(R"(\[([^\]]+)\] \[([^\]]+)\] (.*))");
    //std::regex pattern(R"(\[([^\]]+)\] \[([^\]]+)\] (.*))");
    
    std::smatch match;
    while(getline(file,line))
    {
         //正则表达式

if(std::regex_match (line,match,pattern))
{
logs.push_back(LogEntry(match[1],match[2],match[3]));
std::cout<<
"the hole text is:"<<match[0]<<
"\n"<<
"match[1]:"<<match[1]<<"  "<<
"match[2]:"<<match[2]<<"  "<<
"match[3]:"<<match[3]<<std::endl;
    }
    else
    {
        std::cerr<<"the text is false:"<<line<<std::endl;
    }
}
//find the error
std::cout<<"\n"<<"----------------find the error---------"<<std::endl;
std::cout<<"-----find the first 'error'-----"<<std::endl;
auto it=std::find_if(logs.begin(),logs.end(),[](const LogEntry&a){
    return a.level=="ERROR";
});
if(it!=logs.end())
{
    std::cout<<"find successfully :the line is:"<<it->timestamp<<" "<<it->level<<" "<<it->message<<std::endl;
}
else
{
    std::cout<<"can not find the message"<<std::endl;
}
std::cout<<std::endl;
std::cout<<"--------find all 'error'------------"<<std::endl;
std::vector<LogEntry> errors;
std::copy_if(logs.begin(),logs.end(),std::back_inserter(errors),[](const LogEntry&a){
    return a.level=="ERROR";
});
if(errors.empty())
{
    std::cout<<"can`t find the message which is error"<<std::endl;
}
else{
    std::sort(errors.begin(),errors.end(),[](const LogEntry&a,const LogEntry&b){
        return a.timestamp<b.timestamp;
    });
    std::cout<<"messages have been sorted"<<std::endl;
    for(auto &it:errors)
    {
         std::cout<<"find successfully: "<<it.timestamp<<" "<<it.level<<" "<<it.message<<std::endl;
    }
}
//save the file
std::ofstream out("output.txt");
if(!out.is_open())
return 1;
for(auto&line:errors)
{
    out<<"["<<line.timestamp<<"] "<<"["<<line.level<<"] "<<line.message<<std::endl;
}
auto end=std::chrono::steady_clock::now();
auto duration=std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
std::cout<<"\n"<<"-----all time used---------";
std::cout<<duration.count()<<"ms\n";
    return 0;
}