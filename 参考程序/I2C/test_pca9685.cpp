#include <iostream>
#include <signal.h>

#include "pca9685.hpp"
#include "upm_utilities.h"

using namespace std;

//引脚功能定义及初始化
// Instantiate an PCA9685 on I2C bus 6
upm::PCA9685 pwms(PCA9685_I2C_BUS, PCA9685_DEFAULT_I2C_ADDR);

//初始化程序
void set_initialization()
{
    // put device to sleep
    pwms.setModeSleep(true);
    // setup a period of 50Hz
    pwms.setPrescaleFromHz(50);
    // wake device up
    pwms.setModeSleep(false);
    // Setup a 100% duty cycle -- on time at 0, off time at 4095 (50% ==2048)
    // Set for all channels
    pwms.ledOnTime(PCA9685_ALL_LED, 0);
    pwms.ledOffTime(PCA9685_ALL_LED, 4095);

    pwms.ledFullOff(0, true);
    pwms.ledFullOff(1, true);
    pwms.ledFullOff(2, true);
    pwms.ledFullOff(3, true);
    pwms.ledFullOff(4, true);
    pwms.ledFullOff(5, true);
    pwms.ledFullOff(6, true);
    pwms.ledFullOff(7, true);
    pwms.ledFullOff(8, true);
    pwms.ledFullOff(9, true);
    pwms.ledFullOff(10, true);
    pwms.ledFullOff(11, true);
    pwms.ledFullOff(12, true);
    pwms.ledFullOff(13, true);
    pwms.ledFullOff(14, true);
    pwms.ledFullOff(15, true);

}

//异常处理程序
volatile sig_atomic_t flag = 1;
void sig_handler(int signum)
{
    if (signum == SIGINT)
    {
        std::cout << "Exiting..." << std::endl;
        flag = 0;
    }
}

//前左轮前进
void WHEEL_FL_fwd()
{
    pwms.ledFullOff(0, true);
    pwms.ledFullOff(1, false);
}

//前左轮后退
void WHEEL_FL_bk()
{
    pwms.ledFullOff(0, false);
    pwms.ledFullOff(1, true);
}

//前右轮前进
void WHEEL_FR_fwd()
{
    pwms.ledFullOff(2, true);
    pwms.ledFullOff(3, false);
}

//前右轮后退
void WHEEL_FR_bk()
{
    pwms.ledFullOff(2, false);
    pwms.ledFullOff(3, true);
}

//后左轮前进
void WHEEL_BL_fwd()
{
    pwms.ledFullOff(4, true);
    pwms.ledFullOff(5, false);
}

//后左轮后退
void WHEEL_BL_bk()
{
    pwms.ledFullOff(4, false);
    pwms.ledFullOff(5, true);
}
//后右轮前进
void WHEEL_BR_fwd()
{
    pwms.ledFullOff(6, true);
    pwms.ledFullOff(7, false);
}

//后右轮后退
void WHEEL_BR_bk()
{
    pwms.ledFullOff(6, false);
    pwms.ledFullOff(7, true);
}

//车体前进
void car_fwd()
{
    WHEEL_FL_fwd();
    WHEEL_FR_fwd();
    WHEEL_BL_fwd();
    WHEEL_BR_fwd();
}
//车体后退
void car_bk()
{
    WHEEL_FL_bk();
    WHEEL_FR_bk();
    WHEEL_BL_bk();
    WHEEL_BR_bk();
}
//车体左平移
void car_shift_l()
{
    WHEEL_FL_bk();
    WHEEL_FR_fwd();
    WHEEL_BL_fwd();
    WHEEL_BR_bk();
}

//车体右平移
void car_shift_r()
{
    WHEEL_FL_fwd();
    WHEEL_FR_bk();
    WHEEL_BL_bk();
    WHEEL_BR_fwd();
}

//车体左转
void car_turn_l()
{
    WHEEL_FL_fwd();
    WHEEL_FR_bk();
    WHEEL_BL_fwd();
    WHEEL_BR_bk();
}
//车体右转
void car_turn_r()
{
    WHEEL_FL_bk();
    WHEEL_FR_fwd();
    WHEEL_BL_bk();
    WHEEL_BR_fwd();
}

//斜跑。。。
//to do


int main(int argc, char** argv)
{
    set_initialization();
    signal(SIGINT, sig_handler);

    cout << "小车准备完了..." << endl;
    sleep(1);

    while(flag==1)
    {
        cout << "小车前进..." << endl;
        car_fwd();
        sleep(5);

        cout << "小车后退..." << endl;
        car_bk();
        sleep(5);

        cout << "小车左移..." << endl;
        car_shift_l;
        sleep(5);
        
        cout << "小车右移..." << endl;
        car_shift_r;
        sleep(5);

        cout << endl;
    }
    return EXIT_SUCCESS;
}