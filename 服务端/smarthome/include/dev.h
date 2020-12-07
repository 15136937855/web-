#ifndef _DEV_H_
#define _DEV_H_


typedef enum status
{
	FALSE,
	TRUE,
}BOOL;

#include <unistd.h>
#include <sys/ioctl.h>

//fileName
#define LED_FILENAME "/dev/led"
#define KEY_FILENAME "/dev/key"
#define PWM_FILENAME "/dev/pwm0"
#define ADC_FILENAME "/dev/adc0"
#define TEMP_FILENAME "/sys/bus/w1/devices/28-0000083d8640/w1_slave"


//led
#define HELLO_MAGIC  'h'
#define CMD_ONE	1
#define CMD_TWO 2
#define LED_ON _IOW(HELLO_MAGIC, CMD_ONE, int)
#define LED_OFF _IOW(HELLO_MAGIC, CMD_TWO, int)

//pwm
#define PWM_MAGIC   'P'
#define PWM_CMD_ON  _IO(PWM_MAGIC, CMD_ONE)
#define PWM_CMD_OFF _IO(PWM_MAGIC, CMD_TWO)

//adc 
#define ADC_MAGIC  'A'
#define ADC_CMD_GET  _IOR(ADC_MAGIC, CMD_ONE, int)

//camera
#define CAMERA_DEV_PATH	"/dev/video0"

#define AIRCONDITION 0
#define CURTAIN      1
#define WATERHEATER  2

/*
LED0: XX=00, YY=00为off, YY=01为on
LED1: XX=01, YY=00为off, YY=01为on
LED2: XX=02, YY=00为off, YY=01为on
LED3: XX=03, YY=00为off, YY=01为on
PWM:  XX=04, YY=00为off, YY=01为on
*/
#define LED0_ID 0

#define LED1_ID 1
#define LED2_ID 2
#define LED3_ID 3
#define PWM_ID  4

//文件描述符
int btnFd;
int ledFd;
int pwmFd;
int adcFd;
int tempFd;
int keyID;

//模块状态标志位
BOOL isAirConditionOn;
BOOL isCurtainOn;
BOOL isWaterHeaterOn;
BOOL isAlarmOn;

void openDevices();
void closeDevices();

int ledProcess(int ledNum, int cmd);
int adcProcess();
int keyProcess();
int pwmProcess(int cmd);
int tempProcess();

/*
客户端给服务器发的数据是 FFXXYYZZFF
服务器给客户端发的数据是：
	1。5位图片长度#5位温度#按键#3位pm2.5的值
	2。循环发送图片数据
*/
#endif
