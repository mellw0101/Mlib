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
        //
        //  Records the value of the duration of the profiled function.
        //  @param value ( f64 )
        //  - The duration of the profiled function.
        //  @returns void
        //
        void record(f64 value);

        //
        //  Calculates the mean of the recorded
        //  values.
        //
        //  @return f64 ( double )
        //
        f64 mean() const;

        //
        //  Calculates the standard deviation of the recorded values.
        //  @return ( f64 )
        //
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
        u64 count() const;

    private:
        /// @name @c values
        /// TYPE: @b vector<f64>
        /// @brief
        /// - The vector of values of the duration
        /// - of the profiled function.
        VECTOR<f64> values;
    };

    class GlobalProfiler
    {
    private:
        MAP<STRING, ProfilerStats> stats;
        static GlobalProfiler     *instance;

        STRING output_file;

        GlobalProfiler();

    public:
        void record(C_STRING &name, f64 duration);
        void report();
        void destroy();
        void setOutputFile(STRING_VIEW file_path);

        MAP<STRING, ProfilerStats> getStatsCopy() const;
        VECTOR<STRING>             retrveFormatedStrVecStats() const;

        [[__nodiscard__("GlobalProfiler::Instance()")]]
        static GlobalProfiler *Instance();

        DELETE_COPY_AND_MOVE_CONSTRUCTORS(GlobalProfiler);
    };

    class AutoTimer
    {
    public:
        AutoTimer(const std::string &name);
        ~AutoTimer();

    private:
        STRING name;

        TIME_POINT<HIGH_RES_CLOCK> start;
    };

    //
    //  Sets up the generation of the profiling.
    //  This means that the report will be generated at the end of the program.
    //  Note that this function must be called before any profiling is done,
    //  otherwise the report will not be generated.
    //  @param file_path (std::string_view)
    //  - The path to the file where the report will be generated.
    //
    void setupReportGeneration(STRING_VIEW file_path);

} // namespace Mlib::Profile

#define GLOBALPROFILER                Mlib::Profile::GlobalProfiler::Instance()
#define PROFILE_CURRENT_SCOPE(__Name) Mlib::Profile::AutoTimer PROFILE_CURRENT_SCOPE__(__Name)
#define PROFILE_FUNCTION              PROFILE_CURRENT_SCOPE(__FUNCTION__)
#define PROFILE_SCOPE                 PROFILE_CURRENT_SCOPE(__PRETTY_FUNCTION__)
