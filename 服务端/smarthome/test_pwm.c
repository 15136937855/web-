#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "dev.h"
#include "jpg.h"

int pwmProcess(int cmd)
{
	//通过参数cmd以及宏isAlarmOn来判断是打开蜂鸣器还是关闭蜂鸣器
	//还要注意jpg这个结构体里面的btn的值，打开蜂鸣器btn等于'B',关闭蜂鸣器btn等于'A'
        if(isAlarmOn==FALSE&&cmd==1)
		{
			ioctl(pwmFd,PWM_CMD_ON);
			jpg->btn='B';
            isAlarmOn = TRUE;
		}
        if(isAlarmOn==TRUE&&cmd==2)
		{
			ioctl(pwmFd,PWM_CMD_OFF);
			jpg->btn='A';
            isAlarmOn = FALSE;
		}
	return 0;
}
