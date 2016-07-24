#include "system.h"

static volatile uint32_t usTicks = 0;
uint32_t currentTime = 0;
uint32_t previousTime = 0;
uint16_t cycleTime = 0; 

PID_t SpeedPID;

//限幅
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

//计数器初始化
static void cycleCounterInit(void)
{
	RCC_ClocksTypeDef clocks;
	RCC_GetClocksFreq(&clocks);
	usTicks = clocks.SYSCLK_Frequency / 1000000; 
}

//以微秒为单位返回系统时间
uint32_t micros(void)
{
	register uint32_t ms, cycle_cnt;
	do {
			ms = sysTickUptime;
			cycle_cnt = SysTick->VAL;
	} while (ms != sysTickUptime);
	return (ms * 1000) + (usTicks * 1000 - cycle_cnt) / usTicks;
}

//微秒级延时
void delayMicroseconds(uint32_t us)
{
	uint32_t now = micros();
	while (micros() - now < us);
}

//毫秒级延时
void delay(uint32_t ms)
{
	while (ms--)
			delayMicroseconds(1000);
}

//以毫秒为单位返回系统时间
uint32_t millis(void)
{
	return sysTickUptime;
}
void RCC_Configuration(void)
{
	RCC_DeInit();	//时钟还原为缺省值
	RCC_HSEConfig(RCC_HSE_ON);//使能外部高速时钟
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY)==RESET);//等待外部高速时钟就绪
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	FLASH_SetLatency(FLASH_Latency_2);

	RCC_HCLKConfig(RCC_SYSCLK_Div1); //HCLK设置为等于SYSCLOCK系统时钟
	RCC_PCLK2Config(RCC_HCLK_Div1);	 //PCLK2设置为等于HCLK
	RCC_PCLK1Config(RCC_HCLK_Div2);	 //PCLK1设置为等于HCLK/2
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);//除1的通路，PLL *9
	RCC_PLLCmd(ENABLE); 
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);//等待PLL就绪
	
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//将PLL设为系统时钟
	while(RCC_GetSYSCLKSource()!=0x08);//等待就绪
}
void systemInit(void)
{
	RCC_Configuration();
	cycleCounterInit();
	SysTick_Config(SystemCoreClock / 1000);	//滴答定时器配置，1ms
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
	
	currentTime = micros();	//获取当前系统时间
	
	if ((int32_t)(currentTime - loopTime) >= 0)  
	{			
    loopTime = currentTime + 1000;	//1ms
		Motor_Control();		
	}
	else
	{
		
	}
}
