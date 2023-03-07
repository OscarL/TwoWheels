//------------------------------------------------------------------------------
// *** No mice were injured during the production of this code. ***
//------------------------------------------------------------------------------

#ifndef _TWO_WHEELS_H_
#define _TWO_WHEELS_H_

//------------------------------------------------------------------------------
#define SETTINGS_DIR "/boot/home/config/settings/"
#define SETTINGS_FILE SETTINGS_DIR "two_wheels.settings"
//------------------------------------------------------------------------------

struct wheel_data {
	float x_up;
	float y_up;
	float x_down;
	float y_down;
};

struct mouse_settings {
	wheel_data wheel_1;
	wheel_data wheel_2;

	bool toggle_wheels;
	bool invert_horizontal;
	bool invert_vertical;

	uint32 speedup;

	uint32 accel_key;
	uint32 wheel_toggle_key;
	uint32 bypass_key;
};

//------------------------------------------------------------------------------

extern "C" _EXPORT BInputServerFilter* instantiate_input_filter();

class TwoWheels : public BInputServerFilter 
{
public:
			TwoWheels();
	virtual ~TwoWheels();
	virtual	filter_result Filter(BMessage *message, BList *outList);
   	virtual status_t InitCheck();
private:
	mouse_settings settings;

	void	LoadSettings();
	int32	StringToKey(const char* key);
};

//------------------------------------------------------------------------------

#endif // _TWO_WHEELS_H_
