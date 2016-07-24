/********************************************************************************
 * �ļ���  ��main.c
 * ����    ��ʱ��TIM1���4·PWM��������·����һ�����ٵ��ʵ������ת��һ·����һ�����ٵ��ʵ����ת��һ·����һ�����ʵ�ֶ��Ƕ�ת��  
 *           - PA.08: (TIM1_CH1)
 *           - PA.09: (TIM1_CH2)
 *           - PA.10: (TIM1_CH3)
 *           - PA.11: (TIM1_CH4) 
 *           TIM1 Channel1 duty cycle = (TIM1_CCR1/ TIM1_ARR)* 100 = 50%
 *           TIM1 Channel2 duty cycle = (TIM1_CCR2/ TIM1_ARR)* 100 = 37.5%
 *           TIM1 Channel3 duty cycle = (TIM1_CCR3/ TIM1_ARR)* 100 = 25%
 *           TIM1 Channel4 duty cycle = (TIM1_CCR4/ TIM1_ARR)* 100 = 12.5%
 *       
 * ��汾  ��ST3.5.0
**********************************************************************************/
#include "stm32f10x.h"
#include "pwm_output.h"
#include "stm32f10x_tim.h"
#include "exti.h"

#include "stm32f10x_exti.h"
#include "misc.h"
#include "pwm_output.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"


// #include "stm32f10x_gpio.h"
// #include "stm32f10x_rcc.h"
TIM_OCInitTypeDef  TIM_OCInitStructure;	
/*
 * ��������main
 * ����  ��������
 * ����  ����
 * ���  ����
 */


extern void TIM1_PWM_Init(void);
extern void USART2_Configuration(void);


int main(void)
{
	
	bool ReadValue;
  /* ����ϵͳʱ��Ϊ72M */
	SystemInit();
 
	/* TIM1 PWM�������ʼ������ʹ��TIM1 PWM��� */
  //�ɻ���ɣ��ɵ��������������򣬻ص�ץ�������ȴ�PC����ָ��������ٵ����ʼת��������3·pwm
	
	
//	TIM1_PWM_Init();                 //��·����ʼ���pwm
 TIM1_PWM_Init();
	
	/*PB12 PB13�ֱ����������г̿��أ�
	�ȹ���λ�õ���װ����ӱ�Եʱ
	����һ���г̿��ط�����ײ�����жϾͽ��������ٵ��ͣ��
	*/
	
	EXTI_PB12_Config();
 EXTI_PB13_Config();	
	
	USART2_Configuration();
	
	key_init();
	
  while (1)
  {
	//	ReadValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12);
		
		//USART_ReceiveData(USART2);
	//	USART_SendData(USART2, ReadValue);
}
//		return 0;
}


/**********************************END OF FILE*************************************/



/********************************************************************************
 * �ļ���  ��main.c
 * ����    ��ʱ��TIM1���4·PWM��������·����һ�����ٵ��ʵ������ת��һ·����һ�����ٵ��ʵ����ת��һ·����һ�����ʵ�ֶ��Ƕ�ת��  
 *           - PA.08: (TIM1_CH1)
 *           - PA.09: (TIM1_CH2)
 *           - PA.10: (TIM1_CH3)
 *           - PA.11: (TIM1_CH4) 
 *           TIM1 Channel1 duty cycle = (TIM1_CCR1/ TIM1_ARR)* 100 = 50%
 *           TIM1 Channel2 duty cycle = (TIM1_CCR2/ TIM1_ARR)* 100 = 37.5%
 *           TIM1 Channel3 duty cycle = (TIM1_CCR3/ TIM1_ARR)* 100 = 25%
 *           TIM1 Channel4 duty cycle = (TIM1_CCR4/ TIM1_ARR)* 100 = 12.5%
 *       
 * ��汾  ��ST3.5.0
**********************************************************************************/

