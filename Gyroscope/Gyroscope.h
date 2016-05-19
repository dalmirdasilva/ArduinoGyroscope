/**
 * Arduino - Gyroscope Driver
 * 
 * Abstract interface which all gyroscope should implement.
 *
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_DRIVER_GYROSCOPE_H__
#define __ARDUINO_DRIVER_GYROSCOPE_H__ 1

class Gyroscope {

public:

    virtual ~Gyroscope();

    virtual float getRotationX() = 0;
    
    virtual float getRotationY() = 0;
    
    virtual float getRotationZ() = 0;
};

#endif /* __ARDUINO_DRIVER_GYROSCOPE_H__ */
