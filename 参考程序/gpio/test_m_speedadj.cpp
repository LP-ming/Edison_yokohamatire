#include <stdlib.h>	
#include <unistd.h>	
#include <stdio.h>	
#include <signal.h>		
#include "mraa.hpp"	

// 针脚映射		
static int speed_pin = 14; //马达速度 J18-1=14 PWM1	
static int dir_p_pin = 47; //马达正转 J20-6=47 GPIO49	
static int dir_l_pin = 48; //马达反转 J20-7=48 GPIO15	
static int oe_pin = 46;    //电平转换芯片使能 J20-5=46 GPIO46	
	
//引脚功能定义及初始化 	

mraa::Pwm* m_speed = NULL;	//马达速度
mraa::Gpio* m_dir_p = NULL;	//马达方向
mraa::Gpio* m_dir_l = NULL;	//马达方向
mraa::Gpio* txb0108_oe = NULL; //电平转换芯片使能

void set_initialization()													
  {
    //马达驱动初始化
    m_speed = new mraa::Pwm(speed_pin);
    m_speed->write(0.0);
    m_speed->enable(true);
    m_speed->period_ms(20);

    m_dir_p = new mraa::Gpio(dir_p_pin);
    m_dir_p->dir(mraa::DIR_OUT);
    m_dir_p->write(0);
    m_dir_l = new mraa::Gpio(dir_l_pin);
    m_dir_l->dir(mraa::DIR_OUT);
    m_dir_l->write(0);
    //电平转换芯片初始化
    txb0108_oe = new mraa::Gpio(oe_pin);
    txb0108_oe->dir(mraa::DIR_OUT);		
    txb0108_oe->write(1);	
  }	

//	异常处理程序
void sig_handler(int signo)	
{	
    if(signo == SIGINT)	
      {	
       printf("closing PWM nicely\n");	
       running = -1;	
      }		
      m_speed->write(0.0);
      m_dir_p->write(0);    	
      m_dir_l->write(0);		
      txb0108_oe->write(0);	
}	
	
int main(int argc, char **argv)	
{
    int running = 0;
    int i;	
    float cycle = 0.1;	
    signal(SIGINT, sig_handler);	
	set_initialization();
	sleep(1);		
  	
   while (running == 0)	
   {	  	
    //正转	   
	m_dir_p->write(0);
    m_dir_l->write(1);	
	
    //调速	   
    for(i=0;i<10;i++)
    {          	
	    m_speed->write(cycle);
        cycle += 0.1;	
	    printf("duty-cycle: m_speed:%f",m_speed->read());
	    sleep(3);
    }	
    //反转	
    m_dir_p->write(1);	
    m_dir_l->write(0);	
	
    //调速	
    for(i=0;i<10;i++)	
    {  	
	    m_speed->write(cycle);
      	cycle -= 0.1;
	    printf("duty-cycle: m_speed:%f",m_speed->read());
	    sleep(3);
    }	
   }	
   m_speed->write(0.0);	
   m_dir_p->write(0);	
   m_dir_l->write(0);	
   txb0108_oe->write(0);

   delete m_speed;	
   delete m_dir_p;	
   delete m_dir_l;	
   delete txb0108_oe;	
   return 0;   	
}	
