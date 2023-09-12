// Implementation of ProfilingResultWriter
// Author: Yuting Xie
// 2022.2.22

#include "um_dev/profiling/result_writer.h"

#include <assert.h>
#include <unistd.h>

#include <iostream>

#include "um_dev/utils/files.h"

namespace um_dev {
namespace profiling {

ProfilingResultWriter* ProfilingResultWriter::instance_ =
    new ProfilingResultWriter();

ProfilingResultWriter::ProfilingResultWriter()
    : throttle_threshold_(1.f), profiling_suffix_("default") {

  if (Init()) {
    if (confs_.count("profiling_result_suffix") > 0) {
      profiling_suffix_ = confs_["profiling_result_suffix"];
    }
  } else {
    std::cout << "[um_dev] unable to open conf file for result writer.\n";
  }
  // Make profiling output directories for this time
  apollo::cyber::Time now = apollo::cyber::Time::Now();
  const std::string result_root_dir =
      "/apollo/um_dev/profiling/results/" + profiling_suffix_ + "/";
  um_dev::utils::um_mkdir(result_root_dir);
  for (auto it : metrics_to_names_) {
    um_dev::utils::um_mkdir(result_root_dir + it.second);
  }
  

  auto pid_str = std::to_string(getpid());

  // Open result files here
  auto time_str = now.ToString();
  std::vector<std::string> fields = {
    "ts_start", // The timing starting ts(ns)
    "ts_end", // The timing ending ts(ns), also the ts it outputs result if is_finish
    "type", // What type of latency it records
    "component", // Which component it is
    "execution_time", // The execution time of this component (ns)
    "ts_cam", // ts for camera information carried by output result (ns) (0 means this message carries NO such info)
    "ts_lidar", // ts for lidar information carried by output result (ns)
    "ts_radar", // ts for radar information carried by output result (ns)
    "ts_TL", // ts for TL information carried by output result (ns)
    "ts_lane", // ts for lane information carried by output result (ns)
    "lat_cam", // E2E latency for camera information carried by output result (ns) (0 means this message carries NO such info)
    "lat_lidar", // E2E latency for lidar information carried by output result (ns)
    "lat_radar", // E2E latency for radar information carried by output result (ns)
    "lat_TL", // E2E latency for lidar information carried by output result (ns)
    "lat_lane", // E2E latency for radar information carried by output result (ns)
    "is_finish", // Whether the component outputs a valid result this time
    "info1", // Component-specified information 1 to be carried.
    "info2", // Component-specified information 2 to be carried.
    "info3", // Component-specified information 3 to be carried.
    };
  for (auto it : metrics_to_names_) {
    metrics_to_files_[it.first].open(result_root_dir + it.second + "/" + time_str + '_' + pid_str + ".csv");
    assert(metrics_to_files_[it.first].is_open());
    metrics_to_files_[it.first] << utils::make_csv_header(fields) << std::endl;
  }
}

ProfilingResultWriter::~ProfilingResultWriter() {
  // Can only be referenced
  for (auto& it : metrics_to_files_) {
    if (it.second.is_open()) {
      it.second.close();
    }
  }
}

bool ProfilingResultWriter::Init() {
  metrics_to_names_[PROFILING_METRICS::TIMING] = "Timing";
  // metrics_to_names_[PROFILING_METRICS::LATENCY_CAMERA] = "Lat_Cam";
  // metrics_to_names_[PROFILING_METRICS::LATENCY_RADAR] = "Lat_Rad";
  // metrics_to_names_[PROFILING_METRICS::LATENCY_LIDAR] = "Lat_Lid";

  std::ifstream conf_fin("/apollo/um_dev/profiling/conf/result_writer_conf.txt");
  if (!conf_fin.is_open()) {
    return false;
  }
  std::string conf_name, conf_value;
  while (!conf_fin.eof()) {
    conf_fin >> conf_name >> conf_value;
    confs_[conf_name] = conf_value;
  }
  return true;
}

ProfilingResultWriter& ProfilingResultWriter::Instance() { return *instance_; }

bool ProfilingResultWriter::write_to_file(PROFILING_METRICS profiling_type,
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
                                          bool is_throttled) {
  if (is_throttled) {
    std::lock_guard<std::mutex> lock(mutex_map_);
    apollo::cyber::Time now = apollo::cyber::Time::Now();
    auto throttle_timestamp = component + std::to_string(profiling_type);
    auto it = task_to_timestamp_.find(throttle_timestamp);
    if (it == task_to_timestamp_.end()) {
      task_to_timestamp_[throttle_timestamp] = now;
    } else if (now - it->second <
               apollo::cyber::Duration(throttle_threshold_)) { 
      return true;
    } else {
      it->second = now;
    }
  }

  // (Deprecated) Yuting@2022.6.15 Now reference metrics by their name strings
  // Yuting@2022.6.22 Now puts all results in one file (lines)
  {
    std::lock_guard<std::mutex> locker(metrics_to_mutex_[profiling_type]);
    auto& fout_ = metrics_to_files_[profiling_type];
    // The CSV header: "timestamp", "ts_start", "ts_end", "type", "component", "result"
    fout_ << ts_start.ToNanosecond() << ',' // "ts_start"
          << ts_end.ToNanosecond() << ',' // "ts_end"
          << metrics_to_names_[profiling_type] << ',' // "type"
          << component << ',' // "component"
          << execution_time.ToNanosecond() << ',' // "execution_time"
          << ts_cam << ',' // "ts_cam"
          << ts_lidar << ',' // "ts_lidar"
          << ts_radar << ',' // "ts_radar"
          << ts_TL << ',' // "ts_TL"
          << ts_lane << ',' // "ts_lane"
          << ts_end.ToNanosecond() - ts_cam << ',' // "lat_cam"
          << ts_end.ToNanosecond() - ts_lidar << ',' // "lat_lidar"
          << ts_end.ToNanosecond() - ts_radar << ',' // "lat_radar"
          << ts_end.ToNanosecond() - ts_TL << ',' // "lat_lidar"
          << ts_end.ToNanosecond() - ts_lane << ',' // "lat_radar"
          << int(is_finish) << ',' // "is_finish"
          << info1 << ',' // "info1"
          << info2 << ',' // "info2"
          << info3 << std::endl; // "info3"
  }

  return true;
}

}  // namespace profiling
}  // namespace um_dev