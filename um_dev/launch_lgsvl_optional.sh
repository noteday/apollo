#!/bin/bash

# Launch task graph of apollo, for imcoming sensor data from LgSVL
# Yuting Xie
# 2022.3.31

source cyber/setup.bash

# Check # of args
if [ $# != 1 ]
then 
    echo "error: not node name is provided"
    exit
else
    echo "Launching $1"
fi

# Shutdown current running nodes
bash um_dev/stop_all.sh

# First write current ts as result directory suffix
S1=`date -I | awk -v OFS='' '{split($0,a,"-" ); print a[1], "_", a[2], "_", a[3]}'`
S2=`date +"%T" | awk -v OFS='' '{split($0,a,":" ); print a[1], "_", a[2], "_", a[3]}'`
echo -n profiling_result_suffix lgsvl_{$1}_${S1}_${S2} > /apollo/um_dev/profiling/conf/result_writer_conf.txt

# Then directly launch .dag files
# Transform
nohup mainboard -d modules/transform/dag/static_transform.dag &

# Localization
nohup mainboard -d modules/localization/dag/dag_streaming_rtk_localization.dag &

# Image decompress
nohup mainboard -d modules/drivers/tools/image_decompress/dag/image_decompress.dag &

# Motion service
nohup mainboard -d modules/perception/production/dag/dag_motion_service.dag &

# Perception (for lgsvl, would use velodyne128_detection_conf_lgsvl.pb.txt)
if [ $1 == "perception" ]
then
    nohup mainboard -d modules/perception/production/dag/dag_streaming_perception_lgsvl.dag &
fi

# Camera obstacle detection
if [ $1 == "camera" ]
then
    nohup mainboard -d modules/perception/production/dag/dag_streaming_obstacle_detection.dag &
fi

# Lane detection
if [ $1 == "lane" ]
then
    nohup mainboard -d modules/perception/production/dag/dag_streaming_perception_lane.dag &
fi

# Prediction
if [ $1 == "prediction" ]
then
    nohup mainboard -d modules/prediction/dag/prediction_lego.dag &
fi

# Routing
if [ $1 == "routing" ]
then
    nohup mainboard -d modules/routing/dag/routing.dag &
fi

# Planning
if [ $1 == "planning" ]
then
    nohup mainboard -d modules/planning/dag/planning.dag &
fi

# MPC Control
if [ $1 == "control" ]
then
    nohup mainboard -d modules/control/dag/mpc_module.dag &
fi

# Traffic light detection: launch TL in the end to avoid core dump!
if [ $1 == "trafficlight" ]
then
    nohup mainboard -d modules/perception/production/dag/dag_streaming_perception_trafficlights.dag &
fi