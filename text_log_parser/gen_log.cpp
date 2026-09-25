#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>

int main() {
    std::ofstream out("log.txt");
    if (!out.is_open()) {
        std::cerr << "无法创建 log.txt\n";
        return 1;
    }

    std::vector<std::string> levels = {"INFO", "WARNING", "ERROR", "DEBUG"};
    std::vector<std::string> messages = {
        "User login successful",
        "Database connection failed",
        "Cache miss",
        "File not found",
        "Network timeout",
        "Service started",
        "Configuration reloaded",
        "Authentication failed",
        "Disk space low",
        "Request processed",
        "Database failed",
        "Permission denied",
        "Connection reset by peer",
        "Task completed",
        "Memory usage high"
    };

    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> levelDist(0, levels.size() - 1);
    std::uniform_int_distribution<> msgDist(0, messages.size() - 1);

    auto start = std::chrono::system_clock::now();

    for (int i = 0; i < 10000; ++i) {
        auto t = start + std::chrono::seconds(i);
        std::time_t tt = std::chrono::system_clock::to_time_t(t);
        std::tm tm = *std::localtime(&tt);

        out << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "] "
            << "[" << levels[levelDist(gen)] << "] "
            << messages[msgDist(gen)] << "\n";
    }

    out.close();
    std::cout << "已生成 log.txt，共 20000 条日志\n";
    return 0;
}