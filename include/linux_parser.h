#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

#include "process.h"
#include "processor.h"
#include "system.h"

namespace LinuxParser {

template <typename T> 
T GetLinuxField(std::string const& field, std::string const& filename, char delim = '\n') {
  std::string line;
  std::string key;
  T value;

  std::ifstream filestream(filename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line, delim)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == field) {
          return value;
        }
      }
    }
  }
  return value;
}

template <typename T> 
T GetLinuxField(std::string const& filename, char delim = '\n') {
  std::string line;
  std::string key;
  T value;

  std::ifstream filestream(filename);
  if (filestream.is_open()) {
    std::getline(filestream, line, delim);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value;
}

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};

class Parser : public OSParser {
public:
  // System Paths
  inline static const std::string kProcDirectory{"/proc/"};
  inline static const std::string kCmdlineFilename{"cmdline"};
  inline static const std::string kCpuinfoFilename{"cpuinfo"};
  inline static const std::string kStatusFilename{"status"};
  inline static const std::string kStatFilename{"stat"};
  inline static const std::string kUptimeFilename{"uptime"};
  inline static const std::string kMeminfoFilename{"meminfo"};
  inline static const std::string kVersionFilename{"version"};
  inline static const std::string kOSPath{"/etc/os-release"};
  inline static const std::string kPasswordPath{"/etc/passwd"};

  Processor& Cpu() {return this->cpu_; }
  std::vector<Process>& Processes();
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();

private:
  Processor cpu_;
  std::vector<Process> processes_ {};

  // System
  std::vector<int> Pids();
  std::vector<std::string> CpuUtilization();
};
};

#endif