The various directories contained within this top level directory, contain separate libraries developed during the course of the book. These are to be used with the PlatformIO projects to facilitate such things as those already present in the Arduino Languge/libraries. Serial output, for example, using the USART. 


Libraries included are:

* printf - allows PlatformIO to use printf() function calls to send mixed text and variable data/values etc to the USART. There is an installable library, libprintf, for the Arduino IDE.

* TWI - an interrupt driven slightly updaed version of Chris Herrin's AVRTWILIB from 2014.

* USARTbuffer - a circular buffer implementation, specifically written to mimic the Arduino implementation used when communicating with Serial (the USART).

* USARTinterrupt - an interruipt driven manner of talking to the USART from non-Arduino projects.

Other libraries may be added from time to time.



If the PlatformIO project needs to use any of the libraries, then add the following line to the platformio.ini file:

lib_extra_dirs = ../../../PlatformIO.libraries

This makes this directory almost like a global library directory where all required libraries can be found. 

The path can be relative, as above, or absolute, as in:

lib_extra_dirs = /home/norman/SourceCode/ArdinoInterrupts/code/PlatformIO.libraries

Which is a bit long-winded, and given that all my code examples live somewhere under /home/norman/SourceCode/ArdinoInterrupts/code then a relative path is easier. It should be obvious, that your absolute path will not necessarily be the same as mine, which is another reason to use a relative one.

Unlike the Arduino IDE, PlatformIO uses a separate library for each project, usually in the lib directory. This allows some projects to use different versions of the same library -- impossible using Arduino libraries in the IDE, where everything is global. Using "lib_extra_dirs" allows PlatformIO to use pseudo-global libraries.

In addition, it saves me having copies of the same code all over the directory structure for the PlatformIO projects.


Norman Dunbar
24th August 2020.


