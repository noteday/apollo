#!/bin/bash

# Launch task graph of apollo, for imcoming sensor data from LgSVL
# Yuting Xie
# 2022.3.31

source cyber/setup.bash

# Shutdown current running nodes
bash um_dev/stop_all.sh

# First write current ts as result directory suffix
S1=`date -I | awk -v OFS='' '{split($0,a,"-" ); print a[1], "_", a[2], "_", a[3]}'`
S2=`date +"%T" | awk -v OFS='' '{split($0,a,":" ); print a[1], "_", a[2], "_", a[3]}'`
echo -n profiling_result_suffix lgsvl_realtime_${S1}_${S2} > /apollo/um_dev/profiling/conf/result_writer_conf.txt

# Then directly launch .dag files
# Transform
nohup mainboard -d modules/transform/dag/static_transform.dag &

# Localization
nohup mainboard -d modules/localization/dag/dag_streaming_rtk_localization.dag &

# Image decompress
nohup mainboard -d modules/drivers/tools/image_decompress/dag/image_decompress.dag &
# Perception (for lgsvl, would use velodyne128_detection_conf_lgsvl.pb.txt)
nohup mainboard -d modules/perception/production/dag/dag_streaming_perception_lgsvl.dag &
# Camera obstacle detection
# nohup mainboard -d modules/perception/production/dag/dag_streaming_obstacle_detection.dag &
# Lane detection
nohup mainboard -d modules/perception/production/dag/dag_streaming_perception_lane.dag &
# Motion service
nohup mainboard -d modules/perception/production/dag/dag_motion_service.dag &

# Prediction
nohup mainboard -d modules/prediction/dag/prediction_lego.dag &

# Routing
nohup mainboard -d modules/routing/dag/routing.dag &

# Planning
nohup mainboard -d modules/planning/dag/planning.dag &

# MPC Control
nohup mainboard -d modules/control/dag/mpc_module.dag &

# Traffic light detection: launch TL in the end to avoid core dump!
nohup mainboard -d modules/perception/production/dag/dag_streaming_perception_trafficlights.dag &