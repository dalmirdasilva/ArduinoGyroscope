#include "GyroscopeMPU9250.h"

GyroscopeMPU9250::GyroscopeMPU9250(bool ad0)
        : RegisterBasedWiredDevice(MPU9250_ADDRESS | (ad0 & 0x01)) {
    config.value = 0x00;
}

float GyroscopeMPU9250::getRotationX() {
    return readAxisRotation(GYRO_XOUT_H);
}

float GyroscopeMPU9250::getRotationY() {
    return readAxisRotation(GYRO_YOUT_H);
}

float GyroscopeMPU9250::getRotationZ() {
    return readAxisRotation(GYRO_ZOUT_H);
}

unsigned char GyroscopeMPU9250::readXYZ(unsigned char *buf) {
    return readRegisterBlock(GYRO_XOUT_H, buf, 6);
}

float GyroscopeMPU9250::readAxisRotation(unsigned char axisRegister) {
    unsigned char buf[2];
    if (readRegisterBlock(axisRegister, buf, 2) != 2) {
        return 0.0;
    }
    return convertToDegreePerSeconds(buf);
}

void GyroscopeMPU9250::setFullScaleRange(FullScaleRange fsr) {
    configureRegisterBits(GYRO_CONFIG, GYRO_CONFIG_GYRO_FS_SEL, (unsigned char) fsr);
    config.GYRO_FS_SEL = fsr >> 3;
}

void GyroscopeMPU9250::selectClock(ClockSelection cs) {
    configureRegisterBits(PWR_MGMT_1, PWR_MGMT_1_CLKSEL, (unsigned char) cs);
}

void GyroscopeMPU9250::reset() {
    configureRegisterBits(PWR_MGMT_1, PWR_MGMT_1_H_RESET, 0xff);
}

void GyroscopeMPU9250::sleep() {
    configureRegisterBits(PWR_MGMT_1, PWR_MGMT_1_SLEEP, 0xff);
}

void GyroscopeMPU9250::awake() {
    configureRegisterBits(PWR_MGMT_1, PWR_MGMT_1_SLEEP, 0x00);
}

void GyroscopeMPU9250::enableAxis(Axis axis) {
    configureRegisterBits(PWR_MGMT_2, PWR_MGMT_2_DISABLE_G, ~axis);
}

float GyroscopeMPU9250::convertToDegreePerSeconds(unsigned char buf[2]) {
    int raw = (buf[0] << 8) | buf[1];
    float counts[] = { 131.0, 65.5, 32.8, 16.4 };
    return raw / counts[config.GYRO_FS_SEL];
}
