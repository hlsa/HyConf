cd ../../../
export DISPLAY=:1
gazebo world.sdf -u --verbose --lockstep &
WIN=$(wmctrl -l  | grep Gazebo)
while [ -z "$WIN" ]
do
	echo "not yet opened"
	sleep 0.1
	WIN=$(wmctrl -l | grep Gazebo)
done
echo "window is open"

echo "moving window"
wmctrl -r "Gazebo" -e 0,0,0,2560,1440 -F
