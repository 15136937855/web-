#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <pthread.h>

#include "dev.h"

#include "tcp.h"
#include "camera.h"
#include "jpg.h"
#include "convert.h"

struct jpg_buf_t *jpg = NULL;  
//jpg指向的结构体包含了图片数据，图片大小，温湿度数据，adc数据，还有按钮状态
pthread_t cam_tid,tp_tid,btn_tid,pm2_5_tid;
pthread_mutex_t jpg_mutex;

//参数初始化函数
void initArg();
//视频模块的线程处理函数
void *thread_camera(void *arg);
//服务器模块的线程处理函数
void *thread_server(void *arg);
//温湿度模块的线程处理函数
void *thread_temp(void *arg);
//按键模块的线程处理函数
void *thread_btn(void *arg);
//adc模块的线程处理函数
void *thread_pm2_5(void * arg);
//客户端数据处理函数
extern int client_process(int connfd);


int main(int argc, char *argv[])
{
	
	//函数调用  参数初始化函数  设备初始化函数
 	initArg();
	openDevices();
    pthread_mutex_init(&jpg_mutex, NULL);
    //创建视频模块的线程并判断是否创建成功
	
	int ret = pthread_create(&cam_tid,NULL,thread_camera,NULL);
	if(ret<0){
		printf("create camera failed!\n");
		return -1;
	}
	printf("create camera success\n");
	
	//将线程进行分离，这样线程结束后OS会回收资源
	//否则，需要主线程调用pthread_join来回收资源 
	//视频模块的线程分离
	ret = pthread_detach(cam_tid);
	if (ret) {
		errno = ret;
		perror("detach camera thread");
		exit(EXIT_FAILURE);
	} else
		printf("detach camera thread success\n");
    //创建温湿度模块的线程并判断是否创建成功
    ret = pthread_create(&tp_tid,NULL,thread_temp,NULL);
	if(ret<0){
		printf("create temp failed!\n");
		return -1;
	}
	printf("create temp success\n");
	//温湿度模块的线程分离
	ret = pthread_detach(tp_tid);
	if (ret) {
		errno = ret;
		perror("detach adc thread");
		exit(EXIT_FAILURE);
	} else
		printf("detach adc thread success\n");
	
    //创建按键模块的线程并判断是否创建成功
    ret = pthread_create(&btn_tid,NULL,thread_btn,NULL);
	if(ret<0){
		printf("create btn failed!\n");
		return -1;
	}
	printf("create btn success\n");
	//按键模块的线程分离
	ret = pthread_detach(btn_tid);
	if (ret) {
		errno = ret;
		perror("detach btn thread");
		exit(EXIT_FAILURE);
	} else
		printf("detach btn thread success\n");

	//创建adc模块的线程并判断是否创建成功
	ret = pthread_create(&pm2_5_tid,NULL,thread_pm2_5,NULL);
	if(ret<0){
		printf("create adc failed!\n");
		return -1;
	}
	printf("create adc success\n");
	//adc模块的线程分离
	ret = pthread_detach(pm2_5_tid);
	if (ret) {
		errno = ret;
		perror("detach pm2.5 thread");
		exit(EXIT_FAILURE);
	} else
		printf("detach pm2.5 thread success\n");
	
	//主线程里面执行服务器模块的处理函数
	thread_server(NULL);
	
	closeDevices();
	exit(EXIT_SUCCESS);
}

void initArg()
{
	//相关硬件设备文件的描述符初始化  BOOL类型变量赋值
	//文件描述符
	btnFd = -1;
	ledFd = -1;
	pwmFd = -1;
	adcFd = -1;
	tempFd = -1;
	keyID = -1;

	//模块状态标志位
	isAirConditionOn = FALSE;
	isCurtainOn = FALSE;
	isWaterHeaterOn = FALSE;
	isAlarmOn = FALSE;
}

void *thread_camera(void *arg)
{	
	int fd;
	int ret;
	unsigned int width;
	unsigned int height;
	unsigned int size;
	unsigned int index;
	unsigned int ismjpeg;
	char *yuv;
	char *rgb;

	/* 
	初始化摄像头设备 size=width * height * 2 *, ismjpeg代表格式0：YuYv , 1: mjpeg
	创建了一个有四个结点的队列，用于存放图片数据
	*/
	width = 1024;
	height = 1024;
	fd = camera_init(CAMERA_DEV_PATH, &width, &height, &size, &ismjpeg);
	if (fd == -1)
		exit(EXIT_FAILURE);
	printf("width: %d\n", width);
	printf("height: %d\n", height);

	//开始采集图片
	ret = camera_start(fd);
	if (ret == -1)
		exit(EXIT_FAILURE);

	//采集到的图片信息存放在jpg这个buf中
	if (!jpg)
	{
		jpg = (struct jpg_buf_t *)malloc(sizeof(struct jpg_buf_t));
		if (!jpg) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		memset(jpg, 0, sizeof(struct jpg_buf_t));
	}

    jpg->btn= 'A';
	//如果不是jpeg需要由YuYv转成rgb再压缩成Jpeg
	if (!ismjpeg) {
		rgb = malloc(width * height * 3);
		convert_rgb_to_jpg_init();		
	}

	fprintf(stdout, "init camera success\n");


	/* 循环采集图片 */
	while (1) {
		/*
		现在摄像头已经初始化完成，并且开始采集图片
		采集到的图片在队列里
		index表示出队的结点的编号 从0到3
		出队：可以得到一张YuYv的图片  
		*/
		ret = camera_dqbuf(fd, (void **)&yuv, &size, &index);
		if (ret == -1)
			exit(EXIT_FAILURE);

        convert_yuv_to_rgb(yuv, rgb, width, height, 24);
		pthread_mutex_lock(&jpg_mutex);
		jpg->len = convert_rgb_to_jpg_work(rgb, jpg->buf, width, height, 24, 80);
		pthread_mutex_unlock(&jpg_mutex);
		
		/*
		将刚才出队的结点再次入队，可以继续存放图片
		*/
		ret = camera_eqbuf(fd, index);
		if (ret == -1)
			exit(EXIT_FAILURE);
	}

	/* 如果不是Jpeg图，需要释放 */
	if (!ismjpeg) {
		convert_rgb_to_jpg_exit();
		free(rgb);
	}
	free(jpg);
	jpg = NULL;

	ret = camera_stop(fd);
	if (ret == -1)
		exit(EXIT_FAILURE);

	ret = camera_exit(fd);
	if (ret == -1)
		exit(EXIT_FAILURE);
}

void *thread_server(void *arg)
{
	int connfd;
	//调用服务器初始化函数并判断是否成功初始化
	int listenfd = tcp_server_init();
	if(listenfd==-1)
	{
		printf("服务器初始化失败！\n");
		return NULL;
	}else{
		printf("服务器初始化成功！\n");
	}
	//调用接受客户端连接请求函数并进行数据处理
	while (1) {
		
		if ((connfd = tcp_server_wait_connect(listenfd)) != -1) 
		{	
			while (1) 
			{
				if (client_process(connfd)<0)
				{
					//断开连接
					tcp_server_disconnect(connfd);
					break;
				}
			}
		}
	}
	printf("wochulaile \n");
}

void *thread_temp(void *arg)
{
   tempProcess();
   return NULL;
}

void *thread_pm2_5(void * arg)
{
	adcProcess();
	return NULL;
}

void *thread_btn(void *arg)
{
	keyProcess();  
	return NULL;
}
