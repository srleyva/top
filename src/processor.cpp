#include <fstream>
#include <string>
#include <vector>
#include <numeric>

#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
std::vector<double> Processor::CpuStat() {
    std::vector<double> cpuStat;
    // parse file
    std::string line;
    std::string key;
    double value;

    std::ifstream filestream(LinuxParser::Parser::kProcDirectory + LinuxParser::Parser::kStatFilename);
    if (filestream.is_open()) {
        while(std::getline(filestream, line)) {
            std::istringstream linestream(line);
            while (linestream >> key) {
                if(key == "cpu") {
                    while(linestream >> value) {
                        cpuStat.push_back(value);
                    }
                }
            }
        }
    }
    return cpuStat;
} 


float Processor::Utilization() {
    std::vector<double> cpuStat = this->CpuStat();

    double cpuSum = std::accumulate(cpuStat.begin(), cpuStat.end(), 0.0);
    double cpuDelta = cpuSum - this->lastCpuSum_;
    double cpuIdle = cpuStat[3] - this->oldCpuStat[3];

    double cpuUsed = cpuDelta - cpuIdle;

    double cpuUsage = cpuUsed/cpuDelta;

    this->lastCpuSum_ = cpuSum;
    this->oldCpuStat = cpuStat;
    return cpuUsage;

}