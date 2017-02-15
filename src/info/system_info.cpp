#include "system_info.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

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
  int fileLength;
  char temp_char;
  unsigned temp_number;

  unsigned numProcesses = 0;
  unsigned numThreads = 0;
  unsigned numKernel = 0;
  unsigned numUser = 0;
  unsigned numRunning = 0;

  DIR * dirp;
  struct dirent * ent;
  dirp = opendir("proc");

  //Go through files
  while ((ent = readdir (dirp)) != NULL) {
    string pid = ent->d_name;

    //Have we found a process, if so, act on it
    if (isNumber(pid) && strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {

      //Increment process counter
      numProcesses++;

      //Open status file
      string statusFile = "proc/" + pid + "/status";
      ifstream pidStatus(statusFile.c_str());

      //Determine if process is running
      //Check if cwd foler exists
      struct stat sb;
      string cwdPath = "proc/" + pid + "/cwd";
      if (stat(cwdPath.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {

        //If it exists, check to make sure the process is actually running
        while (getline(pidStatus, line)) {
          istringstream iss(line);
          if (!(iss >> temp >> temp_char)) {
            continue;
          }
          if (temp == "State:") {
             if(temp_char == 'R') {
               numRunning++;
             }
             //cout << numKernel << " - " << pid << endl;
             break;
          }
        }

      }

      //Return to beginning of file for reading again
      pidStatus.clear();
      pidStatus.seekg(0, ios::beg);


      //Determine if process is Kernel/User
      string cmdFile = "proc/" + pid + "/cmdline";
      ifstream pidCmd(cmdFile.c_str());
      pidCmd.seekg(0, ios::end);
      fileLength = pidCmd.tellg();
      pidCmd.close();


      //If Kernel thread
      if (fileLength == 0) {
        while (getline(pidStatus, line)) {
          istringstream iss(line);
          if (!(iss >> temp >> temp_number)) {
            continue;
          }
          if (temp == "Threads:") {
             numKernel += temp_number;
             //cout << numKernel << " - " << pid << endl;
             break;
          }
        }
      }
      //If User thread
      else {
        while (getline(pidStatus, line)) {
          istringstream iss(line);
          if (!(iss >> temp >> temp_number)) {
            continue;
          }
          if (temp == "Threads:") {
             numUser += temp_number;
             //cout << numUser << " - " << pid << endl;
             break;
          }
        }
        numUser--;
      }

      pidStatus.close();
      }
    }


  sissy.num_processes = numProcesses;
  numThreads = numUser + numKernel;
  sissy.num_threads = numThreads;
  sissy.num_user_threads = numUser;
  sissy.num_kernel_threads = numKernel;
  sissy.num_running = numRunning;

  sissy.memory_info = get_memory_info();
  sissy.load_average = get_load_average();
  sissy.cpus = get_cpu_info();
  sissy.processes = get_all_processes("proc");

  return sissy;
  //return SystemInfo();
}
