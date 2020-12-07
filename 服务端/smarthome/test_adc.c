#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "dev.h"
#include "jpg.h"

int adcProcess()
{
	int adcValue = 0;
	while(1)
    {
		//获取adc的值
		ioctl(adcFd,ADC_CMD_GET,&adcValue);
		//将adcValue转成0~500的数
		adcValue = adcValue*5/100;
		if (!jpg)
			continue;
		pthread_mutex_lock(&jpg_mutex);
		//将adcValue转成一个字符串并赋值给jpg所指向的pm2_5数组  提示：sprintf函数
		sprintf(jpg->pm2_5,"%d",adcValue);
		pthread_mutex_unlock(&jpg_mutex);
       
   }
	return 0;
}

