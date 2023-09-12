// Implementation of UM_Timing
// Author: Yuting Xie
// 2022.2.22

#include "um_dev/profiling/timing/timing.h"

#include "um_dev/profiling/result_writer.h"

#include "cyber/time/duration.h"

namespace um_dev {
namespace profiling {

UM_Timing::UM_Timing(std::string taskname) : taskname_(taskname), is_finish_(false) {
  ts_start_ = apollo::cyber::Time::Now();
}

UM_Timing::~UM_Timing() {
  // Finished means we already record the result.
  if (is_finish_) {
    return; 
  }
  auto ts_end =  apollo::cyber::Time::Now();
  auto duration = ts_end - ts_start_;
  ProfilingResultWriter::Instance().write_to_file(PROFILING_METRICS::TIMING,
                                                  ts_start_,
                                                  ts_end,
                                                  taskname_,
                                                  duration,
                                                  0, // All zero since the component output nothing this time
                                                  0,
                                                  0,
                                                  0,
                                                  0,
                                                  false,
                                                  0,
                                                  0,
                                                  0);
}

void UM_Timing::set_finish(const long long ts_cam, 
                          const long long ts_lidar, 
                          const long long ts_radar, 
                          const long long ts_TL,
                          const long long ts_lane) {
  if (is_finish_) {
    return;
  }
  is_finish_ = true;
  auto ts_end =  apollo::cyber::Time::Now();
  auto duration = ts_end - ts_start_;
  // auto lat_cam = ts_cam > 0 ? ts_end.ToNanosecond() - ts_cam : 0;
  // auto lat_lidar = ts_lidar > 0 ? ts_end.ToNanosecond() - ts_lidar : 0;
  // auto lat_radar = ts_radar > 0 ? ts_end.ToNanosecond() - ts_radar : 0;
  ProfilingResultWriter::Instance().write_to_file(PROFILING_METRICS::TIMING,
                                                  ts_start_,
                                                  ts_end,
                                                  taskname_,
                                                  duration,
                                                  ts_cam,
                                                  ts_lidar,
                                                  ts_radar,
                                                  ts_TL,
                                                  ts_lane,
                                                  true,
                                                  info1,
                                                  info2,
                                                  info3);
}

void UM_Timing::set_info(const int info1, const int info2, const int info3) {
  this->info1 = info1;
  this->info2 = info2;
  this->info3 = info3;
}

void UM_Timing::add_checkpoint(std::string name, const long long ts_cam, 
  const long long ts_lidar, const long long ts_radar) {
    auto ns_now =  apollo::cyber::Time::Now().ToNanosecond();
    std::cout << taskname_ + "-" + name << ": " << "ts_ns=" << ns_now
              << ", ts_cam=" << ts_cam
              << ", ts_lidar=" << ts_lidar
              << ", ts_radar=" << ts_radar
              << ", lat_cam=" << ns_now - ts_cam
              << ", lat_lidar=" << ns_now - ts_lidar
              << ", lat_radar=" << ns_now - ts_radar
              << std::endl;
}

}  // namespace profiling
}  // namespace um_dev
