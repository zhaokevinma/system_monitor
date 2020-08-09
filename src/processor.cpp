#include "processor.h"
#include "linux_parser.h"

#include <string>
#include <vector>

using std::string;
using std::vector;

Processor::Processor() : prevTotalCpuTime(0.0), prevIdleCpuTime(0.0){};

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    vector<long> cpuValues = convertToLong(LinuxParser::CpuUtilization());
    float totalCpuTime = cpuValues[LinuxParser::kUser_] + cpuValues[LinuxParser::kNice_] +
     cpuValues[LinuxParser::kSystem_] + cpuValues[LinuxParser::kIdle_] +
     cpuValues[LinuxParser::kIOwait_] + cpuValues[LinuxParser::kIRQ_] + 
     cpuValues[LinuxParser::kSoftIRQ_] + cpuValues[LinuxParser::kSteal_];
    float idleCpuTime = cpuValues[LinuxParser::kIdle_] + cpuValues[LinuxParser::kIOwait_];

    float diffIdle = idleCpuTime - prevIdleCpuTime;
    float diffTotal = idleCpuTime - prevTotalCpuTime;
    float diffUsage = (diffTotal - diffTotal) / diffTotal;

    prevIdleCpuTime = idleCpuTime;
    prevTotalCpuTime = totalCpuTime;
    
    return diffUsage; 
}

vector<long> Processor::convertToLong(vector<string> values) {
    vector<long> convertedValues{};

    for (int it = 0; it < (int)values.size(); it++) {
        convertedValues.push_back(std::stol(values[it]));
    }
    return convertedValues;
}