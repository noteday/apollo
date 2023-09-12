// Implementation of LatencyRecorder
// Author: Yuting Xie
// 2022.2.22

#include "um_dev/profiling/latency/latency_recorder.h"

#include "um_dev/profiling/result_writer.h"

#include "cyber/time/duration.h"

namespace um_dev {
namespace profiling {

LatencyRecorder::LatencyRecorder(std::string taskname) : taskname_(taskname) {}

void LatencyRecorder::record_latency(LatencyType type, apollo::cyber::Time sensor_input_time) {
    if (sensor_input_time >= apollo::cyber::Time::Now()) {
        // return;
    }
    auto latency = apollo::cyber::Time::Now() - sensor_input_time;
    PROFILING_METRICS profiling_metric;
    switch (type) {
    case LATENCY_TYPE_LIDAR:
        profiling_metric = PROFILING_METRICS::LATENCY_LIDAR;
        break;
    case LATENCY_TYPE_RADAR:
        profiling_metric = PROFILING_METRICS::LATENCY_RADAR;
        break;
    case LATENCY_TYPE_CAMERA:
        profiling_metric = PROFILING_METRICS::LATENCY_CAMERA;
        break;
    default:
        break;
    }
    auto ns = latency.ToNanosecond();
    auto micro_s = ns / MICRO_TO_NANO;
    ProfilingResultWriter::Instance().write_to_file(profiling_metric,
                                                    apollo::cyber::Time(0),
                                                    apollo::cyber::Time(0),
                                                    taskname_,
                                                    std::to_string(micro_s),
                                                    true);
}

// void LatencyRecorder::stop_record_latency(LatencyType type, apollo::cyber::Time sensor_input_time) {}

}  // namespace profiling
}  // namespace um_dev
