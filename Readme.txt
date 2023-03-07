(Hell on) TwoWheels version 1.1
by Oscar Lesta (BiPolar <BiPolar@SoftHome.net>)

What is this?
It's just an input_server filter add-on that lets you make (proper) use your secondary mouse wheel (if you have one, of course).

Features:
- Maps your secondary wheel for proper horizontal scrolling.
- Configurable modifier keys for the following actions (These work like "on the fly" options):
	- Exchange wheels (vertical becames horizontal and viceversa).
	- Scroll Speed Up (x times faster scrolling while pressing this key).
	- Bypass filter (temporaly revert to Be's default behavior).
- Configurable "speedup_factor".
- Invert vertical and/or horizontal wheels movement.

If your mouse only have one wheel (or this filter is unable to detect the second) you can still make use of the other features (ie, invert the scroll movement, doing an horizontal scroll by holding down the toggle key while using the wheel, etc).

How to install it?
Either use the provided installer/uninstaller or, if the test fails but you want to try it anyway, do the following:

- move "TwoWheels" to "/boot/home/config/add-ons/input_server/filters"
- Read, edit and move "two_wheels.settings" to "/boot/home/config/settings/"
- restart the input_server (From Terminal: "input_server -q" will do it).
- Test it in a well behaved app (StyledEdit, Tracker, Net+, etc).

Why did you wrote it?
Because I wanted to use the two wheels, duh!
And because I wanted to use them while the nice fellows of OBOS came up with a better kb_mouse driver.

What I need to know?
The WheelTest app sucks, so... be sure to do minimal wheel moves when requested, otherwise it could get confused.

If it says that doesn't work for your mouse, give up.

Otherwise it _could_ work, but be sure that _you_ can diferenciate the wheels by inspecting the values for them (if you can, the filter should do, just put those values in the settings file).

- Be sure to restart the input_server if you make changes to the settings file.
- This works like a charm with most apps, but not with _all_ of them.
  * BePDF: I guess next release will include support for horizontal scrolling (thanks Michael)
  * R5's Terminal + zadig's patch: works but horizontal wheel is not ignored as it should.
  * Apps filtering the B_MOUSE_WHEEL_CHANGED message:
     these could "eat-up" the modifier keys and/or ignore the speedup and/or ignore the
     horizontal scroll (go blame the respective authors :-) ).

Interaction with other input filters: works OK with InstantClick, almost sure it/others will interfer. Your milleage will vary.

Recognized key names (in the settings file) are:
B_SHIFT_KEY
B_COMMAND_KEY
B_CONTROL_KEY
B_CAPS_LOCK
B_SCROLL_LOCK
B_NUM_LOCK
B_OPTION_KEY
B_MENU_KEY
B_LEFT_SHIFT_KEY
B_RIGHT_SHIFT_KEY
B_LEFT_COMMAND_KEY
B_RIGHT_COMMAND_KEY
B_LEFT_CONTROL_KEY
B_RIGHT_CONTROL_KEY
B_LEFT_OPTION_KEY
B_RIGHT_OPTION_KEY

(note: don't try to "or" them, won't work)

For the wheels_toggle_key I suggest you:
if the filter detects both wheels:	B_SCROLL_LOCK
if it doesn't, but you use it anyway?	B_LEFT_COMMAND_KEY or B_LEFT_CONTROL_KEY.

How the magic happens?
For some mice, Be's kb_mouse driver reports diferent values in the "be:wheel_delta_x" and "be:wheel_delta_y" fields of the B_MOUSE_WHEEL_CHANGED message depending on which wheel was moved. This filter attemps to discriminate which one according to those values and re-arranges the message so it looks as it should.

What users have said of it?
- BiPolar wrote: "Uh! What a great addon! I've being looking for something like this since I started using BeOS. Thx!" [/me hopes nobody realize that its me who made that statement]

Mini-FAQ:
Q: It's true that this Readme and the install scripts are larger than the code?
A: In two words? Yes. :-P

Q: My mouse reports different values for each wheel/move but your test app/filter refuses to work. Should I report this?
A: Please, do so. Use the address at the top of this file. Don't forget to include the values that the WheelTest app outputs to the Terminal (or the ones you can read on its window if no output is generated) along with the name/type of your mouse. Thanks.

Q.: Does it works on Exp/Dano?
A.: Yes.

Q: Your english seems to be broken.
A: Well, not a real question but... Yes, its called "Broken English Incide" (Be Inc. for short).

Thanks to:
Be Inc. - you know why. Also for the input_server addons arch.
Axel Dörfler - for his OBOS's driver_settings.{h,c} files.

Version Info:
1.1 -	Now it obeys the "invert wheel" parameters of the settings file and the wheels_toggle
	key. (17-Mar-2004)
	Note to self: next time upload the correct files, and make sure the code reflects what
	you think, not what you type, duh!.
1.0 -	It will _possibily_ work for others. First release. (16-Mar-2004)
0.2 -	May work for others. (10-Mar-2004)
0.1 -	It works for me. (8-Mar-2004)

Unavoidable legal nonsense lines:
This files are released under the OpenBeOS License (BSD/MIT).
Do what you want with this, just don't sue me.

No mice were injured during the production of this code.