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
//jpgָ��Ľṹ�������ͼƬ���ݣ�ͼƬ��С����ʪ�����ݣ�adc���ݣ����а�ť״̬
pthread_t cam_tid,tp_tid,btn_tid,pm2_5_tid;
pthread_mutex_t jpg_mutex;

//������ʼ������
void initArg();
//��Ƶģ����̴߳�����
void *thread_camera(void *arg);
//������ģ����̴߳�����
void *thread_server(void *arg);
//��ʪ��ģ����̴߳�����
void *thread_temp(void *arg);
//����ģ����̴߳�����
void *thread_btn(void *arg);
//adcģ����̴߳�����
void *thread_pm2_5(void * arg);
//�ͻ������ݴ�����
extern int client_process(int connfd);


int main(int argc, char *argv[])
{
	
	//��������  ������ʼ������  �豸��ʼ������
 	initArg();
	openDevices();
    pthread_mutex_init(&jpg_mutex, NULL);
    //������Ƶģ����̲߳��ж��Ƿ񴴽��ɹ�
	
	int ret = pthread_create(&cam_tid,NULL,thread_camera,NULL);
	if(ret<0){
		printf("create camera failed!\n");
		return -1;
	}
	printf("create camera success\n");
	
	//���߳̽��з��룬�����߳̽�����OS�������Դ
	//������Ҫ���̵߳���pthread_join��������Դ 
	//��Ƶģ����̷߳���
	ret = pthread_detach(cam_tid);
	if (ret) {
		errno = ret;
		perror("detach camera thread");
		exit(EXIT_FAILURE);
	} else
		printf("detach camera thread success\n");
    //������ʪ��ģ����̲߳��ж��Ƿ񴴽��ɹ�
    ret = pthread_create(&tp_tid,NULL,thread_temp,NULL);
	if(ret<0){
		printf("create temp failed!\n");
		return -1;
	}
	printf("create temp success\n");
	//��ʪ��ģ����̷߳���
	ret = pthread_detach(tp_tid);
	if (ret) {
		errno = ret;
		perror("detach adc thread");
		exit(EXIT_FAILURE);
	} else
		printf("detach adc thread success\n");
	
    //��������ģ����̲߳��ж��Ƿ񴴽��ɹ�
    ret = pthread_create(&btn_tid,NULL,thread_btn,NULL);
	if(ret<0){
		printf("create btn failed!\n");
		return -1;
	}
	printf("create btn success\n");
	//����ģ����̷߳���
	ret = pthread_detach(btn_tid);
	if (ret) {
		errno = ret;
		perror("detach btn thread");
		exit(EXIT_FAILURE);
	} else
		printf("detach btn thread success\n");

	//����adcģ����̲߳��ж��Ƿ񴴽��ɹ�
	ret = pthread_create(&pm2_5_tid,NULL,thread_pm2_5,NULL);
	if(ret<0){
		printf("create adc failed!\n");
		return -1;
	}
	printf("create adc success\n");
	//adcģ����̷߳���
	ret = pthread_detach(pm2_5_tid);
	if (ret) {
		errno = ret;
		perror("detach pm2.5 thread");
		exit(EXIT_FAILURE);
	} else
		printf("detach pm2.5 thread success\n");
	
	//���߳�����ִ�з�����ģ��Ĵ�����
	thread_server(NULL);
	
	closeDevices();
	exit(EXIT_SUCCESS);
}

void initArg()
{
	//���Ӳ���豸�ļ�����������ʼ��  BOOL���ͱ�����ֵ
	//�ļ�������
	btnFd = -1;
	ledFd = -1;
	pwmFd = -1;
	adcFd = -1;
	tempFd = -1;
	keyID = -1;

	//ģ��״̬��־λ
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
	��ʼ������ͷ�豸 size=width * height * 2 *, ismjpeg�����ʽ0��YuYv , 1: mjpeg
	������һ�����ĸ����Ķ��У����ڴ��ͼƬ����
	*/
	width = 1024;
	height = 1024;
	fd = camera_init(CAMERA_DEV_PATH, &width, &height, &size, &ismjpeg);
	if (fd == -1)
		exit(EXIT_FAILURE);
	printf("width: %d\n", width);
	printf("height: %d\n", height);

	//��ʼ�ɼ�ͼƬ
	ret = camera_start(fd);
	if (ret == -1)
		exit(EXIT_FAILURE);

	//�ɼ�����ͼƬ��Ϣ�����jpg���buf��
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
	//�������jpeg��Ҫ��YuYvת��rgb��ѹ����Jpeg
	if (!ismjpeg) {
		rgb = malloc(width * height * 3);
		convert_rgb_to_jpg_init();		
	}

	fprintf(stdout, "init camera success\n");


	/* ѭ���ɼ�ͼƬ */
	while (1) {
		/*
		��������ͷ�Ѿ���ʼ����ɣ����ҿ�ʼ�ɼ�ͼƬ
		�ɼ�����ͼƬ�ڶ�����
		index��ʾ���ӵĽ��ı�� ��0��3
		���ӣ����Եõ�һ��YuYv��ͼƬ  
		*/
		ret = camera_dqbuf(fd, (void **)&yuv, &size, &index);
		if (ret == -1)
			exit(EXIT_FAILURE);

        convert_yuv_to_rgb(yuv, rgb, width, height, 24);
		pthread_mutex_lock(&jpg_mutex);
		jpg->len = convert_rgb_to_jpg_work(rgb, jpg->buf, width, height, 24, 80);
		pthread_mutex_unlock(&jpg_mutex);
		
		/*
		���ղų��ӵĽ���ٴ���ӣ����Լ������ͼƬ
		*/
		ret = camera_eqbuf(fd, index);
		if (ret == -1)
			exit(EXIT_FAILURE);
	}

	/* �������Jpegͼ����Ҫ�ͷ� */
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
	//���÷�������ʼ���������ж��Ƿ�ɹ���ʼ��
	int listenfd = tcp_server_init();
	if(listenfd==-1)
	{
		printf("��������ʼ��ʧ�ܣ�\n");
		return NULL;
	}else{
		printf("��������ʼ���ɹ���\n");
	}
	//���ý��ܿͻ����������������������ݴ���
	while (1) {
		
		if ((connfd = tcp_server_wait_connect(listenfd)) != -1) 
		{	
			while (1) 
			{
				if (client_process(connfd)<0)
				{
					//�Ͽ�����
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
