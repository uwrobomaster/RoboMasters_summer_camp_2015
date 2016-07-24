#ifndef MY_SERIAL_LIB_H
#define MY_SERIAL_LIB_H


extern int Serial_2_Config(int baudrate,char data_bits,char parity_bits,char stop_bits);
extern int Serial_2_Open(const char *port_str);
extern int Serial_2_Start(const char *dev_name,int baud_rate);
extern int Serial_2_Close();
extern int serial_2_setup(const char *device,int baudrate);
extern int Serial_2_Write(unsigned char *buf,int len);
extern int Serial_2_Read(unsigned char *buf,int len);
extern int Serial_2_StartThread(void);
//extern void serial_alarm(int a);
extern int Serial_timerThread(void);
extern void my_delay(int t);


























#endif // MY_SERIAL_LIB_H
