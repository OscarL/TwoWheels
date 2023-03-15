#  ~TwoWheels~ FreeWheely

This simple `input_server` filter allows you to:

 - Invert the scroll direction of your scroll wheel(s).
 - Speed up the scrolling by your choosen factor (only when you ask it to).
 - Toggle your Vertical / Horizontal wheel(s). Works even if you only have one.

Code quality might not be the best, but it is pretty short, so it might serve as an example/template for other people's filters?


## Installation

Edit the `FreeWheely.settings` to suit your needs. Drop a copy of (or symlink to) that file in the `Drop settings here` symlinked directory.

```sh
> cd filter && make
```

Then drag and drop the resulting `FreeWheely` binary into the `Drop filter here` symlinked directory.


## Uninstallation

Just remove the `FreeWheely` binary from the `Drop filter here` symlinked directory.


## WheelTest

```sh
> cd test && make
> ./WheelTest
```

Place your mouse cursor over the window, and move the scroll wheel(s) to see what values the system gets.


## Doesn't works!!!

Double check that there's no conflict between the keys you selected as modifiers, and the ones used by the app where this filter seems to fail.

(e.g,: does your editor *already* uses SHIFT+scroll_wheel to do fasters scrolls?)


## License

MIT.

----

(happy now, Habbie? :-P)
