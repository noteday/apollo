#ifndef TIMINGCHANNEL_H
#define TIMINGCHANNEL_H

#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <fstream>
#include "cyber/cyber.h"
#include "um_dev/profiling/timing_channel/timing_message.pb.h"

namespace apollo {
namespace um_dev {

using apollo::timingMessage::TimingMessage;

class TimingChannel{

    public:
     TimingChannel(TimingChannel& other) = delete;
     /**
      * Singletons should not be assignable.
      */
     void operator=(const TimingChannel&) = delete;

     static TimingChannel* GetInstance();
     void WriteData(apollo::cyber::Time ts_start, apollo::cyber::Time ts_end,
                    const std::string& component,
                    apollo::cyber::Duration& execution_time,
                    const long long ts_cam, const long long ts_lidar,
                    const long long ts_radar, const long long ts_TL,
                    const long long ts_lane, const bool is_finish,
                    const int info1, const int info2, const int info3);

     void Test();

    private:
     static TimingChannel* timingChannel;
     static std::mutex mutex_;
     static  std::unique_ptr<apollo::cyber::Node> node ;
     static  std::shared_ptr<cyber::Writer<TimingMessage>> time_message_writer_ ;
     bool is_cyber_initialized_;
    
    protected:
     TimingChannel(){};
};
}  
}  
#endif