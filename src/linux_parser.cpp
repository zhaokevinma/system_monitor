#include <dirent.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "linux_consts.h"
#include "linux_helper.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Get Operating Systsem information by reading data from the filesystem
string LinuxParser::OperatingSystem() {
  string line, key, value;
  std::ifstream filestream(LinuxConsts::kOSPath);
  if (filestream.is_open()) {
    // This is a muti-line file and a while loop keeps reading in lines
    while (std::getline(filestream, line)) {
      // Swap out space in-between words in key to underscore
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        // "PRETTY_NAME" is the key we are looking for and here we return the value token
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// Get Kernel information
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(LinuxConsts::kProcDirectory +
                       LinuxConsts::kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// Get Pid information
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(LinuxConsts::kProcDirectory.c_str());
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

// Get Memory information
float LinuxParser::MemoryUtilization() {
  float memTotal = LinuxHelper::GetValueByKey<int>(
      LinuxConsts::filterMemTotalString, LinuxConsts::kMeminfoFilename);
  float memFree = LinuxHelper::GetValueByKey<int>(
      LinuxConsts::filterMemFreeString, LinuxConsts::kMeminfoFilename);
  float memory = (memTotal - memFree) / memTotal;
  return memory;
}

// Get Up-time information
long LinuxParser::UpTime() {
  string line;
  long upTime = LinuxHelper::GetValue<long>(LinuxConsts::kUptimeFilename);
  return upTime;
}

// Get the total number of processes
int LinuxParser::TotalProcesses() {
  return LinuxHelper::GetValueByKey<int>(LinuxConsts::filterProcesses,
                                          LinuxConsts::kStatFilename);
}

// Get actively running processes
int LinuxParser::RunningProcesses() {
  return LinuxHelper::GetValueByKey<int>(LinuxConsts::filterRunningProcesses,
                                          LinuxConsts::kStatFilename);
}

// Get user information by UID
string LinuxParser::UserByUID(int UID) {
  string line, user, x;
  int fileUid;
  std::ifstream filestream(LinuxConsts::kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> fileUid) {
        if (fileUid == UID) {
          return user;
        }
      }
    }
  }
  return user;
}