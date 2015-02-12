# Pato is a HD44780 display driver for Atmel's megaAVR.

## Yet another Arduino library?

Pato is *NOT* Arduino display driver libary. If you need
one, just google it - you will probaly find the LiquidCrystal for
Arduino. Use it, it is great!

Pato is for those who want to save their valuable i/o pins, flash and
RAM memory and offload all the display-related functionality to a
separate processor. Pato provides free, open source display driver for
relatively cheap Atmel microprocessors.

Pato aims to be an all-in-one soluction that integrates different software
components:

 * HD44780 display driver and provides access to the basic display API
 * Communication drivers - UART or I2C
 * Output buffering. Pato stores text to be displayed in memory and allows
   delta modifications only for parts that should change.   
 * String formatting - based on printf library from Atmel. This
   feature is optional and depends on amount of flash memory available
   on specific chip.

## Hey, I can get such a thing on ebay for a buck!

Sure there are other similar solutions. Cheap ones provide only
core functionality, allowing to reduce number of i/o pins used to
drive the display, but software driver is still supposed to be a part
of the main processor firmware. More sophisticated solutions, that are
able to perform string formatting and terminal management are much more
expensive.

## What chips are supported?

My initial goal was to drive the display with Arduino Pro Mini that
can be found on ebay for a few bucks. Thus, Atmega328 and Atmega328p are
supplorted. Atmega2560 is supported too. Support for Attiny2313 is under
development.

## License

BSD License. See LICENSE.md for more info.







