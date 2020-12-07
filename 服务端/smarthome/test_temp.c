#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "dev.h"
#include "jpg.h"

int tempProcess()
{
	while(1)
	{
		//获取温湿度数据
		char ch,zh;
		char t[6] = {0};
		while(1)
		{
			lseek(tempFd,0,SEEK_SET);
			while(1)
			{
				read(tempFd,&ch,1);
				if(ch == '\n')
					break;
			}
			while(1)
			{
				read(tempFd,&zh,1);
				if(zh == '=')
					break;
			}
			read(tempFd,t,5);
			printf("%s\n",t);

			strcpy(jpg->tp,t);
		}
	   
	    	if (NULL == jpg)
			continue;
		pthread_mutex_lock(&jpg_mutex);
		//将获取的温湿度数据赋值给jpg所指向的tp数组
		
		//jpg = (struct jpg_buf_t *)malloc(sizeof(struct jpg_buf_t));
	
		
		pthread_mutex_unlock(&jpg_mutex);
	}
	return 0;
}
