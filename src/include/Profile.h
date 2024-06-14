#pragma once

#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include "def.h"

namespace Mlib::Profile {
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
        void record(string const& name, double duration);
        void report(string const& filename);

        [[__nodiscard__("GlobalProfiler::Instance()")]] static GlobalProfiler* const& Instance();

        // Prevent copy-construction and assignment
        DELETE_COPY_CONSTRUCTORS(GlobalProfiler);

        // Prevent move-construction and assignment
        DELETE_MOVE_CONSTRUCTORS(GlobalProfiler);

        // Destructor
        ~GlobalProfiler()
        {}

    private:
        map<string, ProfilerStats> stats;
        static GlobalProfiler*     instance;
        GlobalProfiler()
        {}
    };

    class AutoTimer
    {
    public:
        AutoTimer(string const& name);
        ~AutoTimer();

    private:
        string                                            name;
        chrono::time_point<chrono::high_resolution_clock> start;
    };

    /// @name @c setupReportGeneration
    /// @brief
    /// - Sets up the generation of the profiling
    /// - report at the end of the program.
    /// @returns void
    void setupReportGeneration();
} // namespace Mlib::Profile
