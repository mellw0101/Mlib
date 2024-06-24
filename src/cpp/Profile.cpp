/// @file Profile.cpp
/// @brief
/// - Contains the implementation of the
/// - profiling classes and functions.
#include "../include/Profile.h"

namespace Mlib::Profile
{
    /// @class @c ProfilerStats
    /// Functions:
    /// - record(double value)
    /// - mean() const
    /// - stddev() const
    /// - min() const
    /// - max() const
    /// - count() const

    void
    ProfilerStats::record(f64 const value)
    {
        values.push_back(value);
    }

    double
    ProfilerStats::mean() const
    {
        if (values.empty())
        {
            return 0.0;
        }
        double sum = std::accumulate(values.begin(), values.end(), 0.0);
        return sum / values.size();
    }

    double
    ProfilerStats::stddev() const
    {
        if (values.size() < 2)
        {
            return 0.0;
        }
        f64 mean_val = mean();
        f64 sq_sum   = std::accumulate(values.begin(), values.end(), 0.0,
                                       [mean_val](f64 a, f64 b)
                                       {
                                         return a + (b - mean_val) * (b - mean_val);
                                     });
        return std::sqrt(sq_sum / values.size());
    }

    f64
    ProfilerStats::min() const
    {
        return values.empty() ? 0.0 : *std::min_element(values.begin(), values.end());
    }

    f64
    ProfilerStats::max() const
    {
        return values.empty() ? 0.0 : *std::max_element(values.begin(), values.end());
    }

    size_t
    ProfilerStats::count() const
    {
        return values.size();
    }

    /// @class @c GlobalProfiler

    GlobalProfiler *GlobalProfiler::instance = nullptr;

    GlobalProfiler ::GlobalProfiler()
    {}

    void
    GlobalProfiler ::destroy()
    {
        if (instance != nullptr)
        {
            delete instance;
            instance = nullptr;
        }
    }

    void
    GlobalProfiler::record(const std::string &name, const f64 duration)
    {
        stats[name].record(duration);
    }

    void
    GlobalProfiler::setOutputFile(std::string_view file_path)
    {
        output_file = file_path;
    }

    std::string
    makeNamePadding(const std::string &s)
    {
        std::stringstream ss;
        for (int i = 0; (i + s.length()) < 30; ++i)
        {
            ss << ' ';
        }

        return ss.str();
    }

    std::string
    mili()
    {
        // Get the current time point
        auto now = std::chrono::system_clock::now();

        // Convert to time_t for seconds and tm for local time
        auto    in_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm buf {};
        localtime_r(&in_time_t, &buf);

        // Use stringstream to format the time
        std::ostringstream ss;
        ss << "[" << std::put_time(&buf, "%Y-%m-%d %H:%M:%S");

        // Calculate milliseconds (now time since epoch minus time_t converted back to time since epoch)
        auto since_epoch = now.time_since_epoch();

        auto s = std::chrono::duration_cast<std::chrono::seconds>(since_epoch);

        since_epoch -= s;

        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch);

        // Append milliseconds to the formatted string, correctly placing the closing square bracket
        ss << "." << std::setfill('0') << std::setw(3) << ms.count() << "]";

        return ss.str();
    }

    void
    GlobalProfiler ::report()
    {
        std::ofstream file(output_file, std::ios::app);
        file << "\n\nProfiling report: " << mili() << '\n';
        for (const auto &pair : stats)
        {
            file << pair.first << makeNamePadding(pair.first) << ": Mean = " << pair.second.mean() << " ms, "
                 <<                                               /* makeDoublePadding(pair.second.mean())   << */
                "Stddev = " << pair.second.stddev() << " ms, " << /* makeDoublePadding(pair.second.stddev()) << */
                "   Min = " << pair.second.min() << " ms, " <<    /* makeDoublePadding(pair.second.min())    << */
                "   Max = " << pair.second.max() << " ms, " <<    /* makeDoublePadding(pair.second.max())    << */
                " Count = " << pair.second.count() <<             /* makeDoublePadding(pair.second.count())  << */
                "\n";
        }

        file.close();

        // for (const auto &i : stats)
        // {
        //     std::ofstream File("/home/mellw/gprof/" + i.first.data(), std::ios::app);
        //     File << i.second.mean() << ':' << i.second.stddev() << ':' << i.second.min() << ':' << i.second.max() <<
        //     ':'
        //          << i.second.count() << ':' << "\n";
        // }
    }

    GlobalProfiler *
    GlobalProfiler ::Instance()
    {
        if (!instance)
        {
            instance = new GlobalProfiler;
        }
        return instance;
    }

    /// @class @c AutoTimer

    AutoTimer ::AutoTimer(std::string const &name)
        : name(name)
        , start(std::chrono::high_resolution_clock::now())
    {}

    AutoTimer ::~AutoTimer()
    {
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> duration = end - start;
        GlobalProfiler::Instance()->record(name, duration.count());
    }

    void
    setupReportGeneration(std::string_view file_path)
    {
        GlobalProfiler::Instance()->setOutputFile(file_path);
        atexit(
            []
            {
                GlobalProfiler::Instance()->report();
            });
    }
} // namespace Mlib::Profile
