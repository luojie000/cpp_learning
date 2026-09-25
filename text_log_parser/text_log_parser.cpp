#include<iostream>
#include<string>
#include<fstream>
#include<regex>
#include<vector>
#include<algorithm>
#include<iterator>
#include<chrono>
#include<stdexcept>

class LogEntry
{
public:
    std::string timestamp;
    std::string level;
    std::string message;
    LogEntry(const std::string& a,const std::string& b,const std::string& c):timestamp(a),level(b),message(c)
    {}
};

std::vector<LogEntry> parseLogFile(const std::string& filename)
{
    std::vector<LogEntry> logs;
    std::ifstream file(filename);
    if(!file.is_open())
    {
        throw std::runtime_error("input file cannot open");
    }

    std::regex pattern(R"(\[([^\]]+)\] \[([^\]]+)\] (.*))");
//std::regex pattern(R"(\[([\^]]+)\] \[([\^]]+)\] (.*))");
    std::string line;
    while(std::getline(file,line))
    {
        std::smatch match;
//std::smatch match;
//if(regex_match(line,match,pattern))

        if(std::regex_match( line,match,pattern))
        {
            logs.emplace_back(match[1].str(),match[2].str(),match[3].str());
        }
        else
        {
            std::cerr<<"Invalid line: "<<line<<'\n';
        }
    }
    return logs;
}

//find the errors
std::vector<LogEntry> findErrors(const std::vector<LogEntry>& logs)
{
 std::vector<LogEntry> errors;
 /*
 std::copy_if(logs.begin(),logs.end(),back_inserter(errors),[](const logentry&a){
 return a.level=="erorr"});
 */
    std::copy_if(logs.begin(),logs.end(),std::back_inserter(errors),[](const LogEntry& log)
        {
return log.level=="ERROR";
        });
    return errors;
}

//sort the logs
void sortByTime(std::vector<LogEntry>& logs)
{
    std::sort(logs.begin(),logs.end(),[](const LogEntry& a,const LogEntry& b)
        {
return a.timestamp<b.timestamp;
        }
    );
}

//save the message
void writeLogs(const std::string& filename,const std::vector<LogEntry>& logs)
{
std::ofstream out(filename);
    if(!out.is_open())
    {
throw std::runtime_error("output file cannot open");
    }
    for(const auto& log:logs)
    {
        out<<"["<<log.timestamp<<"] "
        <<"["<<log.level<<"] "<<log.message<<'\n';
    }
}

int main()
{
    auto start =std::chrono::steady_clock::now();
    try{
        std::vector<LogEntry> logs =
        parseLogFile("log.txt");
        std::cout<<"Total logs: "<<logs.size()<<'\n';

        std::vector<LogEntry> errors =findErrors(logs);
        std::cout<<"ERROR logs: "<<errors.size()<<'\n';

        sortByTime(errors);
        writeLogs("filtered.log",errors);
        std::cout<<"Output successfully\n";
    }
    catch(const std::exception& e)
    {
        std::cerr<<"Exception: "<<e.what()<<'\n';
        return 1;

    }

    auto end =std::chrono::steady_clock::now();
    auto duration =std::chrono::duration_cast<std::chrono::milliseconds>( end-start);

    std::cout<<"\n=========== Time ==========\n"<<"Total time: "<<duration.count()<<" ms\n";
    return 0;

}