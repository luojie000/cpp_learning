#include<iostream>
#include<string>
#include<fstream>
#include<regex>
#include<vector>
#include<algorithm>
#include<iterator>
#include<chrono>
#include<stdexcept>
#include<string_view>


enum class LogLevel
{
    INFO,
    WARNING,
    ERROR,
    DEBUG,
    UNKNOWN
};


LogLevel stringToLevel(const std::string& level)
{
    if(level=="ERROR")
    {
        return LogLevel::ERROR;
    }
    else if(level=="WARNING")
    {
        return LogLevel::WARNING;
    }
    else if(level=="DEBUG")
    {
        return LogLevel::DEBUG;
    }
    else if(level=="INFO")
    {
        return LogLevel::INFO;
    }
    else{
        return LogLevel::UNKNOWN;
    }
}


std::string levelToString(LogLevel level)
{
    switch(level)
    {
        case LogLevel::ERROR:
            return "ERROR";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
    }
    return "UNKNOWN";
}


class LogEntry
{
public:
    std::string timestamp;
    LogLevel level;
    std::string message;
    LogEntry(const std::string_view& a,LogLevel b,const std::string_view& c):timestamp(a),level(b),message(c)
    {}
    LogEntry()=default;
};




struct LogStatistics
{
    int errorsnum=0;
    int infosnum=0;
    int debugsnum=0;
    int warningsnum=0;
};




class LogParser
{
public:
LogParser(){logs.reserve(100000);}
    //input the filename
    bool parseline(const std::string &str,LogEntry&entity)
    {
        std::string_view view(str);
        size_t index1=view.find(']');
        if(index1==std::string_view::npos)
        {
            return false;
        }
        std::string_view a=view.substr(1,index1-1);
        size_t index2=view.find(']',index1+3);
        if(index2==std::string_view::npos)
        {
            return false;
        }
            std::string_view b=view.substr(index1+3,index2-index1-3);
            LogLevel d=stringToLevel(std::string(b));
        std::string_view c=view.substr(index2+2);
        entity=LogEntry(a,d,c);
        return true;
    }
void parseLogFile(const std::string& filename){
std::ifstream file(filename);
if(!file.is_open()){ 
    throw std::runtime_error("input file cannot open");}

 /*std::regex pattern(R"(\[([^\]]+)\] \[([^\]]+)\] (.*))");
        std::string line;
        */
        std::string line;
 while(std::getline(file,line))
{ 
    //std::smatch match;
    LogEntry temp;
    if(parseline(line,temp))
        {
        LogLevel currentLevel = temp.level;
        logs.emplace_back(std::move(temp));
    if(currentLevel==LogLevel::ERROR)
{
logstatistics.errorsnum++;
}
else if(currentLevel==LogLevel::DEBUG)
{
    logstatistics.debugsnum++;
} 
else if(currentLevel==LogLevel::WARNING)
{
    logstatistics.warningsnum++;
}
else if(currentLevel==LogLevel::INFO)
{
    logstatistics.infosnum++;
}
        }
    else{
        std::cerr<<"Invalid line: "<<line<<'\n';} 
}

}
    //find the message
    std::vector<LogEntry> findMessages(LogLevel level)
    {
        std::vector<LogEntry> messages;
        std::copy_if(logs.begin(),logs.end(),std::back_inserter(messages),[level](const LogEntry& log)
            {
                return log.level==level;
            }
        );
        return messages;
    }

    void PrintLogStatistics()
    {
        std::cout<<"the num of errors:"<<logstatistics.errorsnum<<"\n";
        std::cout<<"the num of infos:"<<logstatistics.infosnum<<"\n";
        std::cout<<"the num of debugs:"<<logstatistics.debugsnum<<"\n";
        std::cout<<"the num of warnings:"<<logstatistics.warningsnum<<"\n";
    }
    //sort the logs

    void sortByTime(std::vector<LogEntry>& target)
    {
        std::sort(target.begin(),target.end(),[](const LogEntry& a,const LogEntry& b)
            {
                return a.timestamp<b.timestamp;
            }

        );
    }

    //save the message
    void writeLogs(const std::string& filename,const std::vector<LogEntry>& target)
    {
        std::ofstream out(filename);
        if(!out.is_open())
        {
            throw std::runtime_error("output file cannot open");
        }
        for(const auto& log:target)
        {out<<"["<<log.timestamp<<"] "<<"["<<levelToString(log.level)<<"] "
            <<log.message<<'\n';
        }
    }

    void printlogs()
    {
        std::cout<<"the total logs:"
        <<logs.size()<<"\n";
    }


LogStatistics logstatistics;
private:
    std::vector<LogEntry> logs;
};

int main()
{
    LogParser parser;
    auto start =std::chrono::steady_clock::now();
    try
    {
        parser.parseLogFile("log.txt");
        parser.printlogs();
        //找到ERROR日志
        auto errors =parser.findMessages(LogLevel::ERROR);
        std::cout<<"the total errors:"<<parser.logstatistics.errorsnum<<"\n";
        //排序ERROR日志
        parser.sortByTime(errors);
        //保存ERROR日志
        parser.writeLogs("filtered.log",errors);

        std::cout<<"Output successfully\n";
        std::cout<<"the all statistics:\n";
        std::cout<<"\n";
        parser.PrintLogStatistics();
    }
    catch(const std::exception& e)
    {

        std::cerr<<"Exception: "<<e.what()<<'\n';
        return 1;
    }
    auto end =std::chrono::steady_clock::now();

    auto duration =std::chrono::duration_cast<std::chrono::milliseconds>(end-start);

    std::cout<<"\n=========== Time ==========\n"
    <<"Total time: "<<duration.count()<<" ms\n";
    return 0;

}