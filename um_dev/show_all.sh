ps -ef | grep .dag | grep -v grep | awk '{print $2 " " $10}'