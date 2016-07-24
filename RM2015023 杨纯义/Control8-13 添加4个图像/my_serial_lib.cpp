#include "my_serial_lib.h"
#include "DJI_Pro_Codec.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<sys/ioctl.h>

#include <termios.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

static int serial_2_fd = -1;//串口2文件描述符
static fd_set serial_2_fd_set;

/*
用法：#include  <pthread.h>
原型：void  pthread_exit（void  *retval）
retval是线程的返回码；
*/




/*
功能：	打开串口设备文件
输入：例：   "/dev/ttyUSB0"
返回：成功返回0 
*/
int Serial_2_Open(const char *port_str)
{
    serial_2_fd = open(port_str, O_RDWR | O_NOCTTY);  //block mode
    if(serial_2_fd < 0)
    {
        printf("%s,%d:ERROR\n",__func__,__LINE__);
        return -1;
    }
    return 0;
}

/*
功能：	设置串口2的波特率并开启串口2接收线程
输入：设备名：例： "/dev/ttyUSB0"  ， 波特率 ：例：115200
返回：成功返回0 
*/
int serial_2_setup(const char *device,int baudrate)
{
    printf("setting serial_2\n");
    if(Serial_2_Start(device,baudrate) < 0)
    {
        Serial_2_Close();
        return -1;
    }

    if(Serial_2_StartThread() < 0)
    {
        return -1;
    }

    return 0;
}


/*
功能：	设置串口2的波特率
输入：设备名：例： "/dev/ttyUSB0"  ， 波特率 ：例：115200
返回：成功返回0 
*/


 int Serial_2_Start(const char *dev_name,int baud_rate)
{
    const char *ptemp;
    if(dev_name == NULL)
    {
        ptemp = "/dev/ttyUSB0";
    }
    else
    {
        ptemp = dev_name;
    }
    if(Serial_2_Open(ptemp) == 0
            && Serial_2_Config(baud_rate,8,'N',1) == 0)
    {
        FD_ZERO(&serial_2_fd_set);
        FD_SET(serial_2_fd, &serial_2_fd_set);
        printf("succeed to open serial_2 and config serial_2\n");
        return serial_2_fd;
    }
    return -1;
}


//关闭串口2
int Serial_2_Close()
{
    close(serial_2_fd);
    serial_2_fd = -1;
    return 0;
}


/*
功能：	配置串口2
输入：例：115200，8,'N',1
返回：成功返回0 
*/

int Serial_2_Config(int baudrate,char data_bits,char parity_bits,char stop_bits)
{
    int st_baud[]=
    {
        B4800,
        B9600,
        B19200,
        B38400,
        B57600,
        B115200,
        B230400,
        B1000000,
        B1152000,
        B3000000,
    };
    int std_rate[]=
    {
        4800,
        9600,
        19200,
        38400,
        57600,
        115200,
        230400,
        1000000,
        1152000,
        3000000,
    };

    int i,j;
    struct termios newtio2, oldtio2;
    /* save current port parameter */
    if (tcgetattr(serial_2_fd, &oldtio2) != 0)
    {
        printf("%s,%d:ERROR\n",__func__,__LINE__);
        return -1;
    }
    bzero(&newtio2, sizeof(newtio2));

    /* config the size of char */
    newtio2.c_cflag |= CLOCAL | CREAD;
    newtio2.c_cflag &= ~CSIZE;

    /* config data bit */
    switch (data_bits)
    {
    case 7:
        newtio2.c_cflag |= CS7;
        break;
    case 8:
        newtio2.c_cflag |= CS8;
        break;
    }
    /* config the parity bit */
    switch (parity_bits)
    {
        /* odd */
    case 'O':
    case 'o':
        newtio2.c_cflag |= PARENB;
        newtio2.c_cflag |= PARODD;
        break;
        /* even */
    case 'E':
    case 'e':
        newtio2.c_cflag |= PARENB;
        newtio2.c_cflag &= ~PARODD;
        break;
        /* none */
    case 'N':
    case 'n':
        newtio2.c_cflag &= ~PARENB;
        break;
    }
    /* config baudrate */
    j = sizeof(std_rate)/4;
    for(i = 0;i < j;i ++)
    {
        if(std_rate[i] == baudrate)
        {
            /* set standard baudrate */
            cfsetispeed(&newtio2, st_baud[i]);
            cfsetospeed(&newtio2, st_baud[i]);
            break;
        }
    }
    /* config stop bit */
    if( stop_bits == 1 )
       newtio2.c_cflag &=  ~CSTOPB;
    else if ( stop_bits == 2 )
       newtio2.c_cflag |=  CSTOPB;

    /* config waiting time & min number of char */
    newtio2.c_cc[VTIME]  = 1;
    newtio2.c_cc[VMIN] = 1;

    /* using the raw data mode */
    newtio2.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);
    newtio2.c_oflag  &= ~OPOST;

    /* flush the hardware fifo */
    tcflush(serial_2_fd,TCIFLUSH);

    /* activite the configuration */
    if((tcsetattr(serial_2_fd,TCSANOW,&newtio2))!=0)
    {
        printf("%s,%d:ERROR\n",__func__,__LINE__);
        return -1;
    }
    return 0;
}



//串口2接收线程
static void * Serial_2_RecvThread(void * arg)
{
    int ret;
    unsigned int depth,len;
    unsigned char buf[64];
    int i;
    while(1)
    {
       // printf("5467\n");
        ret = select(FD_SETSIZE, &serial_2_fd_set, (fd_set *)0, (fd_set *)0,(struct timeval *) 0);
     //   printf("123\n");
        if (ret < 1)
        {
            printf("%s,%d,ERROR\n", __func__, __LINE__);
            FD_ZERO(&serial_2_fd_set);
            FD_SET(serial_2_fd, &serial_2_fd_set);
            continue;
        }

        ioctl(serial_2_fd, FIONREAD, &depth);
        if(depth > 0)
        {
            //TODO...Call protocol decode function
            len = depth > sizeof(buf) ? sizeof(buf) : depth;
            ret = read(serial_2_fd,buf,len);
            for(i = 0; i < ret; i ++)
            {
                sdk_serial_byte_handle(buf[i]);
            }
            buf[i]='\0';
            printf("buf2=%s\n",buf);
        }
    }
    return NULL;
}


int Serial_2_StartThread(void)
{
    int ret;
    pthread_t A_ARR;
    ret = pthread_create(&A_ARR, 0,Serial_2_RecvThread,NULL);
    if(ret != 0)
    {
        return -1;
    }
    return 0;
}


//串口2 写函数，输入buf指针，和字符发送字符长度，没开线程
int Serial_2_Write(unsigned char *buf,int len)
{
    return write(serial_2_fd,buf,len);
}


//串口2 读函数 不是用线程读的
int Serial_2_Read(unsigned char *buf,int len)
{
    int saved = 0;
    int ret = -1;

    if( buf == NULL)
        return -1;
    else
    {
        for(; saved < len ;)
        {
            ret = read(serial_2_fd,buf + saved,len - saved);
            if(ret > 0)
            {
                saved += ret;
            }
            else
            {
                break;
            }
        }
        return saved;
    }
}

