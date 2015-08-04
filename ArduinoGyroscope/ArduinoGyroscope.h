/**
 * Arduino - Serial Gyroscope Driver
 * 
 * SerialGyroscope.h
 * 
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_DRIVER_GYROSCOPE_H__
#define __ARDUINO_DRIVER_GYROSCOPE_H__ 1

class ArduinoGyroscope {

    virtual int getRotationX() = 0;
    
    virtual int getRotationY() = 0;
    
    virtual int getRotationZ() = 0;
};

#endif /* __ARDUINO_DRIVER_GYROSCOPE_H__ */
