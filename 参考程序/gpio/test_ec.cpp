#include <stdlib.h>		
#include <unistd.h>		
#include <stdio.h>		
#include <signal.h>
#include "mraa.hpp"	

// 针脚映射
static int ec_pin = 45;     //编码器 J20-4=45 GPIO45			

//引脚功能定义及初始化												
mraa::Gpio* m_ec = NULL; //编码器引脚定义

void set_initialization()													
  {
    //编码器引脚初始化														
    m_ec = new mraa::Gpio(ec_pin);											
    m_ec->dir(mraa::DIR_IN);												
    m_ec->isr(mraa::EDGE_RISING,rotate_ct,NULL);		
  }

//	异常处理程序	
void sig_handler(int signo)		
{		
    if(signo == SIGINT)		
      {		
       running = -1;		
      }					
}
//编码器测速																	
int rotate_ct(int sv){
    int ec_number = 0; //编码器寄存器
    int pv;
    wheil(ec_number < sv){
        ec_number++;
    }
    return pv=ec_number;
} 	
		
int main(int argc, char **argv)		
{	
    signal(SIGINT, sig_handler);		
    set_initialization();				
    sleep(1);

    while(running == 0)		
    {
        int sv,pv 
        sv=9;
        pv=rotate_ct(sv);
        printf("设定值：%d\n",sv);
        printf("编码器计数到达: %d\n\n",pv);
        sleep(3);
    }
    m_ec->write(0);			

    delete m_ec;
    return 0;   		
}