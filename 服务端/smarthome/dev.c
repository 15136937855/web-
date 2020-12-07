#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "dev.h"

void openDevices()
{
	//打开key并判断是否打开成功
	btnFd = open(KEY_FILENAME,O_RDWR);
	if(btnFd < 0)
	{
		perror("btn");
	}
	printf("open btn success!\n");
	
	//打开led并判断是否打开成功
	ledFd = open(LED_FILENAME,O_RDWR);
	if(ledFd < 0)
	{
		perror("led");
		return ;
	}
	printf("open led success!\n");
	
	//打开pwm并判断是否打开成功
	pwmFd = open(PWM_FILENAME,O_RDWR);
	if(pwmFd < 0)
	{
		perror("pwm");
		return ;
	}
	printf("open pwm success!\n");
	
	//打开adc并判断是否打开成功
	adcFd = open(ADC_FILENAME,O_RDONLY);
	if(adcFd < 0)
	{
		perror("adc");
		return ;
	}
	printf("open adc success!\n");
	
	//打开temp并判断是否打开成功，但是temp的文件名需要手动scanf输入
	tempFd = open(TEMP_FILENAME,O_RDONLY);
	if(tempFd < 0)
	{
		perror("temp");
		return ;
	}
	printf("open temp success!\n");
	
}
void closeDevices()
{
	//关闭所有文件描述符
	close(btnFd);
	close(ledFd);
	close(pwmFd);
	close(adcFd);
	close(tempFd);
}
