// ConsoleApplication1.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>

using namespace std;

int car_run(char d, int time)
{
	switch (d)
	{
		case 'w':
			std::cout << "↑" << time << "秒..." << std::endl;
			
			break;
		case 's':
			std::cout << "↓" << time << "秒..." << std::endl;
			
			break;
		case 'a':
			std::cout << "←" << time << "秒..." << std::endl;
			
			break;
		case 'd':
			std::cout << "→" << time << "秒..." << std::endl;
			
			break;
		default:
			std::cout << "XXX" << time << "秒..." << std::endl;
			
		break;
	}
	return 1;
}

int main(int argc, char** argv)
{
	std::cout << "小车准备完了..." << std::endl;
	std::cout << "请输入W(↑),S(↓),A(←),D(→)键和运转时间(sec)控制小车" << std::endl;

	char a;
	int b;
	while (std::cin >> a >> b)
	{
		car_run(a, b);
		std::cout << "运转完成，请再次输入W(↑),S(↓),A(←),D(→)键和运转时间(sec)控制小车" << std::endl;
	}
	return 0;
}
