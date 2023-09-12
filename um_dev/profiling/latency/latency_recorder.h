// A latency recorder
// Author: Yuting Xie
// 2022.5.26

#ifndef LATENCY_RECORDER_H
#define LATENCY_RECORDER_H

#include <string>

#include "cyber/time/time.h"

#define NANO_TO_MICRO (0.001)
#define MICRO_TO_MILLI (0.001)

namespace um_dev {
namespace profiling {

enum LatencyType {
    LATENCY_TYPE_LIDAR = 0,
    LATENCY_TYPE_RADAR,
    LATENCY_TYPE_CAMERA,
};

class LatencyRecorder {
 public:
  LatencyRecorder() = delete;
  LatencyRecorder(LatencyRecorder&) = delete;
  LatencyRecorder(std::string taskname);
  ~LatencyRecorder() = default;

  // Record one of the input sensor data information from generation until now, this also tracks it!
  void record_latency(LatencyType type, apollo::cyber::Time sensor_input_time);
  // No more track it (e.g., reaching the end of control), this would output result via result_writer
  // void stop_record_latency(LatencyType type, apollo::cyber::Time sensor_input_time);

 private:
  std::string taskname_;
};

}  // namespace profiling
}  // namespace um_dev

#endif