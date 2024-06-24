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

namespace Mlib::Profile
{
    /// @class @c ProfilerStats
    /// @brief
    /// - This class is used to store the profiling
    /// - statistics of a particular function.
    /// - It stores the values of the time taken
    /// - by the function in each call.
    /// - It also provides the mean, standard deviation,
    /// - minimum, maximum and count of the values.
    /// @ingroup @c Mlib::Profiling
    class ProfilerStats
    {
    public:
        /// @name @c record
        /// @brief
        /// - Records the value of the duration
        /// - of the profiled function.
        /// @param value
        /// - The duration of the profiled function.
        /// @returns void
        void record(f64 value);

        /// @name @c mean
        /// @brief
        /// - Calculates the mean of the recorded
        /// - values.
        /// @returns ( double )
        f64 mean() const;

        /// @name @c stddev
        /// @brief
        /// - Calculates the standard deviation of
        /// - the recorded values.
        /// @returns ( double )
        f64 stddev() const;

        /// @name @c min
        /// @brief
        /// - Returns the minimum of the recorded
        /// - values.
        /// @returns ( double )
        f64 min() const;

        /// @name @c max
        /// @brief
        /// - Returns the maximum of the recorded
        /// - values.
        /// @returns ( double )
        f64 max() const;

        /// @name @c count
        /// @brief
        /// - Returns the number of values recorded.
        /// @returns ( size_t )
        size_t count() const;

    private:
        /// @name @c values
        /// TYPE: @b vector<f64>
        /// @brief
        /// - The vector of values of the duration
        /// - of the profiled function.
        std::vector<f64> values;
    };

    class GlobalProfiler
    {
    public:
        void record(std::string const &name, double duration);
        void report(std::string const &filename);

        [[__nodiscard__("GlobalProfiler::Instance()")]]
        static GlobalProfiler *Instance();

        // Prevent copy-construction and assignment
        DELETE_COPY_CONSTRUCTORS(GlobalProfiler);

        // Prevent move-construction and assignment
        DELETE_MOVE_CONSTRUCTORS(GlobalProfiler);

        // Destructor
        ~GlobalProfiler()
        {}

    private:
        std::map<std::string, ProfilerStats> stats;
        static GlobalProfiler               *instance;
        GlobalProfiler()
        {}
    };

    class AutoTimer
    {
    public:
        AutoTimer(std::string const &name);
        ~AutoTimer();

    private:
        std::string                                                 name;
        std::chrono::time_point<std::chrono::high_resolution_clock> start;
    };

    /// @name @c setupReportGeneration
    /// @brief
    /// - Sets up the generation of the profiling
    /// - report at the end of the program.
    /// @returns void
    void setupReportGeneration();
} // namespace Mlib::Profile
