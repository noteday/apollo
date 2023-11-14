#!/bin/bash

# Launch task graph of apollo, for imcoming sensor data from LgSVL
# Yuting Xie
# 2022.3.31

# stop before
ps -ef | grep .dag | grep -v grep | awk '{print $2}' | xargs kill -9

source /apollo/cyber/setup.bash


# Transform
nohup mainboard -d modules/transform/dag/static_transform.dag &

# Localization
nohup mainboard -d modules/localization/dag/dag_streaming_rtk_localization.dag &




# FusionCameraComponent +radar+detection
nohup mainboard -d modules/perception/production/dag/dag_streaming_perception.dag &

# CameraObstacleDetectionComponent
nohup mainboard -d modules/perception/production/dag/dag_streaming_obstacle_detection.dag &

#FusionCameraComponent
nohup mainboard -d modules/perception/production/dag/dag_streaming_perception_camera.dag &


# Lane detection
nohup mainboard -d modules/perception/production/dag/dag_streaming_perception_lane.dag &


# Motion service
nohup mainboard -d modules/perception/production/dag/dag_motion_service.dag &

# Prediction
nohup mainboard -d modules/prediction/dag/prediction.dag &

# Routing
nohup mainboard -d modules/routing/dag/routing.dag &

# Planning
nohup mainboard -d modules/planning/dag/planning.dag &

# Control
nohup mainboard -d modules/control/dag/mpc_module.dag &

# Image decompress
nohup mainboard -d modules/drivers/tools/image_decompress/dag/image_decompress.dag &




###test2
#FusionCameraComponent
nohup mainboard -d modules/perception/production/dag/dag_streaming_perception_camera_2.dag &
# Prediction
nohup mainboard -d modules/prediction/dag/prediction_2.dag &
# Planning
nohup mainboard -d modules/planning/dag/planning_2.dag &


###test3
# CameraObstacleDetectionComponent
nohup mainboard -d modules/perception/production/dag/dag_streaming_obstacle_detection.dag &
# Prediction
nohup mainboard -d modules/prediction/dag/prediction_3.dag &
# Planning
nohup mainboard -d modules/planning/dag/planning_3.dag &



### test4 only radar+point cloud
#FusionCameraComponent
nohup mainboard -d modules/perception/production/dag/dag_streaming_perception_4.dag &
# Prediction
nohup mainboard -d modules/prediction/dag/prediction_4.dag &
# Planning
nohup mainboard -d modules/planning/dag/planning_4.dag &