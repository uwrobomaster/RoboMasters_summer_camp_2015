#include "system.h"

static volatile uint32_t usTicks = 0;
uint32_t currentTime = 0;
uint32_t previousTime = 0;
uint16_t cycleTime = 0; 

PID_t SpeedPID;

//�޷�
int constrain(int amt, int low, int high)
{
	if (amt < low)
			return low;
	else if (amt > high)
			return high;
	else
			return amt;
}

void Parameter_Init(void)
{
	SpeedPID.P = 10;
	SpeedPID.I = 14;
	SpeedPID.D = 2;
}

//��������ʼ��
static void cycleCounterInit(void)
{
	RCC_ClocksTypeDef clocks;
	RCC_GetClocksFreq(&clocks);
	usTicks = clocks.SYSCLK_Frequency / 1000000; 
}

//��΢��Ϊ��λ����ϵͳʱ��
uint32_t micros(void)
{
	register uint32_t ms, cycle_cnt;
	do {
			ms = sysTickUptime;
			cycle_cnt = SysTick->VAL;
	} while (ms != sysTickUptime);
	return (ms * 1000) + (usTicks * 1000 - cycle_cnt) / usTicks;
}

//΢�뼶��ʱ
void delayMicroseconds(uint32_t us)
{
	uint32_t now = micros();
	while (micros() - now < us);
}

//���뼶��ʱ
void delay(uint32_t ms)
{
	while (ms--)
			delayMicroseconds(1000);
}

//�Ժ���Ϊ��λ����ϵͳʱ��
uint32_t millis(void)
{
	return sysTickUptime;
}
void RCC_Configuration(void)
{
	RCC_DeInit();	//ʱ�ӻ�ԭΪȱʡֵ
	RCC_HSEConfig(RCC_HSE_ON);//ʹ���ⲿ����ʱ��
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY)==RESET);//�ȴ��ⲿ����ʱ�Ӿ���
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	FLASH_SetLatency(FLASH_Latency_2);

	RCC_HCLKConfig(RCC_SYSCLK_Div1); //HCLK����Ϊ����SYSCLOCKϵͳʱ��
	RCC_PCLK2Config(RCC_HCLK_Div1);	 //PCLK2����Ϊ����HCLK
	RCC_PCLK1Config(RCC_HCLK_Div2);	 //PCLK1����Ϊ����HCLK/2
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);//��1��ͨ·��PLL *9
	RCC_PLLCmd(ENABLE); 
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);//�ȴ�PLL����
	
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//��PLL��Ϊϵͳʱ��
	while(RCC_GetSYSCLKSource()!=0x08);//�ȴ�����
}
void systemInit(void)
{
	RCC_Configuration();
	cycleCounterInit();
	SysTick_Config(SystemCoreClock / 1000);	//�δ�ʱ�����ã�1ms
	PWM_Configuration();
	CAN_Configuration();
	Encoder_Configuration();
	ADC_Configuration();
	LED_Configuration();
	KEY_Configuration();
	Parameter_Init();
	USART2_Configuration();
	Encoder_Start();
}

void Loop(void)
{	
	static uint32_t currentTime = 0;
	static uint32_t loopTime = 0;
	
	currentTime = micros();	//��ȡ��ǰϵͳʱ��
	
	if ((int32_t)(currentTime - loopTime) >= 0)  
	{			
    loopTime = currentTime + 1000;	//1ms
		Motor_Control();		
	}
	else
	{
		
	}
}
