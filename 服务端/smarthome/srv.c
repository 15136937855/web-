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

int processClientCmd(int connfd)  //�ͻ��˴�������ҽ��д���
{
	//cmd��������������տͻ��˷�������ʵ����Ϣ  
	//cmd[1]�����������ĸ��豸   cmd[2]�����Ǹ�ʲô����
	char cmd[CMDLEN] = {0};
	//���տͻ��˴���������Ϣ  recv����read
	read(connfd,cmd,5); //010
	printf("%c\n",cmd[0]);
	printf("%c\n",cmd[1]);
	printf("%c\n",cmd[2]);
	printf("%c\n",cmd[3]);
	printf("%c\n",cmd[4]);
	//��������  ��cmd[1]������������������ģ��
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

int client_process(int connfd)  //���տͻ��˴���������Ϣ
{
	char cmd[4]={0};
	//int total = 0;
	//int ret1;
	//��jpg�����ڴ�ռ�
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
	//���տͻ��˴���������Ϣ�����ж������ǲ���"cmd"���ǵĻ�˵��Ҫ���������������תִ��processClientCmd()����
	memset(cmd,0,sizeof(cmd));
	read(connfd,cmd,3);
	
	printf("%s\n",cmd);
	if(strcmp(cmd,"cmd")==0)
		return processClientCmd(connfd);
	pthread_mutex_lock(&jpg_mutex);
	//������ǣ���������ݰ���ʽ�����

	//struct stat stBuf = {0};
	//stat("3.jpg", &stBuf);
	//jpg->len = stBuf.st_size;
	
	//�������ݵĸ�ʽ��ͼƬ����#�¶�#��ť#adc    ��ʾ��snprintf����   strcat����
	//int snprintf ( char * str, size_t size, const char * format, ... );
	char buf[20];
	snprintf(buf,20,"%d#%s#%c#%s#",jpg->len,jpg->tp,jpg->btn,jpg->pm2_5);
	//jpg->len
	
	//����ͼƬ���ݵĿռ��С��ͼƬ��С��󣬾�������Ŀռ�   malloc������
	unsigned char *sizeImg = (unsigned char *)malloc(jpg->len);
	sizeImg = jpg->buf;
	
	
	pthread_mutex_unlock(&jpg_mutex);
	
	//��ͻ��˷�������õ�����
	int res = write(connfd,buf,strlen(buf));
	if(res<0){
		printf("send fail!\n");
		return -1;
	}
	//��ͻ��˷���ͼƬ����
	printf("%s\n",buf);
	int ret = write(connfd,sizeImg,jpg->len);
	if(ret < 0)
	{
		perror("send error!");
		return -1;
	}
	return 0;
}
