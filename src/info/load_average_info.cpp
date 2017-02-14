#include "load_average_info.h"
#include <fstream>
#include <iostream>

using namespace std;


LoadAverageInfo get_load_average() {
  LoadAverageInfo avgInfoStruct;
  FILE* avgInfo;
  avgInfo = fopen("proc/loadavg", "r");
  double one;
  double five;
  double fifteen;
  fscanf(avgInfo, "%lf %lf %lf", &one, &five, &fifteen);
  avgInfoStruct.one_min = one;
  avgInfoStruct.five_mins = five;
  avgInfoStruct.fifteen_mins = fifteen;
  return avgInfoStruct;
  //return LoadAverageInfo();
}
