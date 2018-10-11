# summerbot-motionbase
Control library for the summerbot's motion base

This library is meant to control the servos in the base of the summerbot.

## Usage
You can order add a move to the move list with:
~~~~
translate(d)
rotate(a)
moveto(x,y)
moveto(x,y,a)

translateRPM(d,RPM)
rotateRPM(a,RPM)
movetoRPM(x,y,RPM)
movetoRPM(x,y,a,RPM)
~~~~
You have to call `update()` at a high frequency (higher than 10x your max Steps per seconds)\
You can also get and set position\
and make a pause at any moment with `pause()` and `unpause()`