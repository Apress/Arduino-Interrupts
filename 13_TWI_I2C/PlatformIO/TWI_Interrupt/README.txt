TWI_Interrupt

This sketch uses the AVRTWILIB to read and write as many bytes of data are required from almost any TWIO/I2C devices.

It sends config data to a sensor to select an internal register to read from, then reads the data requested. It shows how a repeated start can be used between wriotes and reads to t he same peripheral, to avoid losing control of the TWI/I2C bus.


