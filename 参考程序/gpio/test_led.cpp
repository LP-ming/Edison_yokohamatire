#include <stdlib.h>		
#include <unistd.h>		
#include <stdio.h>		
#include <signal.h>
#include<sys/time.h>
#include "mraa.hpp"	

// 针脚映射
static int led_pin = 15;  //led灯 J18-2=15 GPIO165	

//引脚功能定义及初始化
mraa::Gpio* led1 = NULL;	//led灯

void set_initialization()													
  {
    //led灯初始化
    led1 = new mraa::Gpio(led_pin);
    led1->dir(mraa::DIR_OUT);
    led1->write(0);
  }

//	异常处理程序	
void sig_handler(int signo)		
{		
    if(signo == SIGINT)		
      {		
       running = -1;		
      }			
      led1->write(0); 		
}

int main(int argc, char **argv)		
{	
    signal(SIGINT, sig_handler);		
    set_initialization();				
    sleep(1);

    while(running == 0)		
    {
        led1->write(1); 
        printf("led灯亮\n");
        sleep(3);//闪烁间隔3s
        led1->write(0); 
        printf("led灯灭\n");
        sleep(3);//闪烁间隔3s
    }				
    led1->write(0);	
    
    delete led1;	   					
    return 0;   		
}