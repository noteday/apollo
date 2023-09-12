// Profiling result writer
// Author: Yuting Xie
// 2022.2.22

#ifndef RESULT_WRITER_H
#define RESULT_WRITER_H

#include <fstream>
#include <unordered_map>
#include <mutex>
#include <string>

#include "cyber/time/time.h"


#define NANO_TO_MICRO 1 / (1000)
#define MICRO_TO_MILLI 1 / (1000)
#define MILLI_TO_MICRO (1000)
#define MICRO_TO_NANO (1000)

namespace um_dev {
namespace profiling {

enum PROFILING_METRICS {
  TIMING = 0,
  MEMORY,
  GPU,
  LATENCY_LIDAR,
  LATENCY_RADAR,
  LATENCY_CAMERA,
};

class ProfilingResultWriter {
 public:
  ProfilingResultWriter(ProfilingResultWriter&) = delete;
  ~ProfilingResultWriter();
  static ProfilingResultWriter& Instance();
  bool write_to_file(PROFILING_METRICS profiling_type,
                                          apollo::cyber::Time ts_start,
                                          apollo::cyber::Time ts_end,
                                          const std::string& component,
                                          apollo::cyber::Duration& execution_time,
                                          const long long ts_cam,
                                          const long long ts_lidar,
                                          const long long ts_radar,
                                          const long long ts_TL,
                                          const long long ts_lane,
                                          const bool is_finish,
                                          const int info1,
                                          const int info2,
                                          const int info3,
                                          bool is_throttled = false);

 private:
  ProfilingResultWriter();
  bool Init();

 private:
  static ProfilingResultWriter* instance_;
  static std::mutex mutex_instance_;

  std::unordered_map<std::string, std::string> confs_;

  const float throttle_threshold_;  // throttle threshold in second
  std::string profiling_suffix_;

  std::unordered_map<std::string, apollo::cyber::Time> task_to_timestamp_;

  std::mutex mutex_result_file_;
  std::mutex mutex_map_;

  // Result folders
  std::unordered_map<std::string, std::string> type_to_folders_;

  // Result files
  std::unordered_map<PROFILING_METRICS, std::string> metrics_to_names_;
  std::unordered_map<PROFILING_METRICS, std::ofstream> metrics_to_files_;
  std::unordered_map<PROFILING_METRICS, std::mutex> metrics_to_mutex_;

};

}  // namespace profiling
}  // namespace um_dev
#endif