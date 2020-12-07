#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "dev.h"

//ledNum：操作的是哪颗灯    cmd:操作的命令
int ledProcess(int ledNum,int cmd)
{

        if (ledNum < 0 || ledNum > 3 || cmd > 2 || cmd < 1)
        {
                return -1;
        }
    //具体操作
        if(ledNum == 0 && cmd == 1)
        {
                ioctl(ledFd, LED_ON, 0);
        }
        else if(ledNum == 0 && cmd == 2)
        {
            ioctl(ledFd, LED_OFF, 0);
        }

        if(ledNum == 1 && cmd == 1)
        {
                ioctl(ledFd, LED_ON, 1);
        }
        else if(ledNum == 1 && cmd == 2)
        {
            ioctl(ledFd, LED_OFF, 1);
        }

        if(ledNum == 2 && cmd == 1)
        {
                ioctl(ledFd, LED_ON, 2);
        }
        else if(ledNum == 2 && cmd == 2)
        {
            ioctl(ledFd, LED_OFF, 2);
        }

        if(ledNum == 3 && cmd == 1)
        {
                ioctl(ledFd, LED_ON, 3);
        }
        else if(ledNum == 3 && cmd == 2)
        {
            ioctl(ledFd, LED_OFF, 3);
        }

        return 0;
}

