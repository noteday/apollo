source cyber/setup.bash

# Perception
ps -ef | grep dag_streaming_perception.dag | grep -v grep | awk '{print $2}' | xargs kill -9
# Traffic light detection
ps -ef | grep dag_streaming_perception_trafficlights.dag | grep -v grep | awk '{print $2}' | xargs kill -9
# Camera obstacle detection
ps -ef | grep dag_streaming_obstacle_detection.dag | grep -v grep | awk '{print $2}' | xargs kill -9
# Lane detection
ps -ef | grep dag_streaming_perception_lane.dag | grep -v grep | awk '{print $2}' | xargs kill -9
# Motion service
ps -ef | grep dag_motion_service.dag | grep -v grep | awk '{print $2}' | xargs kill -9 