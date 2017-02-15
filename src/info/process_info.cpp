#include "process_info.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <ctype.h>
#include <vector>
#include <stdio.h>


using namespace std;


ProcessInfo get_process(int pid, const char* basedir) {

  ProcessInfo process;

  //Convert given parameters to strings for ease of use
  /*string proc(basedir, 255);
  ostringstream temp;
  temp << pid;
  string newPid = temp.str();*/

  //string for statm file (destination)
  stringstream statmPath;
  statmPath << basedir << "/" << pid << "/statm";
  string statm = statmPath.str();
  //Open the file
  ifstream statmFile(statm.c_str());

  //Declare variables for use inside while loop
  string line;
  long memorySize;
  long residentSize;
  long sharedSize;

  while (getline(statmFile, line)) {
    istringstream iss(line);

    if (!(iss >> memorySize >> residentSize >> sharedSize)) {
      continue;
    }
    //cout << memorySize;
    process.size = memorySize;
    process.resident = residentSize;
    process.share = sharedSize;

  }

  statmFile.close();

  //string for stat file (destination)
  stringstream statPath;
  statPath << basedir << "/" << pid << "/stat";
  string stat = statPath.str();
  //Open the file
  ifstream statFile(stat.c_str());

  statFile >> process.pid >> process.comm >> process.state >> process.ppid >> process.pgrp
  >> process.session >> process.tty_nr >> process.tpgid >> process.flags >> process.minflt
   >> process.cminflt >> process.majflt >> process.cmajflt >> process.utime >> process.stime
   >> process.cutime >> process.cstime >> process.priority >> process.nice >> process.num_threads
   >> process.itrealvalue >> process.starttime >> process.vsize >> process.rss >> process.rsslim
   >> process.startcode >> process.endcode >> process.startstack >> process.kstkesp >> process.kstkeip
   >> process.signal >> process.blocked >> process.sigignore >> process.sigcatch >> process.wchan >> process.nswap
   >> process.cnswap >> process.exit_signal >> process.processor >> process.rt_priority >>
   process.policy >> process.delayacct_blkio_ticks >> process.guest_time >> process.cguest_time;

  //stime = utime;

  statFile.close();
  //Command line stuff
  char *cmdline_fn = (char *) alloca(strlen(basedir) + 21);
  sprintf(cmdline_fn, "%s/%d/cmdline", basedir, pid);
  FILE *cmdline_fp = fopen(cmdline_fn, "r");
  if (!cmdline_fp) {
    return process;
  }
  char c;
  while ((c = fgetc(cmdline_fp)) != EOF) {
    if (c) process.command_line.push_back(c);
    else process.command_line.push_back(' ');
  }

  if (!process.command_line.length())
    process.command_line = std::string(process.comm + 1);
    process.command_line.pop_back();
    fclose(cmdline_fp);

    char *taskdir = (char *) alloca(strlen(basedir) + 18);
    sprintf(taskdir, "%s/%d/task", basedir, pid);
    process.threads = get_all_processes(taskdir);
    for (size_t i = 0; i < process.threads.size(); i++) {
      process.threads[i].tgid = pid;
    }

  return process;
}


vector<ProcessInfo> get_all_processes(const char* basedir) {

  vector<ProcessInfo> allProcesses;
  DIR * dp;
  struct dirent * dirp;
  if ((dp = opendir(basedir)) == NULL)
    return vector<ProcessInfo>();
  while ((dirp = readdir(dp)) != NULL) {
    for (char  *p = dirp->d_name; *p; p++)
      if (!isdigit(*p)) goto while_cont;
    int pid;
    sscanf(dirp->d_name, "%d", &pid);
    allProcesses.push_back(get_process(pid, basedir));
    while_cont:
    ;
  }
  closedir(dp);
  return allProcesses;

}
