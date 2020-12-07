#ifndef __JPG_H__
#define __JPG_H__

#define MAX_JPG_SIZE	(1024 * 1024 - sizeof (unsigned int))

struct jpg_buf_t {
	unsigned char buf[MAX_JPG_SIZE];// 图片数据
	unsigned int len;//图片大小
    char tp[6];//温度   
	char pm2_5[4];//pm2.5是一个三位数的整数
    unsigned char btn;//按钮  ‘A’ 'B' 
};

extern struct jpg_buf_t *jpg;
extern pthread_mutex_t jpg_mutex;
#endif
