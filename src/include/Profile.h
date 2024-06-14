#ifndef PROF_HPP
#define PROF_HPP

#include <chrono>
#include <fstream>
#include <ios>
#include <iostream>
#include <map>
#include <string>
#include <vector>
// #include <memory>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>


#include "def.h"


/// @class ProfilerStats
/// @brief
/// - This class is used to store the profiling
/// - statistics of a particular function.
/// - It stores the values of the time taken
/// - by the function in each call.
/// - It also provides the mean, standard deviation,
/// - minimum, maximum and count of the values.
class ProfilerStats
{
public:
    void   record(double value);
    double mean() const;
    double stddev() const;
    double min() const;
    double max() const;
    size_t count() const;

private:
    vector<double> values;
};

class GlobalProfiler
{
public:
    /* static GlobalProfiler& getInstance()
    {
        static GlobalProfiler instance;
        return instance;
    } */
    void                   record(const std::string& name, double duration);
    void                   report(const std::string& filename);
    static GlobalProfiler* createNewGprof();

private:
    map<string, ProfilerStats> stats;
    GlobalProfiler()
    {}
};
static GlobalProfiler* gProf(nullptr);

void init_gProf();

class AutoTimer
{
public:
    AutoTimer(const std::string& name); /*  : name(name), start(std::chrono::high_resolution_clock::now()) {} */
    ~AutoTimer();

private:
    std::string                                                 name;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

// Register at-exit handler to generate the report
void setupReportGeneration();

void setupVulkanReportGen();

#endif /* PROF_HPP */
