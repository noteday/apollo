source cyber/setup.bash

S1=`date -I | awk -v OFS='' '{split($0,a,"-" ); print a[1], "_", a[2], "_", a[3]}'`
S2=`date +"%T" | awk -v OFS='' '{split($0,a,":" ); print a[1], "_", a[2], "_", a[3]}'`

# mkdir records/storage/lgsvl_${S1}_${S2}

# cyber_recorder record -o records/storage/lgsvl_${S1}_${S2}/bag.record -c /apollo/canbus/chassis /apollo/localization/msf_status /apollo/localization/pose /apollo/perception/traffic_light /apollo/routing_request /apollo/routing_response /apollo/routing_response_history /apollo/sensor/camera/front_12mm/image /apollo/sensor/camera/front_12mm/image/compressed /apollo/sensor/camera/front_6mm/image /apollo/sensor/camera/front_6mm/image/compressed /apollo/prediction /apollo/perception/obstacles /apollo/planning /apollo/sensor/gnss/best_pose /apollo/sensor/gnss/corrected_imu /apollo/sensor/gnss/imu /apollo/sensor/gnss/ins_stat /apollo/sensor/gnss/odometry /apollo/sensor/lidar128/compensator/PointCloud2 /apollo/sensor/radar/front 
# cyber_recorder record -o records/storage/lgsvl_${S1}_${S2}/bag.record -a

mkdir records/lgsvl_${S1}_${S2}
cyber_recorder record -o records/lgsvl_${S1}_${S2}/bag.record -a