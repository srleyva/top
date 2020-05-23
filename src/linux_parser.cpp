#include <dirent.h>
#include <algorithm>
#include <unistd.h>
#include <string>
#include <vector>
#include <fstream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

std::vector<Process>& LinuxParser::Parser::Processes() { 
  //clear old processes
  this->processes_ = std::vector<Process>{};

  for (int pid: this->Pids()) {
    Process process(pid);
    this->processes_.push_back(process);
  }
  std::sort(this->processes_.begin(), this->processes_.end());
  return this->processes_; 
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Parser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Parser::Kernel() {
  string os, kernel, _;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> _ >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Parser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::Parser::MemoryUtilization() {
  float memTotal = GetLinuxField<int>("MemTotal:", kProcDirectory + kMeminfoFilename);
  float memFree = GetLinuxField<int>("MemFree:", kProcDirectory + kMeminfoFilename);
  return (memTotal-memFree)/memTotal;
}

long LinuxParser::Parser::UpTime() {
  return GetLinuxField<long>(kProcDirectory + kUptimeFilename);
}

// TODO: Read and return the total number of processes
int LinuxParser::Parser::TotalProcesses() {
  return GetLinuxField<int>("processes", kProcDirectory + kStatFilename);
}

// TODO: Read and return the number of running processes
int LinuxParser::Parser::RunningProcesses() {
  return GetLinuxField<int>("procs_running", kProcDirectory + kStatFilename);
}