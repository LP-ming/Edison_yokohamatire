#include <iostream>
#include <stdint.h>

#include "lsm303dlh.hpp"

int main(int argc, char** argv)
{
    // Instantiate LSM303DLH compass on I2C
    upm::LSM303DLH sensor(6);

    // 获取坐标数据
    sensor.getCoordinates();
    int16_t* coor = sensor.getRawCoorData(); // in XYZ order.
    // The sensor returns XZY, but the driver compensates and makes it XYZ

    // Print out the X, Y, and Z 坐标数据 using two different methods
    std::cout << "coor: rX " << (int) coor[0] << " - rY " << (int) coor[1] << " - rZ "
              << (int) coor[2] << std::endl;
    // std::cout << "coor: gX " << sensor.getCoorX() << " - gY " << sensor.getCoorY() << " - gZ "
    //           << sensor.getCoorZ() << std::endl;

    // Get and print out the heading
    std::cout << "heading: " << sensor.getHeading() << std::endl;

    // 获取加速度
    sensor.getAcceleration();
    int16_t* accel = sensor.getRawAccelData();
    // Print out the X, Y, and Z 加速度数据 using two different methods
    std::cout << "acc: rX " << (int) accel[0] << " - rY " << (int) accel[1] << " - Z "
              << (int) accel[2] << std::endl;
    // std::cout << "acc: gX " << sensor.getAccelX() << " - gY " << sensor.getAccelY() << " - gZ "
    //           << sensor.getAccelZ() << std::endl;

    return 0;
}
