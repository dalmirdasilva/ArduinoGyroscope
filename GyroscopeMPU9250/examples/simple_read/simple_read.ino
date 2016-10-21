#include <Wire.h>
#include <WiredDevice.h>
#include <RegisterBasedWiredDevice.h>
#include <Gyroscope.h>
#include <GyroscopeMPU9250.h>

GyroscopeMPU9250 gyro(0);

void setup() {
    Serial.begin(9600);
    gyro.enableAxis(GyroscopeMPU9250::AXIS_XYZ);
    gyro.setFullScaleRange(GyroscopeMPU9250::FS_SEL_1000DPS);
    gyro.setOutputDataRate(GyroscopeMPU9250::ODR_1_188HZ);
}

void loop() {
    Serial.print("x: ");
    Serial.println(gyro.getRotationX());
    Serial.print("y: ");
    Serial.println(gyro.getRotationY());
    Serial.print("z: ");
    Serial.println(gyro.getRotationZ());
    Serial.println("-----------");
    delay(1000);
}