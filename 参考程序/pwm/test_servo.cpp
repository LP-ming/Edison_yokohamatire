#include <stdlib.h>		
#include <unistd.h>		
#include <stdio.h>		
#include <signal.h>		
#include "mraa.hpp"		

// 针脚映射		
static int servo_pin = 20;  //转向舵机 J18-7=20 PWM0	

//引脚功能定义及初始化 	
mraa::Pwm* servo = NULL;//转向舵机

void set_initialization()													
  {
    //转向舵机驱动初始化
    servo = new mraa::Pwm(servo_pin);							
    servo->write(0.000);						
    servo->enable(true);												
    servo->period_ms(20);
    servo->write(0.051);
    //电平转换芯片初始化
  }	
		
//	异常处理程序	
void sig_handler(int signo)		
{		
    if(signo == SIGINT)		
      {		
       running = -1;		
      }			
      servo->write(0);   		
}		

/*********************************										
   周期 = 20ms时 小车角度 = 占空比												
 0=0.025;  15=0.033; 30=0.041;	
 45=0.050; 60=0.058; 75=0.067;
 90=0.075; 120=0.091         												
 *********************************/										

int main(int argc, char **argv)		
{	
    int running = 0;	
    signal(SIGINT, sig_handler);		
    set_initialization();				
    sleep(1);				
    while(running == 0)		
    {		
        servo->write(0.025);//0度	
        sleep(1000);
        servo->write(0.050);//45度
        sleep(1000);
        servo->write(0.075);//90度
        sleep(1000);
        servo->write(0.100);//135度
        sleep(1000);
        servo->write(0.125);//180度
        sleep(1000);
    }	
    servo->write(0.0);		
    delete servo;	
    return 0;   		
}		
