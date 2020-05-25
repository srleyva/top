#include <algorithm>
#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

vector<string> Process::Stats() {
    vector<string> proc_stat{};

    // parse file
    std::string line;
    string value;

    std::ifstream filestream(this->procLocation_ + LinuxParser::Parser::kStatFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        while(linestream >> value) {
            proc_stat.push_back(value);
        }
    }
    return proc_stat;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    long uptime = LinuxParser::GetLinuxField<long>(LinuxParser::Parser::kProcDirectory + LinuxParser::Parser::kUptimeFilename);
    auto proc_stat = this->Stats(); 
    
    double hertz = sysconf(_SC_CLK_TCK);
    double utime = std::stod(proc_stat[13]);
    double stime = std::stod(proc_stat[14]);
    double cutime = std::stod(proc_stat[15]);
    double cstime = std::stod(proc_stat[16]);
    double startTime = std::stod(proc_stat[21]);
    double totalTime = utime + stime;
    totalTime = totalTime + cutime + cstime;
    double seconds = uptime - (startTime / hertz);
    return ((totalTime/hertz) / seconds);
}

// TODO: Return the command that generated this process
string Process::Command() {
    std::string cmd = LinuxParser::GetLinuxField<string>(this->procLocation_ + LinuxParser::Parser::kCmdlineFilename, '\0');
    return cmd;
}

// TODO: Return this process's memory utilization
string Process::Ram() const {
  int kbRam = LinuxParser::GetLinuxField<int>("VmSize:", this->procLocation_ + LinuxParser::Parser::kStatusFilename);

  std::stringstream mbRam;
  mbRam << std::fixed << std::setprecision(2) << (float)kbRam/1000;
  return mbRam.str();
}

// TODO: Return the user (name) that generated this process
string Process::User() {
    long int uid = LinuxParser::GetLinuxField<int>("Uid:", this->procLocation_ + LinuxParser::Parser::kStatusFilename);
    passwd *user = getpwuid(uid);
    if (!user) {
        return ""; 
    }
    return user->pw_name;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    long uptime = LinuxParser::GetLinuxField<long>(LinuxParser::Parser::kProcDirectory + LinuxParser::Parser::kUptimeFilename);
    long startTime = std::stol(this->Stats()[21])/sysconf(_SC_CLK_TCK);
    return uptime - startTime;
    
}

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
    float proc_1_mem = std::stof(this->Ram());
    float proc_2_mem = std::stof(a.Ram());
    return proc_1_mem > proc_2_mem;
}
