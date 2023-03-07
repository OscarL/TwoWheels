#!/bin/sh

ANSWER=$(alert --info --modal "Uninstall TwoWheels? Sure?" "Yes" "No")
if [ "$ANSWER" == "Yes" ]
then
	rm /boot/home/config/settings/two_wheels.settings
	rm /boot/home/config/add-ons/input_server/filters/TwoWheels
	/system/servers/input_server -q
	sleep 3s
	alert "TwoWheels was removed."
fi
