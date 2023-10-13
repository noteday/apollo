
#include "cyber/cyber.h"
#include "um_dev/profiling/timing_channel/timing_channel.h"

namespace apollo{
namespace um_dev{

using apollo::cyber::Reader;
using apollo::cyber::Writer;
using apollo::um_dev::TimingChannel;
using apollo::timingMessage::TimingMessage;


TimingChannel* TimingChannel::timingChannel{nullptr};
std::mutex TimingChannel::mutex_;
std::unique_ptr<apollo::cyber::Node> TimingChannel::node{nullptr} ;
std::shared_ptr<cyber::Writer<TimingMessage>>
    TimingChannel::time_message_writer_{nullptr};

void TimingChannel::WriteData(
    apollo::cyber::Time ts_start, apollo::cyber::Time ts_end,
    const std::string& component, apollo::cyber::Duration& execution_time,
    const long long ts_cam, const long long ts_lidar, const long long ts_radar,
    const long long ts_TL, const long long ts_lane, const bool is_finish,
    const int info1, const int info2, const int info3) {
  apollo::timingMessage::TimingMessage msg;
  time_message_writer_->Write(msg);
}

void TimingChannel::Test(){
    apollo::timingMessage::TimingMessage msg;

  this->time_message_writer_->Write(msg);

}


TimingChannel* TimingChannel::GetInstance(){
    std::lock_guard<std::mutex> lock(mutex_);
    if (timingChannel == nullptr) {
      timingChannel = new TimingChannel();
      
    }
    if (node == nullptr){
        node = apollo::cyber::CreateNode("TimingChannelzwzw");
    }
    if(time_message_writer_== nullptr){
      time_message_writer_ = node->CreateWriter<TimingMessage>("TimingMessage");
    }


    return timingChannel;


  return TimingChannel::timingChannel;
}
}  // namespace um_dev
}  // namespace apollo

