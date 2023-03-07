#include <InputServerFilter.h>
#include <InterfaceDefs.h>
#include <Message.h>
#include <TypeConstants.h>

#include <stdlib.h>
#ifdef DEBUG
	#include <syslog.h>
#endif
#include "TwoWheels.h"
#include "driver_settings.h"

//------------------------------------------------------------------------------

BInputServerFilter* instantiate_input_filter()
{
	return new TwoWheels();
}

//------------------------------------------------------------------------------

TwoWheels::TwoWheels()
{
	LoadSettings();
#ifdef DEBUG
	syslog(LOG_DEBUG, "wheel_1.x_up: %d",   settings.wheel_1.x_up);
	syslog(LOG_DEBUG, "wheel_1.x_down: %d", settings.wheel_1.x_down);
	syslog(LOG_DEBUG, "wheel_1.y_up: %d",   settings.wheel_1.y_up);
	syslog(LOG_DEBUG, "wheel_1.y_down: %d", settings.wheel_1.y_down);


	syslog(LOG_DEBUG, "wheel_2.x_up: %d",   settings.wheel_2.x_up);
	syslog(LOG_DEBUG, "wheel_2.x_down: %d", settings.wheel_2.x_down);
	syslog(LOG_DEBUG, "wheel_2.y_up: %d",   settings.wheel_2.y_up);
	syslog(LOG_DEBUG, "wheel_2.y_down: %d", settings.wheel_2.y_down);
#endif
}

void
TwoWheels::LoadSettings()
{
	void *handle = load_driver_settings_from_path(SETTINGS_FILE);
	if (handle != NULL)
	{
		const char* item;
		char*		end;
		int32		value;

		settings.toggle_wheels     = get_driver_boolean_parameter(handle, "toggle_wheels", false, true);
		settings.invert_horizontal = get_driver_boolean_parameter(handle, "invert_horizontal", false, true);
		settings.invert_vertical   = get_driver_boolean_parameter(handle, "invert_vertical", false, true);

	// SpeedUp Factor
		item = get_driver_parameter(handle, "speedup_factor", "2", "0");
		value = strtoul(item, &end, 0);
		if (*end == '\0') settings.speedup = value;

		// Keys
		item = get_driver_parameter(handle, "accel_key", "", "");
		settings.accel_key = StringToKey(item);

		item = get_driver_parameter(handle, "wheel_toggle_key", "B_SCROLL_LOCK", "");
		settings.wheel_toggle_key = StringToKey(item);

		item = get_driver_parameter(handle, "bypass_key", "", "");
		settings.bypass_key = StringToKey(item);

	// Wheel mappings - Default values came from what Be's kb_mouse driver
	// reports for my mouse's wheels.

	// First Wheel
		item = get_driver_parameter(handle, "x1_up", "0", "0");
		value = strtol(item, &end, 0);
		if (*end == '\0') settings.wheel_1.x_up = value;

		item = get_driver_parameter(handle, "x1_down", "0", "0");
		value = strtol(item, &end, 0);
		if (*end == '\0') settings.wheel_1.x_down = value;

		item = get_driver_parameter(handle, "y1_up", "-1", "0");
		value = strtol(item, &end, 0);
		if (*end == '\0') settings.wheel_1.y_up = value;

		item = get_driver_parameter(handle, "y1_down", "1", "0");
		value = strtol(item, &end, 0);
		if (*end == '\0') settings.wheel_1.y_down = value;

	// Second Wheel
		item = get_driver_parameter(handle, "x2_up", "0", "0");
		value = strtol(item, &end, 0);
		if (*end == '\0') settings.wheel_2.x_up = value;

		item = get_driver_parameter(handle, "x2_down", "0", "0");
		value = strtol(item, &end, 0);
		if (*end == '\0') settings.wheel_2.x_down = value;

		item = get_driver_parameter(handle, "y2_up", "-2", "0");
		value = strtol(item, &end, 0);
		if (*end == '\0') settings.wheel_2.y_up = value;

		item = get_driver_parameter(handle, "y2_down", "2", "0");
		value = strtol(item, &end, 0);
		if (*end == '\0') settings.wheel_2.y_down = value;
	}

	unload_driver_settings(handle);
}

int32
TwoWheels::StringToKey(const char* key)
{
	if (key == NULL) return 0;

	if      (!strcmp(key, "B_SHIFT_KEY"))			return B_SHIFT_KEY;
	else if (!strcmp(key, "B_COMMAND_KEY"))			return B_COMMAND_KEY;
	else if (!strcmp(key, "B_CONTROL_KEY"))			return B_CONTROL_KEY;
	else if (!strcmp(key, "B_CAPS_LOCK"))			return B_CAPS_LOCK;
	else if (!strcmp(key, "B_SCROLL_LOCK"))			return B_SCROLL_LOCK;
	else if (!strcmp(key, "B_NUM_LOCK"))			return B_NUM_LOCK;
	else if (!strcmp(key, "B_OPTION_KEY"))			return B_OPTION_KEY;
	else if (!strcmp(key, "B_MENU_KEY"))			return B_MENU_KEY;
	else if (!strcmp(key, "B_LEFT_SHIFT_KEY"))		return B_LEFT_SHIFT_KEY;
	else if (!strcmp(key, "B_RIGHT_SHIFT_KEY"))		return B_RIGHT_SHIFT_KEY;
	else if (!strcmp(key, "B_LEFT_COMMAND_KEY"))	return B_LEFT_COMMAND_KEY;
	else if (!strcmp(key, "B_RIGHT_COMMAND_KEY"))	return B_RIGHT_COMMAND_KEY;
	else if (!strcmp(key, "B_LEFT_CONTROL_KEY"))	return B_LEFT_CONTROL_KEY;
	else if (!strcmp(key, "B_RIGHT_CONTROL_KEY"))	return B_RIGHT_CONTROL_KEY;
	else if (!strcmp(key, "B_LEFT_OPTION_KEY"))		return B_LEFT_OPTION_KEY;
	else if (!strcmp(key, "B_RIGHT_OPTION_KEY"))	return B_RIGHT_OPTION_KEY;
	else											return 0;
}

TwoWheels::~TwoWheels()
{
}

status_t TwoWheels::InitCheck()
{
	return B_OK;
}

//------------------------------------------------------------------------------

filter_result
TwoWheels::Filter(BMessage *message, BList *outList)
{
	filter_result result = B_DISPATCH_MESSAGE;
	uint32 mods = modifiers();

	if (!(mods & settings.bypass_key)) // if bypass_key is (also) pressed
	{
		switch (message->what)
		{
			case B_MOUSE_WHEEL_CHANGED:
			{
				float delta_x = 0, delta_y = 0;
				float wheel_delta_x = 0, wheel_delta_y = 0;

				if ((message->FindFloat("be:wheel_delta_x", &delta_x) != B_OK) ||
					(message->FindFloat("be:wheel_delta_y", &delta_y) != B_OK))
				{
					break; // if the data is not there... runaway like a coward.
				}

				// Do we need to toggle wheels?
				bool tk = (mods & settings.wheel_toggle_key);
				bool tg = settings.toggle_wheels;
				bool toggle = (tk ? (tg ? false : true) : tg);

				// if we "detect" the second wheel...
				// XXX - maybe these checks are not enough.
				if ((delta_x == settings.wheel_2.x_up || delta_x == settings.wheel_2.x_down) &&
					(delta_y == settings.wheel_2.y_up || delta_y == settings.wheel_2.y_down))
				{
					delta_y == settings.wheel_2.y_up ? delta_x = -1 : delta_x = 1;

					// If there's a need for speed...
					if (settings.speedup && (mods & settings.accel_key))
						delta_x *= settings.speedup;

					wheel_delta_x = toggle ? 0 : delta_x;
					wheel_delta_y = toggle ? delta_x : 0;
				}
				else // First (or unique) wheel.
				{
					if (settings.speedup && (mods & settings.accel_key))
					{
						delta_x *= settings.speedup;
						delta_y *= settings.speedup;
					}

					wheel_delta_x = toggle ? delta_y : delta_x;
					wheel_delta_y = toggle ? delta_x : delta_y;
				}

				// don't multiply by -1 if wheel_delta == 0.
				if (settings.invert_horizontal && wheel_delta_x) wheel_delta_x *= -1;
				if (settings.invert_vertical && wheel_delta_y)   wheel_delta_y *= -1;

				message->ReplaceFloat("be:wheel_delta_x", wheel_delta_x);
			  	message->ReplaceFloat("be:wheel_delta_y", wheel_delta_y);
			}
		}
	}
	return result;
}

//------------------------------------------------------------------------------
