/**
 * Arduino - Gyroscope Driver
 *
 * Implementation for MPU9250.
 *
 * @author Dalmir da Silva <dalmirdasilva@gmail.com>
 */

#ifndef __ARDUINO_DRIVER_GYROSCOPE_MPU9250_H__
#define __ARDUINO_DRIVER_GYROSCOPE_MPU9250_H__ 1

#include <Gyroscope.h>
#include <RegisterBasedWiredDevice.h>

#define MPU9250_ADDRESS 0x68

/**
 * The MPU-9250 consists of three independent vibratory MEMS rate gyroscopes, which detect rotation about
 * the X-, Y-, and Z- Axes. When the gyros are rotated about any of the sense axes, the Coriolis Effect causes
 * a vibration that is detected by a capacitive pickoff. The resulting signal is amplified, demodulated, and filtered
 * to produce a voltage that is proportional to t he angular rat e. This voltage is digitized using individual on-chip
 * 16-bit Analog-to-Digital Converters (ADCs) to sample each axis. The full-scale range of the gyro sensors
 * may be digitally programmed to ±250, ±500, ±1000, or ±2000 degrees per second (dps). The ADC sample
 * rate is programmable from 8,000 s amples per second, down to 3.9 samples per second, and us er-s electable
 * low-pass filters enable a wide range of cut-off frequencies.
 */
class GyroscopeMPU9250 : public Gyroscope, public RegisterBasedWiredDevice {

public:

    enum Register {
        CONFIG = 0x1a,
        PWR_MGMT_1 = 0x6b,
        PWR_MGMT_2 = 0x6c,
        GYRO_CONFIG = 0x1b,
        GYRO_XOUT_H = 0x43,
        GYRO_XOUT_L = 0x44,
        GYRO_YOUT_H = 0x45,
        GYRO_YOUT_L = 0x46,
        GYRO_ZOUT_H = 0x47,
        GYRO_ZOUT_L = 0x48,
    };

    /**
     * Gyroscope Full Scale Select
     *
     * <pre>
     * FS1 FS0  g Range
     * 0    0   +250dps
     * 0    1   +500dps
     * 1    0   +1000dps
     * 1    1   +2000dps
     * </pre>
     */
    enum FullScaleRange {
        FS_SEL_250DPS = 0x00,
        FS_SEL_500DPS = 0x08,
        FS_SEL_1000DPS = 0x10,
        FS_SEL_2000DPS = 0x18
    };

    enum ClockSelection {
        INTERNAL_20MHZ_OSCILLATOR = 0x00,
        BEST_AVAILABLE_SOURCE = 0x01,
        STOPS_CLOCK_KEEPS_TIMING = 0x07,
    };

    /**
     * Axis
     */
    enum Axis {
        AXIS_NONE = 0x00,
        AXIS_X = 0x04,
        AXIS_Y = 0x02,
        AXIS_Z = 0x01,
        AXIS_XY = AXIS_X | AXIS_Y,
        AXIS_XZ = AXIS_X | AXIS_Z,
        AXIS_YZ = AXIS_Y | AXIS_Z,
        AXIS_XYZ = AXIS_X | AXIS_Y | AXIS_Z
    };

    /**
     * Some useful masks.
     */
    enum Mask {
        GYRO_CONFIG_GYRO_FS_SEL = 0x18,
        GYRO_CONFIG_FCHOICE_B = 0x03,
        PWR_MGMT_2_DISABLE_G = 0x07,
        PWR_MGMT_1_H_RESET = 0x80,
        PWR_MGMT_1_SLEEP = 0x40,
        PWR_MGMT_1_CYCLE = 0x20,
        PWR_MGMT_1_GYRO_STANDBY = 0x10,
        PWR_MGMT_1_CLKSEL = 0x0,
        CONFIG_EXT_SYNC_SET = 0x38,
        CONFIG_DLPF_CFG = 0x07
    };

    /**
     * Gyroscope Configuration (GYRO_CONFIG 0x1b)
     * Serial IF: R/W
     * Reset value: 0x00
     */
    union GYRO_CONFIGbits {
        struct {
            unsigned char FCHOICE_B :2;
            unsigned char :1;
            unsigned char GYRO_FS_SEL :2;
            unsigned char ZGYRO_CTEN :1;
            unsigned char YGYRO_CTEN :1;
            unsigned char XGYRO_Cten :1;
        };
        unsigned char value;
    };

    /**
     * Power Management 1
     * Serial IF: R/W
     * Reset value: 0x00
     */
    union PWR_MGMT_1bits {
        struct {
            unsigned char CLKSEL :3;
            unsigned char PD_PTAT :1;
            unsigned char GYRO_STANDBY :1;
            unsigned char CYCLE :1;
            unsigned char SLEEP :1;
            unsigned char H_RESET :1;
        };
        unsigned char value;
    };

    /**
     * Power Management 2
     * Serial IF: R/W
     * Reset value: 0x00
     */
    union PWR_MGMT_2bits {
        struct {
            unsigned char DISABLE_ZG :1;
            unsigned char DISABLE_YG :1;
            unsigned char DISABLE_XG :1;
            unsigned char DISABLE_ZA :1;
            unsigned char DISABLE_YA :1;
            unsigned char DISABLE_XA :1;
            unsigned char :2;
        };
        struct {
            unsigned char DISABLE_G :3;
            unsigned char DISABLE_A :3;
            unsigned char :2;
        };
        unsigned char value;
    };

    /**
     * The DLPF is configured by DLPF_CFG, when FCHOICE_B [1:0] = 2b’00. The gyroscope and
     * temperature sensor are filtered according to the value of DLPF_CFG and FCHOICE_B as shown in
     * the table below. Note that FCHOICE mentioned in the table below is the inverted value of
     * FCHOICE_B (e.g. FCHOICE=2b’00 is same as FCHOICE_B=2b’11
     *
     * <pre>
     * FCHOICE  DLPF_CFG
     * <1>  <0>     Bandwidth   Delay
     * x    0   x   4000 Hz     0.04 ms
     * 0    1   x   4000 Hz     0.04 ms
     * 1    1   0   4000 Hz     0.04 ms
     * 1    1   1   188 Hz      1.9 ms
     * 1    1   2   98  Hz      2.8  ms
     * 1    1   3   42 Hz       4.8 ms
     * 1    1   4   20 Hz       8.3 ms
     * 1    1   5   10 Hz       13.4 ms
     * 1    1   6   5 Hz        18.6 ms
     * 1    1   7   4000 Hz     0.04 ms
     * </pre>
     */
    enum OutputDataRate {
        ODR_0_4000HZ = 0x00,
        ODR_1_188HZ = 0x01,
        ODR_2_98HZ = 0x02,
        ODR_3_42HZ = 0x03,
        ODR_4_20HZ = 0x04,
        ODR_5_10HZ = 0x05,
        ODR_6_5HZ = 0x06,
        ODR_7_4000HZ = 0x07
    };

    /**
     * Public constructor.
     *
     * @param ad0   LSBit of the device address.
     */
    GyroscopeMPU9250(bool ad0);

    float getRotationX();

    float getRotationY();

    float getRotationZ();

    unsigned char readXYZ(unsigned char *buf);

    float readAxisRotation(unsigned char axisRegister);

    void setFullScaleRange(FullScaleRange fsr);

    /**
     * Set the output data rate.
     *
     * @param odr       Output data rate.
     * @see
     */
    void setOutputDataRate(OutputDataRate odr);

    void selectClock(ClockSelection cs);

    void reset();

    void sleep();

    void awake();

    void enableAxis(Axis axis);

    float convertToDegreePerSeconds(unsigned char buf[2]);

private:

    GYRO_CONFIGbits config;
};

#endif /* __ARDUINO_DRIVER_GYROSCOPE_MPU9250_H__ */
