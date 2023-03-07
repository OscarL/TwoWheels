#!/bin/sh
# Installer for the TwoWheels input_server filter add-on.

BASE_DIR=$(dirname "$0")
TEST_OUT_FILE="test_out.txt"
SETTINGS_FILE="two_wheels.settings"
SETTINGS_DIR="/boot/home/config/settings"
FILTER_ADDON_DIR="/boot/home/config/add-ons/input_server/filters"

cd "$BASE_DIR"

function TestMouse()
{
	WheelTest > "$TEST_OUT_FILE"
	_retval=$?
	[ $_retval -ne 0 ] && rm "$TEST_OUT_FILE"
}

function NoGo()
{
EXIT=$(alert --info --modal "TwoWheels is unable to detect/use your second wheel.
Bad luck :-(" "Oh well..." "Sigh..., lazy dev...")
}

function Install()
{
	rm "$SETTINGS_DIR/$SETTINGS_FILE"
	copyattr --data ./"$SETTINGS_FILE" "$SETTINGS_DIR"
	copyattr --data ./TwoWheels "$FILTER_ADDON_DIR"

	echo "# Automatically added by the installer." >> "$SETTINGS_DIR/$SETTINGS_FILE"
	echo "" >> "$SETTINGS_DIR/$SETTINGS_FILE"
	cat "$TEST_OUT_FILE" >> "$SETTINGS_DIR/$SETTINGS_FILE"
	rm "$TEST_OUT_FILE"

	sync

	/system/servers/input_server -q
	snooze 3s

	ANWSER=$(alert --info --modal  "TwoWheels has been installed.
Do you want to read/edit the auto-generated settings file?

Remember to restart the input_server if you make modifications to that file." "Yes" "No")
	if [ "$ANWSER" = "Yes" ]
	then
		StyledEdit "$SETTINGS_DIR/$SETTINGS_FILE"
	fi
}


#--- MAIN ---
TestMouse
if [ $_retval -eq 0 ]
then
	Install
	exit 0
else
	NoGo
	exit 1
fi
