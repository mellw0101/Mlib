/** @file Profile.cpp.  Contains the implementation of the profiling classes and functions. */
#include "../include/Profile.h"
#include "../include/def.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <new>
#include <numeric>
#include <ratio>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

using namespace std;
using namespace chrono;

namespace Mlib::Profile {
  /** @class @c ProfilerStats */

  void ProfilerStats::record(const double value) {
    values.push_back(value);
  }

  double ProfilerStats::mean(void) const {
    if (values.empty()) {
      return 0.0;
    }
    double sum = std::accumulate(values.begin(), values.end(), 0.0);
    return (sum / values.size());
  }

  double ProfilerStats::stddev(void) const {
    if (values.size() < 2) {
      return 0.0;
    }
    double mean_val = mean();
    double sq_sum   = std::accumulate(values.begin(), values.end(), 0.0, [mean_val](double a, double b) {
      return a + (b - mean_val) * (b - mean_val);
    });
    return std::sqrt(sq_sum / values.size());
  }

  double ProfilerStats::min(void) const {
    return values.empty() ? 0.0 : *std::min_element(values.begin(), values.end());
  }

  double ProfilerStats::max(void) const {
    return values.empty() ? 0.0 : *std::max_element(values.begin(), values.end());
  }

  Ulong ProfilerStats::count(void) const {
    return values.size();
  }

  /** @class @c GlobalProfiler */

  GlobalProfiler *GlobalProfiler::instance = nullptr;

  void GlobalProfiler::_destroy(void) noexcept {
    delete instance;
    instance = nullptr;
  }

  void GlobalProfiler::record(const string &name, const double duration) {
    stats[name].record(duration);
  }

  void GlobalProfiler::setOutputFile(string_view file_path) {
    output_file = file_path;
  }
 
  map<string, ProfilerStats> GlobalProfiler::getStatsCopy() const {
    return stats;
  }

  string makeNamePadding(const string &s) {
    std::stringstream ss;
    for (int i = 0; (i + s.length()) < 30; ++i) {
      ss << ' ';
    }
    return ss.str();
  }

  string mili(void) {
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

  void GlobalProfiler ::report(void) {
    if (output_file.empty()) {
      cout << "\n\nProfiling report: " << mili() << '\n';
      for (const auto &[first, second] : stats) {
        cout << first << makeNamePadding(first) <<     /*  */
          ": Mean = " << second.mean() << " ms, " <<   /* makeDoublePadding(pair.second.mean())   << */
          "Stddev = " << second.stddev() << " ms, " << /* makeDoublePadding(pair.second.stddev()) << */
          "   Min = " << second.min() << " ms, " <<    /* makeDoublePadding(pair.second.min())    << */
          "   Max = " << second.max() << " ms, " <<    /* makeDoublePadding(pair.second.max())    << */
          " Count = " << second.count() <<             /* makeDoublePadding(pair.second.count())  << */
          "\n";
      }
    }
    else {
      std::ofstream file(output_file, std::ios::app);
      file << "\n\nProfiling report: " << mili() << '\n';
      for (const auto &pair : stats) {
        file << pair.first << makeNamePadding(pair.first) << /*  */
          ": Mean = " << pair.second.mean() << " ms, " <<    /* makeDoublePadding(pair.second.mean())   << */
          "Stddev = " << pair.second.stddev() << " ms, " <<  /* makeDoublePadding(pair.second.stddev()) << */
          "   Min = " << pair.second.min() << " ms, " <<     /* makeDoublePadding(pair.second.min())    << */
          "   Max = " << pair.second.max() << " ms, " <<     /* makeDoublePadding(pair.second.max())    << */
          " Count = " << pair.second.count() <<              /* makeDoublePadding(pair.second.count())  << */
          "\n";
      }
      file.close();
    }
  }

  vector<string> GlobalProfiler::retrveFormatedStrVecStats(void) const {
    std::vector<std::string> formated_stats;
    formated_stats.push_back("\n\nProfiling report: " + mili() + '\n');
    char buffer[4096];
    for (const auto &[name, stats] : stats) {
      snprintf(
        buffer, sizeof(buffer),
        "%s%s: Mean = %.6f ms, Stddev = %.6f ms, Min = %.6f ms, Max = %.6f, Count = %lu\n",
        name.c_str(), makeNamePadding(name).c_str(), stats.mean(), stats.stddev(), stats.min(), stats.max(), stats.count()
      );
      formated_stats.push_back(buffer);
    }
    return formated_stats;
  }

  GlobalProfiler::GlobalProfiler(void) noexcept {}

  GlobalProfiler *GlobalProfiler::Instance(void) noexcept {
    if (!instance) {
      instance = new (nothrow) GlobalProfiler();
      if (!instance) {
        exit(1);
      }
      atexit(GlobalProfiler::_destroy);
    }
    return instance;
  }

  /** @class @c AutoTimer */

  AutoTimer::AutoTimer(const string &name) : name(name), start(high_resolution_clock::now()) {}

  AutoTimer::~AutoTimer(void) {
    auto                    end      = high_resolution_clock::now();
    duration<double, milli> duration = end - start;
    GLOBALPROFILER->record(name, duration.count());
  }

  void setupReportGeneration(string_view file_path) {
    GLOBALPROFILER->setOutputFile(file_path);
    atexit([] {
      GLOBALPROFILER->report();
    });
  }
}
