source cyber/setup.bash

ps -ef | grep .dag | grep -v grep | awk '{print $2}' | xargs kill -9