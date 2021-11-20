# Linux Touchboost
A clone of the Android touchboost feature for Linux devices

This is intended to improve the responsiveness of ARM Linux devices running heavy user interfaces such as Gnome, or heavy programs such as Chromium, without going to drastic and wasteful measures like forcing the CPU and GPU to always run at their maximum speed.

Good results have been seen on various devices such as the Pinebook Pro and Raspberry Pi 4.

## Do you need this?
If any of the following applies to you then probably not:
- You use an x86 device
- You run a light user interface
- Your device already runs smoothly

If all of the following apply to you then you **might** benefit from this:
- You use a device that relies on the "normal" Linux CPU governors
- You run a heavy user interface or heavy software
- Your device feels sluggish even under light load

A good way to test if this will improve performance is to set your CPU and GPU scaling governors to "performance" briefly and see if the system is more responsive, if not then this will not help.

## Installation from source
### Requirements:
- C compiler
- libudev headers

### OpenRC:
```
$ make -j
$ sudo make install-openrc
$ sudo rc-update add touchboost default
$ sudo rc-service touchboost start
```
### systemd:
```
$ make -j
$ sudo make install-systemd
$ sudo systemctl enable --now touchboost.service
```

## Known issues
- Compiling with the clang undefined behaviour sanitizer will lock up when quitting

## Warnings
This program is not yet thoroughly tested, use it at your own risk.

By its nature this program will likely make your device run slightly hotter and use more power, this goes without saying and should already be the case but make sure you have thermal limits in place and plenty of spare battery life.
