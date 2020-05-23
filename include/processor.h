#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  std::vector<double> CpuStat();
 private:
  double lastCpuSum_ = 0.0;
  std::vector<double> oldCpuStat {0,0,0,0,0,0,0,0};
};

#endif