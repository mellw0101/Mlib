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
    private:
        std::map<std::string, ProfilerStats> stats;
        std::string                          output_file;
        static GlobalProfiler               *instance;

        GlobalProfiler();

    public:
        void record(const std::string &name, double duration);
        void report();
        void destroy();
        void setOutputFile(std::string_view file_path);

        [[__nodiscard__("GlobalProfiler::Instance()")]]
        static GlobalProfiler *Instance();

        DELETE_COPY_AND_MOVE_CONSTRUCTORS(GlobalProfiler);
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

    //
    //  Sets up the generation of the profiling.
    //  This means that the report will be generated at the end of the program.
    //  Note that this function must be called before any profiling is done,
    //  otherwise the report will not be generated.
    //  The report is generated in the file path
    //
    void setupReportGeneration(std::string_view file_path);

} // namespace Mlib::Profile
