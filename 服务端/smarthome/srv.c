#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <error.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "dev.h"
#include "tcp.h"
#include "jpg.h"

#define REQ_DATA_SIZE   32
#define HDR_DATA_SIZE   128

//#define DEBUG
#define CMDLEN 6
extern pthread_mutex_t jpg_mutex;

int processClientCmd(int connfd)  //客户端传的命令并且进行处理
{
	//cmd这个数组用来接收客户端发过来的实际信息  
	//cmd[1]代表具体操作哪个设备   cmd[2]代表是个什么操作
	char cmd[CMDLEN] = {0};
	//接收客户端传过来的信息  recv或者read
	read(connfd,cmd,5); //010
	printf("%c\n",cmd[0]);
	printf("%c\n",cmd[1]);
	printf("%c\n",cmd[2]);
	printf("%c\n",cmd[3]);
	printf("%c\n",cmd[4]);
	//处理命令  看cmd[1]传的数据来操作具体模块
	switch(cmd[1])
	{
		case '0':if(cmd[2]=='0')
					ledProcess(LED0_ID,CMD_TWO);
				 else if(cmd[2]=='1')
					ledProcess(LED0_ID,CMD_ONE);
				 break;
		case '1':if(cmd[2]=='0')
					ledProcess(LED1_ID,CMD_TWO);
				 else if(cmd[2]=='1')
					ledProcess(LED1_ID,CMD_ONE); 
				 break;
		case '2':if(cmd[2]=='0')
					ledProcess(LED2_ID,CMD_TWO);
				 else if(cmd[2]=='1')
					ledProcess(LED2_ID,CMD_ONE);
				 break;
		case '3':if(cmd[2]=='0')
					ledProcess(LED3_ID,CMD_TWO);
				 else if(cmd[2]=='1')
					ledProcess(LED3_ID,CMD_ONE);
				 break;
		case '4':if(cmd[2]=='0')
					pwmProcess(CMD_TWO);
				 else if(cmd[2]=='1')
					pwmProcess(CMD_ONE); 
				 break;
		
	}
	
	return 0;
}

int client_process(int connfd)  //接收客户端传过来的信息
{
	char cmd[4]={0};
	//int total = 0;
	//int ret1;
	//给jpg申请内存空间
	if (NULL == jpg)
	{
		printf("jpg has no memory alloc\r\n");
		jpg = (struct jpg_buf_t *)malloc(sizeof(struct jpg_buf_t));
		if (!jpg) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		memset(jpg, 0, sizeof(struct jpg_buf_t));
	}
	//接收客户端传过来的信息，先判断数据是不是"cmd"，是的话说明要进行命令处理，所以跳转执行processClientCmd()函数
	memset(cmd,0,sizeof(cmd));
	read(connfd,cmd,3);
	
	printf("%s\n",cmd);
	if(strcmp(cmd,"cmd")==0)
		return processClientCmd(connfd);
	pthread_mutex_lock(&jpg_mutex);
	//如果不是，将相关数据按格式整理好

	//struct stat stBuf = {0};
	//stat("3.jpg", &stBuf);
	//jpg->len = stBuf.st_size;
	
	//发送数据的格式：图片长度#温度#按钮#adc    提示：snprintf函数   strcat函数
	//int snprintf ( char * str, size_t size, const char * format, ... );
	char buf[20];
	snprintf(buf,20,"%d#%s#%c#%s#",jpg->len,jpg->tp,jpg->btn,jpg->pm2_5);
	//jpg->len
	
	//申请图片内容的空间大小（图片大小多大，就申请多大的空间   malloc函数）
	unsigned char *sizeImg = (unsigned char *)malloc(jpg->len);
	sizeImg = jpg->buf;
	
	
	pthread_mutex_unlock(&jpg_mutex);
	
	//向客户端发送整理好的数据
	int res = write(connfd,buf,strlen(buf));
	if(res<0){
		printf("send fail!\n");
		return -1;
	}
	//向客户端发送图片内容
	printf("%s\n",buf);
	int ret = write(connfd,sizeImg,jpg->len);
	if(ret < 0)
	{
		perror("send error!");
		return -1;
	}
	return 0;
}
