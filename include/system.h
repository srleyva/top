#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "processor.h"

// Define Contract for OS Interaction
// Abstract OS type away from the System class
class OSParser {
public:
  virtual Processor& Cpu() = 0;
  virtual std::vector<Process>& Processes() = 0;
  virtual float MemoryUtilization() = 0;
  virtual long UpTime() = 0;
  virtual int TotalProcesses() = 0;
  virtual int RunningProcesses() = 0;
  virtual std::string Kernel() = 0;
  virtual std::string OperatingSystem() = 0;
};

class System {
 public:
  // Inject the OSParser and utilize composition
  System(OSParser *osParser) : osParser_(osParser) {}
  Processor& Cpu() { return this->osParser_->Cpu(); };                   // TODO: See src/system.cpp
  std::vector<Process>& Processes() { return this->osParser_->Processes(); };  // TODO: See src/system.cpp
  float MemoryUtilization() { return this->osParser_->MemoryUtilization(); };          // TODO: See src/system.cpp
  long UpTime() { return this->osParser_->UpTime(); };                      // TODO: See src/system.cpp
  int TotalProcesses() { return this->osParser_->TotalProcesses(); };               // TODO: See src/system.cpp
  int RunningProcesses() { return this->osParser_->RunningProcesses(); };             // TODO: See src/system.cpp
  std::string Kernel() { return this->osParser_->Kernel(); };               // TODO: See src/system.cpp
  std::string OperatingSystem() { return this->osParser_->OperatingSystem(); };      // TODO: See src/system.cpp

 private:
  OSParser *osParser_;
};

#endif