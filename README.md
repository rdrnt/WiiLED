# WiiLED

A homebrew channel app to demonstrate manipulating the Wii disc drive LED.

I created this a couple of years ago when learning about Wii Homebrew. The code isn't impressive, but I hope someone can learn something from this :)

## How to run

1. Install [devkitppc](https://wiibrew.org/wiki/DevkitPPC) and setup [WiiLoad](https://wiibrew.org/wiki/Wiiload)

2. Run `make`

3. When the build is complete, run `./wiiload.py ./boot.dol` to load into onto your Wii without having to insert/eject an SD card.
