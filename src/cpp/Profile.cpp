/** @file Profile.cpp
 * Contains the implementation of the profiling classes and functions. */
#include "../include/Profile.h"

using namespace std;
using namespace chrono;

namespace Mlib::Profile
{
    /** @class @c ProfilerStats */

    void
    ProfilerStats::record(const double value)
    {
        values.push_back(value);
    }

    double
    ProfilerStats::mean(void) const
    {
        if (values.empty())
        {
            return 0.0;
        }
        double sum = std::accumulate(values.begin(), values.end(), 0.0);
        return sum / values.size();
    }

    double
    ProfilerStats::stddev(void) const
    {
        if (values.size() < 2)
        {
            return 0.0;
        }
        double mean_val = mean();
        double sq_sum =
            std::accumulate(values.begin(), values.end(), 0.0,
                            [mean_val](double a, double b)
                            {
                                return a + (b - mean_val) * (b - mean_val);
                            });
        return std::sqrt(sq_sum / values.size());
    }

    double
    ProfilerStats::min(void) const
    {
        return values.empty() ? 0.0
                              : *std::min_element(values.begin(), values.end());
    }

    double
    ProfilerStats::max(void) const
    {
        return values.empty() ? 0.0
                              : *std::max_element(values.begin(), values.end());
    }

    unsigned long
    ProfilerStats::count(void) const
    {
        return values.size();
    }

    /** @class @c GlobalProfiler */

    GlobalProfiler *GlobalProfiler::instance = nullptr;

    GlobalProfiler ::GlobalProfiler(void)
    {}

    void
    GlobalProfiler::destroy(void)
    {
        if (instance != nullptr)
        {
            delete instance;
            instance = nullptr;
        }
    }

    void
    GlobalProfiler::record(const string &name, const double duration)
    {
        stats[name].record(duration);
    }

    void
    GlobalProfiler::setOutputFile(string_view file_path)
    {
        output_file = file_path;
    }

    map<string, ProfilerStats>
    GlobalProfiler::getStatsCopy() const
    {
        return stats;
    }

    string
    makeNamePadding(const string &s)
    {
        std::stringstream ss;
        for (int i = 0; (i + s.length()) < 30; ++i)
        {
            ss << ' ';
        }

        return ss.str();
    }

    string
    mili(void)
    {
        using namespace std;
        using namespace chrono;
        /* Get the current time point */
        auto now = system_clock::now();
        /* Convert to time_t for seconds and tm for local time */
        auto in_time_t = system_clock::to_time_t(now);
        tm   buf {};
        localtime_r(&in_time_t, &buf);
        /* Use stringstream to format the time */
        ostringstream ss;
        ss << "[" << put_time(&buf, "%Y-%m-%d %H:%M:%S");
        /* Calculate milliseconds (now time since epoch minus time_t converted
         * back to time since epoch) */
        auto since_epoch = now.time_since_epoch();
        auto s           = duration_cast<seconds>(since_epoch);
        since_epoch -= s;
        auto ms = duration_cast<milliseconds>(since_epoch);
        /* Append milliseconds to the formatted string, correctly placing the
         * closing square bracket */
        ss << "." << setfill('0') << setw(3) << ms.count() << "]";
        return ss.str();
    }

    void
    GlobalProfiler ::report(void)
    {
        std::ofstream file(output_file, std::ios::app);
        file << "\n\nProfiling report: " << mili() << '\n';
        for (const auto &pair : stats)
        {
            file << pair.first << makeNamePadding(pair.first) << /*  */
                ": Mean = " << pair.second.mean() << " ms, "
                 << /* makeDoublePadding(pair.second.mean())   << */
                "Stddev = " << pair.second.stddev() << " ms, "
                 << /* makeDoublePadding(pair.second.stddev()) << */
                "   Min = " << pair.second.min() << " ms, "
                 << /* makeDoublePadding(pair.second.min())    << */
                "   Max = " << pair.second.max() << " ms, "
                 << /* makeDoublePadding(pair.second.max())    << */
                " Count = " << pair.second.count()
                 << /* makeDoublePadding(pair.second.count())  << */
                "\n";
        }

        file.close();
    }

    vector<string>
    GlobalProfiler::retrveFormatedStrVecStats(void) const
    {
        std::vector<std::string> formated_stats;
        formated_stats.push_back("\n\nProfiling report: " + mili() + '\n');
        for (const auto &[name, stats] : stats)
        {
            std::stringstream ss;
            ss << name << makeNamePadding(name) << /*  */
                ": Mean = " << stats.mean() << " ms, "
               << /* makeDoublePadding(pair.second.mean())   << */
                "Stddev = " << stats.stddev() << " ms, "
               << /* makeDoublePadding(pair.second.stddev()) << */
                "   Min = " << stats.min() << " ms, "
               << /* makeDoublePadding(pair.second.min())    << */
                "   Max = " << stats.max() << " ms, "
               << /* makeDoublePadding(pair.second.max())    << */
                " Count = " << stats.count()
               << /* makeDoublePadding(pair.second.count())  << */
                "\n";
            formated_stats.push_back(ss.str());
        }
        return formated_stats;
    }

    GlobalProfiler *
    GlobalProfiler::Instance(void)
    {
        if (!instance)
        {
            instance = new GlobalProfiler;
        }
        return instance;
    }

    /** @class @c AutoTimer */

    AutoTimer::AutoTimer(std::string const &name)
        : name(name)
        , start(high_resolution_clock::now())
    {}

    AutoTimer::~AutoTimer(void)
    {
        auto                    end      = high_resolution_clock::now();
        duration<double, milli> duration = end - start;
        GLOBALPROFILER->record(name, duration.count());
    }

    void
    setupReportGeneration(string_view file_path)
    {
        GLOBALPROFILER->setOutputFile(file_path);
        atexit(
            []
            {
                GLOBALPROFILER->report();
                GLOBALPROFILER->destroy();
            });
    }

} // namespace Mlib::Profile
