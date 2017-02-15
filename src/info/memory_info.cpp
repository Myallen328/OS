#include "memory_info.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;


MemoryInfo get_memory_info() {

  MemoryInfo memDawg;

  unsigned long long temp_number;

  string temp;
  string line;

  ifstream memFile("proc/meminfo");

  while (getline(memFile, line)) {
  istringstream iss(line);

   if (!(iss >> temp >> temp_number)) {
     continue;
   }
   if (temp == "MemTotal:") {
      memDawg.total_memory = temp_number;
      continue;
    }
   if (temp == "MemFree:") {
      memDawg.free_memory = temp_number;
      continue;
    }
   if (temp == "Buffers:") {
      memDawg.buffers_memory = temp_number;
      continue;
   }
   if (temp == "Cached:") {
      memDawg.cached_memory = temp_number;
      continue;
   }
   if (temp == "SwapTotal:") {
      memDawg.total_swap = temp_number;
      continue;
   }
   if (temp == "SwapFree:") {
      memDawg.free_swap = temp_number;
      break;
   }
 }

  return memDawg;
  //return MemoryInfo();
}
