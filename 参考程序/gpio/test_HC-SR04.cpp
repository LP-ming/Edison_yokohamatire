#include <stdlib.h>		
#include <unistd.h>		
#include <stdio.h>		
#include <signal.h>
#include<sys/time.h>
#include "mraa.hpp"	

// 针脚映射
static int trig_pin = 31;  //HC-SR04触发 J19-4=31 GPIO44
static int echo_pin = 32;  //HC-SR04接收 J19-5=32 GPIO46	

//引脚功能定义及初始化
mraa::Gpio* us_trig = NULL;	//HC-SR04触发
mraa::Gpio* us_echo = NULL;	//HC-SR04接收

void set_initialization()													
  {
    //HC-SR04初始化
    us_trig = new mraa::Gpio(trig_pin);
    us_trig->dir(mraa::DIR_OUT);
    us_trig->write(0);

    us_echo = new mraa::Gpio(echo_pin);
    us_echo->dir(mraa::DIR_IN);
    us_echo->read();
  }

//	异常处理程序	
void sig_handler(int signo)		
{		
    if(signo == SIGINT)		
      {		
       running = -1;		
      }			
      us_trig->write(0); 		
}
//HC-SR04测距
int measure_distance(us_trig, us_echo){			
			
	int on_flag, off_flag;		
	struct timeval t_on, t_off, t_start, t_judge;		
	float duration, distance;		
			
	//触发信号，发射超声波，至少10us		
	us_trig->write(1);		
	usleep(20);		
	us_trig->write(0);		
	//读取接收信号并计算距离
	duration=0;
	on_flag=0;		
	off_flag=0;
	gettimeofday(&t_start, NULL);		
	while(off_flag==0){
		if(on_flag==0){	
        	if(us_echo->read() == 1){
                gettimeofday(&t_on, NULL);
                on_flag=1;
			}
            else if(us_echo->read() == 0){
                    gettimeofday(&t_judge, NULL);
                    if((t_judge.tv_usec-t_start.tv_usec)>=10000){
                        return distance=450;
                    }
			}
		}
		if(on_flag==1){	
			if(us_echo->read()==1){
                gettimeofday(&t_judge, NULL);
                if((t_judge.tv_usec-t_start.tv_usec)>=60000){
                    return distance=460;
                }
			}
            else if(us_echo->read()==0){
                    gettimeofday(&t_off, NULL);
                    off_flag=1;      
			}
		}
    }		
    duration=t_off.tv_usec-t_on.tv_usec;
	return distance=duration/58;
}			
		
int main(int argc, char **argv)		
{	
    float distance1;
    signal(SIGINT, sig_handler);		
    set_initialization();				
    sleep(1);

    while(running == 0)		
    {
        distance1=measure_distance(us_echo, us_echo);
        printf("障碍物距离: %fcm\n",distance1);
        sleep(3);//测距周期60ms以上
    }				
    us_trig->write(0);	
    
    delete us_echo;	
    delete us_trig;	   					
    return 0;   		
}