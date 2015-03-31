# Pato is a HD44780 display driver for Atmel's megaAVR.

Pato turns Arduino Pro Mini (or any other Atmega based board) and its
attached HD44780-based display into printf() machine with small
subset of VT100 controls.

## Yet another Arduino library?

Pato is *NOT* Arduino display driver libary. If you need
one, just google it - you will probaly find the LiquidCrystal for
Arduino. Use it, it is great!

Pato is for those who want to save their valuable i/o pins, flash and
RAM memory and offload all the display-related functionality to a
separate processor. Pato provides free, open source display driver for
relatively cheap Atmel microprocessors.

This said, Pato actually contains HD44780 driver that can be used in
Arduino sketches. It is written in assembly and optimized to use minimal
amount of flash memory (208 bytes .text code segment size in hd44780.o) and
have small stack footprint (3 nested calls maximum).

Pato aims to be an all-in-one solution that integrates different software
components:

 * HD44780 display driver - provides access to the basic display API
 * Communication drivers - UART or I2C (TWI in Atmel terms)
 * Output buffering. Pato stores text to be displayed in memory and allows
   delta modifications only for parts that should change.   
 * String formatting - based on printf library from Atmel. This
   feature is optional and depends on amount of flash memory available
   on specific chip.
 * VT100 terminal controls support. Right now only basic control characters
   are supported.

## Hey, I can get such a thing on ebay for a buck!

Sure there are other similar solutions. Cheap ones provide only
core functionality, allowing to reduce number of i/o pins used to
drive the display, but software driver is still supposed to be a part
of the main processor firmware. More sophisticated solutions, that are
able to perform string formatting and terminal management are much more
expensive and are often closed source.

## What chips are supported?

My initial goal was to drive the display with Arduino Pro Mini that
can be found on ebay for a few bucks. Thus, Atmega328 and Atmega328p are
supplorted. Atmega2560 is supported too. Support for Attiny2313 is under
development.

In fact, porting to any Atmega is trivial, provided it has enough flash
memory, RAM, i/o pins and UART/TWI compatible with Atmega328.

## License

BSD License. See LICENSE.md for more info.







