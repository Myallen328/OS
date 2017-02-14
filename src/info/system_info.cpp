#include "system_info.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <set>

using namespace std;

// -----------------DONE----------------
double get_uptime() {
  SystemInfo sissy;

  ifstream memFile("proc/uptime");
  double up;

  memFile >> up;

  return up;
}
// -----------------DONE----------------


bool isNumber(const std::string& s) {

  std::string::const_iterator it = s.begin();
  while (it != s.end() && std::isdigit(*it)) ++it;
  return !s.empty() && it == s.end();

}

SystemInfo get_system_info() {
  SystemInfo sissy;
  sissy.uptime = get_uptime();

  string line;
  string temp;
  unsigned temp_number;

  unsigned numProcesses = 0;
  unsigned numThreads = 0;
  set<string> directoryList;

  DIR * dirp;
  struct dirent * ent;
  dirp = opendir("proc");

  //Go through files
  while ((ent = readdir (dirp)) != NULL) {
    string pid = ent->d_name;
    //printf("%s\n", ent->d_name);
    if (isNumber(pid) && strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
      numProcesses++;

      //Counting threads
      /*string taskDirectory = "proc/" + pid + "/status";
      ifstream pidStatus(taskDirectory.c_str());
      while (getline(pidStatus, line)) {
        istringstream iss(line);
        if (!(iss >> temp >> temp_number)) {
          continue;
        }
        if (temp == "Threads:") {
           numThreads += temp_number;
           //cout << numThreads << " - " << s << endl;
           break;
        }
      }*/

      /*DIR * subDirp;
      struct dirent * subEnt;
      subDirp = opendir(taskDirectory.c_str());
      if (subDirp == NULL) {
        continue;
      }
        while (subEnt = readdir (subDirp)) {
          string p = subEnt->d_name;
        }*/
      }
    }


  sissy.num_processes = numProcesses;
  sissy.num_threads = numThreads;
  sissy.num_user_threads = 0;
  sissy.num_kernel_threads = 0;
  sissy.num_running = 0;

  return sissy;
  //return SystemInfo();
}
