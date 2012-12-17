# LED Display Sign

This C project has three components:
* A dot-matrix display sign formatter with an 8px ASCII font
* An e-mail inbox interface for posting messages on the sign
* A display sign controller for scrolling through the available messages.


## Dot-Matrix Display Formatter (LDF)

The command-line formatter was written to produce bitmaps for a frickin' laser display
but the format of the .LDM binary bitmap files can be adapted for
[off-the-shelf LED displays](http://www.ebay.com/sch/i.html?_nkw=programmable+led+sign) instead.
(Our laser display had optics to scan and modulate an 8W laser over a 1024x24px bitmap
but we did not have the money or safety clearence to use it with a laser any more than a few mW.)


## E-mail Interface (LDF3)

Messages are posted to the display sign by emailing them to the sign's email address.
Because SMS also uses SMTP/POP3 it is easy to convert this to an SMS interface
but you will also want to simplify the [TROFF](http://troff.org)-like syntax of the message for SMS users.


## Sign Controller (LASER)

This component is written to drive our awesome 8W laser display attached by parallel printer port
to an MS-DOS PC.
It scans the contents of a directory for .LDM files and using their .FR(om) and .TO times and .PR(iority)
settings builds a playlist of messages to scroll through on the display sign.
It adds stock 'house' messages to the playlist if there is nothing else to display.


## Utilities

* VIEWLDM prints the contents of a .LDM (Laser/Led Display Message) file to the console.
* STDMESS/ direcory contains the stock 'house' messages
* TEST/ directory contains some example messages
* LINEUP is a utility you won't need with an LED display sign. (It was needed with our laser display prototype to compensate for imperfections in the milling of the raster-scanning reflector surfaces and determine the values of lineorder[] and lineoffset[] arrays used by set() in LDFBMAP.C to correct to the actual geometry of the scan pattern.)


## User Guide

[https://raw.github.com/rfinean/led-display-sign/master/HELP/README.TXT](https://github.com/rfinean/led-display-sign/tree/master/HELP#readme)