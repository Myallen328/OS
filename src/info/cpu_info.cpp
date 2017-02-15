#include "cpu_info.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;


vector<CpuInfo> get_cpu_info() {
  CpuInfo cpu;
  vector<CpuInfo> allCpu;

  string temp;
  string line;
  unsigned long long userTime;
  unsigned long long niceTime;
  unsigned long long systemTime;
  unsigned long long idleTime;
  unsigned long long ioWaitTime;
  unsigned long long irqTime;
  unsigned long long softIrqTime;
  unsigned long long stealTime;
  unsigned long long guestTime;
  unsigned long long guestNiceTime;


  ifstream cpuFile("proc/stat");

  while (getline(cpuFile, line)) {
    istringstream iss(line);

    if (!(iss >> temp >> userTime >> niceTime >> systemTime >> idleTime >>
      ioWaitTime >> irqTime >> softIrqTime >> stealTime >> guestTime >>
      guestNiceTime)) {
      continue;
    } else if (temp.substr(0,3) == "cpu"){
      //cout << temp << endl;
      cpu.user_time = userTime;
      cpu.nice_time = niceTime;
      cpu.system_time = systemTime;
      cpu.idle_time = idleTime;
      cpu.io_wait_time = ioWaitTime;
      cpu.irq_time = irqTime;
      cpu.softirq_time = softIrqTime;
      cpu.steal_time = stealTime;
      cpu.guest_time = guestTime;
      cpu.guest_nice_time = guestNiceTime;

      allCpu.push_back(cpu);
    }

  }


  return allCpu;
}


CpuInfo operator -(const CpuInfo& lhs, const CpuInfo& rhs) {
  CpuInfo newCpu;
  newCpu.user_time = lhs.user_time - rhs.user_time;
  newCpu.nice_time = lhs.nice_time - rhs.nice_time;
  newCpu.idle_time = lhs.idle_time - rhs.idle_time;
  newCpu.io_wait_time = lhs.io_wait_time - rhs.io_wait_time;
  newCpu.irq_time = lhs.irq_time - rhs.irq_time;
  newCpu.softirq_time = lhs.softirq_time - rhs.softirq_time;
  newCpu.steal_time = lhs.steal_time - rhs.steal_time;
  newCpu.guest_time = lhs.guest_time - rhs.guest_time;
  newCpu.guest_nice_time = lhs.guest_nice_time - rhs.guest_nice_time;

  return newCpu;
  //return CpuInfo();
}
