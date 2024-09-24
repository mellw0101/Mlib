#pragma once

#include <map>
#include <string>
#include <vector>

#include "Attributes.h"
#include "def.h"

namespace Mlib::Profile
{
    using std::map;
    using std::string;
    using std::vector;

    /* This class is used to store the profiling statistics of a
     * particular function. It stores the values of the time taken
     * by the function in each call.  It also provides the mean,
     * standard deviation, minimum, maximum and count of the values. */
    class ProfilerStats
    {
    public:
        /* Records the value of the duration of the profiled function. */
        void record(double value);

        /* Calculates the mean of the recorded values. */
        double mean(void) const;

        /* Calculates the standard deviation of the recorded values. */
        double stddev(void) const;

        /* Returns the minimum of the recorded values. */
        double min(void) const;

        /* Returns the maximum of the recorded values. */
        double max(void) const;

        /* Returns the number of values recorded. */
        unsigned long count(void) const;

    private:
        /* The vector of values of the duration of the profiled function. */
        vector<double> values;
    };

    class GlobalProfiler
    {
        map<string, ProfilerStats> stats;
        string                     output_file;
        static GlobalProfiler     *instance;
        GlobalProfiler(void) noexcept;
        static void destroy(void) noexcept;

    public:
        void record(const string &name, double duration);
        void report(void);
        void setOutputFile(STRING_VIEW file_path);

        map<string, ProfilerStats> getStatsCopy(void) const;
        vector<string>             retrveFormatedStrVecStats(void) const;

        static GlobalProfiler *__warn_unused Instance(void) noexcept;

        DEL_CM_CONSTRUCTORS(GlobalProfiler);
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

    /* Sets up the generation of the profiling.
     * This means that the report will be generated at the end of the program.
     * Note that this function must be called before any profiling is done,
     * otherwise the report will not be generated.
     * @param file_path (std::string_view)
     * - The path to the file where the report will be generated. */
    void setupReportGeneration(STRING_VIEW file_path);

} // namespace Mlib::Profile

#define GLOBALPROFILER                Mlib::Profile::GlobalProfiler::Instance()
#define PROFILE_CURRENT_SCOPE(__Name) Mlib::Profile::AutoTimer PROFILE_CURRENT_SCOPE__(__Name)
#define PROFILE_FUNCTION              PROFILE_CURRENT_SCOPE(__FUNCTION__)
#define PROFILE_SCOPE                 PROFILE_CURRENT_SCOPE(__PRETTY_FUNCTION__)
