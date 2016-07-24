#include "main.h"

//ADC123_IN2(PA2)

void Power_Detection_Configuration(void)
{
    ADC_InitTypeDef adc;
    GPIO_InitTypeDef gpio;
    //开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	// io 模拟输入
	gpio.GPIO_Pin = GPIO_Pin_2;
	gpio.GPIO_Mode = GPIO_Mode_AN;
	gpio.GPIO_OType = GPIO_OType_PP;
	gpio.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&gpio);
    
    adc.ADC_Resolution = ADC_Resolution_10b;
    adc.ADC_ScanConvMode = DISABLE;
    adc.ADC_ContinuousConvMode = ENABLE;//使能持续转换
    adc.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    adc.ADC_DataAlign = ADC_DataAlign_Right;//adc数据右对齐
    adc.ADC_NbrOfConversion = 1;//顺序进行规则转换adc通道的数目1
    ADC_Init(ADC1,&adc);//根据制定的参数初始化外设adc1
    
    ADC_Cmd(ADC1,ENABLE);//使能指定的adc1
    
    ADC_RegularChannelConfig(ADC1,ADC_Channel_2,1,ADC_SampleTime_56Cycles);
		//设置规则序列弘道以及采样周期的函数
}

//电源电压检测
void Power_Detect(void)
{
    unsigned int AD_Value;

    ADC_SoftwareStartConv(ADC1);//使能指定的adc1的软件转换启动功能
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET);

    AD_Value = ADC_GetConversionValue(ADC1);//获取adc的转换结果值
    if(AD_Value < 3000)
    {
        //....
    }
}
