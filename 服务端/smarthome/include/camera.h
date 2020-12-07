#ifndef __CAM_H__
#define __CAM_H__

/*
camera_init
初始化摄像头设备
参数：devPath是文件名
width是图的宽，一般为1024
height是图的高，一般为1024
size是图的大小，size=width * height * 2
ismjpeg代表格式0：YuYv , 1: mjpeg
返回值：失败为-1，成功时大于0
*/
int camera_init(char *devPath, unsigned int *width, unsigned int *height, unsigned int *size, unsigned int *ismjpeg);
/*
camera_start
启动摄像头
参数: fd为camera_init的返回值
返回值：失败为-1，成功为0
*/
int camera_start(int fd);
/*
camera_dqbuf
出队：可以得到一张YuYv的图片
参数：fd为camera_init的返回值
buf指向存放YuYv图片的首地址，
size是图片的大小
index:将返回已经出队的结点编号
返回值：失败为-1，成功为0
*/
int camera_dqbuf(int fd, void **buf, unsigned int *size, unsigned int *index);
/*
camera_eqbuf
入队：将已经出队的结点再次入队
参数：fd为camera_init的返回值
index已经出队的结点编号
返回值：失败为-1，成功为0
*/
int camera_eqbuf(int fd, unsigned int index);
/*
camera_stop
停止采集
参数：fd为camera_init的返回值
返回值：失败为-1，成功为0
*/
int camera_stop(int fd);
/*
camera_exit
关闭设备
参数：fd为camera_init的返回值
返回值：失败为-1，成功为0
*/
int camera_exit(int fd);

#endif
