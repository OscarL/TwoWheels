/*
 * Copyright 2004, 2023, Oscar Lesta, <oscar.lesta@gmail.com>.
 * Distributed under the terms of the MIT License.
 */

#include <driver_settings.h>
#include <FindDirectory.h>
#include <InputServerFilter.h>
#include <InterfaceDefs.h>
#include <Path.h>

#include <string>
#include <unordered_map>

//------------------------------------------------------------------------------

#define SETTINGS_FILE_NAME "FreeWheely.settings"


struct filter_settings {
	bool toggle_wheels;
	bool invert_horizontal;
	bool invert_vertical;

	uint32 speedup;

	uint32 accel_key;
	uint32 wheel_toggle_key;
	uint32 bypass_key;
};


class FreeWheely : public BInputServerFilter 
{
public:
			FreeWheely();
	virtual	filter_result Filter(BMessage* message, BList* outList);
private:
	void	LoadSettings();
	int32	StringToKey(const char* key);

	filter_settings fSettings;
};

//------------------------------------------------------------------------------

extern "C" _EXPORT BInputServerFilter* instantiate_input_filter()
{
	return new FreeWheely();
}

//------------------------------------------------------------------------------

static std::unordered_map<std::string, int32> sKeys = {
	{ "B_SHIFT_KEY", B_SHIFT_KEY },
	{ "B_COMMAND_KEY", B_COMMAND_KEY },
	{ "B_CONTROL_KEY", B_CONTROL_KEY },
	{ "B_CAPS_LOCK", B_CAPS_LOCK },
	{ "B_SCROLL_LOCK", B_SCROLL_LOCK },
	{ "B_NUM_LOCK", B_NUM_LOCK },
	{ "B_OPTION_KEY", B_OPTION_KEY },
	{ "B_MENU_KEY", B_MENU_KEY },
	{ "B_LEFT_SHIFT_KEY", B_LEFT_SHIFT_KEY },
	{ "B_RIGHT_SHIFT_KEY", B_RIGHT_SHIFT_KEY },
	{ "B_LEFT_COMMAND_KEY", B_LEFT_COMMAND_KEY },
	{ "B_RIGHT_COMMAND_KEY", B_RIGHT_COMMAND_KEY },
	{ "B_LEFT_CONTROL_KEY", B_LEFT_CONTROL_KEY },
	{ "B_RIGHT_CONTROL_KEY", B_RIGHT_CONTROL_KEY },
	{ "B_LEFT_OPTION_KEY", B_LEFT_OPTION_KEY },
	{ "B_RIGHT_OPTION_KEY", B_RIGHT_OPTION_KEY }
};


FreeWheely::FreeWheely()
{
	LoadSettings();
}

void
FreeWheely::LoadSettings()
{
	BPath path;
	status_t status = find_directory(B_USER_SETTINGS_DIRECTORY, &path);
	if (status == B_OK)
		path.Append(SETTINGS_FILE_NAME);
	else
		path.SetTo("/boot/home/config/settings/" SETTINGS_FILE_NAME);

	int fd = open(path.Path(), O_RDONLY);
	if (fd <= 0)
		return;

	void* handle = load_driver_settings_file(fd);

	if (handle != NULL)	{
		const char* item;
		char* end;
		int32 value;

		fSettings.toggle_wheels = get_driver_boolean_parameter(handle, "toggle_wheels", false, true);
		fSettings.invert_horizontal = get_driver_boolean_parameter(handle, "invert_horizontal", false, true);
		fSettings.invert_vertical = get_driver_boolean_parameter(handle, "invert_vertical", false, true);

		// SpeedUp Factor
		item = get_driver_parameter(handle, "speedup_factor", "2", "0");
		value = strtoul(item, &end, 0);
		if (*end == '\0')
			fSettings.speedup = value;

		// Keys
		item = get_driver_parameter(handle, "accel_key", "", "");
		fSettings.accel_key = StringToKey(item);

		item = get_driver_parameter(handle, "wheel_toggle_key", "B_SCROLL_LOCK", "B_SCROLL_LOCK");
		fSettings.wheel_toggle_key = StringToKey(item);

		item = get_driver_parameter(handle, "bypass_key", "", "");
		fSettings.bypass_key = StringToKey(item);
	}

	unload_driver_settings(handle);
	close(fd);
}

int32
FreeWheely::StringToKey(const char* key)
{
	if (key == NULL)
		return 0;

	return sKeys[key];
}


filter_result
FreeWheely::Filter(BMessage* message, BList* outList)
{
	filter_result result = B_DISPATCH_MESSAGE;
	uint32 mods = modifiers();

	// if bypass_key is (also) pressed
	if (!(mods & fSettings.bypass_key)) {
		switch (message->what)
		{
			case B_MOUSE_WHEEL_CHANGED:
			{
				float delta_x = 0;
				float delta_y = 0;

				if ((message->FindFloat("be:wheel_delta_x", &delta_x) != B_OK) ||
					(message->FindFloat("be:wheel_delta_y", &delta_y) != B_OK))
					break;

				if (fSettings.speedup && (mods & fSettings.accel_key)) {
					delta_x *= fSettings.speedup;
					delta_y *= fSettings.speedup;
				}

				// Do we need to toggle wheels?
				bool tk = (mods & fSettings.wheel_toggle_key);
				bool tg = fSettings.toggle_wheels;
				bool toggle = (tk ? (tg ? false : true) : tg);

				float wheel_delta_x = toggle ? delta_y : delta_x;
				float wheel_delta_y = toggle ? delta_x : delta_y;

				// don't multiply by -1 if wheel_delta == 0.
				if (fSettings.invert_horizontal && wheel_delta_x)
					wheel_delta_x *= -1;
				if (fSettings.invert_vertical && wheel_delta_y)
					wheel_delta_y *= -1;

				message->ReplaceFloat("be:wheel_delta_x", wheel_delta_x);
			  	message->ReplaceFloat("be:wheel_delta_y", wheel_delta_y);
			}
		}
	}
	return result;
}
