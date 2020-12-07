#ifndef _HELLO_H_
#define _HELLO_H_

#include <asm/ioctl.h>

#define ADC_MAGIC  'A'
#define CMD_ONE	1

#define ADC_CMD_GET _IOR(ADC_MAGIC, CMD_ONE, int)



#endif

