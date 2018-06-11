/* standard headers */
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

/* mraa headers */
#include "mraa.hpp"

std::string getData;
std::string sendData;
/********************************************		
	        	引脚映射											
                     														
********************************************/		
#define UART_PORT 0

/********************************************				
	 
           引脚功能定义及初始化                    														

********************************************/
//UART引脚定义
mraa::Uart* uart0;

void set_initialization(){
    //uart通讯初始化
   uart0 = new mraa::Uart(UART_PORT);		 										 									
   if (uart0->setBaudRate(9600) != mraa::SUCCESS) 
      {
       std::cout << "Error setting parity on UART" << std::endl;
      }		 									
   if (uart0->setMode(8, mraa::UART_PARITY_NONE, 1) != mraa::SUCCESS)
      {
       std::cout << "Error setting parity on UART" << std::endl;
      }	 									
   if (uart0->setFlowcontrol(false, false) != mraa::SUCCESS)   
      {
       std::cout << "Error setting flow control UART" << std::endl;
      }
}

/********************************************								

	    	异常处理程序												
                     														
********************************************/
volatile sig_atomic_t flag = 1;
void sig_handler(int signum)
{
    if (signum == SIGINT) {
        std::cout << "Exiting..." << std::endl;
        flag = 0;
    }
    delete uart0;
}

int main(void)
{
    set_initialization();
    signal(SIGINT, sig_handler);
    while (flag == 1) {
        sendData = "tian wang gai di hu!!!";
        uart0->writeStr(sendData);
        std::cout << sendData <<std::endl;
        sleep(1);
        getData=uart0->readStr(22);
        if(getData == "tian wang gai di hu!!!") {
            std::cout << "bao ta zhen he yao!!!" <<std::endl;
        }
        sleep(1);
    }
    delete uart0;
    return EXIT_SUCCESS;
}
