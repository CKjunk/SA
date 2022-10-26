 #include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./adc/bsp_adc.h"
#include "./sequence/sequence.h"
#include "./time/bsp_time.h"
#include "./key/key.h"
#include "./led/bsp_led.h"  
#include "./usart_dma/usart_dma.h"
#include "./HadamardOutput/bsp_dma_gpio.h"
#include "./systick/bsp_SysTick.h"
#include "stdlib.h"
#include "math.h"
 

#define arrayIndex 0
int octalArray[12]={23,45,103,203,435,1021,2011,4005,10123,20033};
int coefSize[12]={4,5,6,7,8,9,10,11,12,13,14,15};
int period[12] = {16,31,63,127,255,511,1023,2047,4095,8191};
int numberOfZero = 0;  //补零的个数
int overSampling = 1;  //过采样系数
int timAdcPeriod = 100-1;   
int adcLowTime = 50;          //定时器一个周期内低电平持续时间
int sequencePeriod = 100 - 1;  //过采样系数为1的时候的adc定时器周期
int sequenceLowTime = 100;     //定时器一个周期内低电平持续时间
 
void initialSequence();
void beginSample(int number);
int framecount = 0;
extern  uint16_t* SA_IMS_ADC_ConvertedValue;// ADC转换的电压值通过DMA方式传到SRAM
int recSize = 0;  //接收的命令大小
int flag = 0;  //总的flag
volatile int frame_count = 0;  //帧数计数
int  sequenceLength = 511;
int  adcSequenceCount = 0; 
int  errorSample = 3;
int sequenceCount;
int main(void)
{	  
	  /*选择序列长度*/
	  sequenceLength = 511;
 	  SysTick_Init();
 	  EXTI_Key_Config(); //按键初始化
	  Debug_USART_Config();//串口初始化
     SA_IMS_ADC_ConvertedValue = (uint16_t*)malloc((sequenceLength+errorSample) * (1+numberOfZero)*sizeof(uint16_t)); 
  	if(SA_IMS_ADC_ConvertedValue == NULL){
		   printf("SA_IMS_ADC_ConvertedValue allocate failure\r\n");
		}
 
    beginSample(sequenceLength+errorSample);
  
   // printf("over\r\n");
    while (1){
			
		 switch(flag){
			 case 1: //数据接收标志位
				 flag = 10;
			   
				 break;
			 case 2: //adc完成标志位
			//	 printf("adc complete\r\n");
				 flag = 10;
			   framecount++;
			 if(framecount>1){
				 //  printf("SA_IMS_ADC_ConvertedValue\r\n");
				   for(int i = errorSample;i < sequenceLength+errorSample;i++){
				    printf("%d\r\n",SA_IMS_ADC_ConvertedValue[i]);
				   }
				 
			 
			}
  
 		}
  }

}
 
void  beginSample(int number){
	   //printf("1.sequenceCount:%d,adcSequenceCount:%d\r\n",sequenceCount,adcSequenceCount);

     sequenceCount = 0;
	   adcSequenceCount = 0;
		if(framecount == 0){	
			 ADCX_Init(number);

			 TIM1_GPIO_Config(sequencePeriod,SENIOR_PSC,0);//序列时钟初始化
			
  		 TIM8_ADC_Config(timAdcPeriod,SENIOR_PSC,adcLowTime);//adc时钟初始化
		}
		else{
			
			 TIM_ClearFlag(TIM_ADC1,TIM_FLAG_Update);
	     TIM_ClearITPendingBit(TIM_ADC1, TIM_IT_Update);    //清除 TIM2 更新中断标志
			 TIM_ClearFlag(SENIOR_TIM,TIM_FLAG_Update);
       TIM_ClearITPendingBit(SENIOR_TIM, TIM_IT_Update);  //清除 TIM2 更新中断标志
       
			 RHEOSTAT_ADC_DMA_STREAM->NDTR = number;            //重新装填DMA需要传输的数据个数
       DMA_Cmd(RHEOSTAT_ADC_DMA_STREAM, ENABLE);          //先使能DMA通道
       RHEOSTAT_ADC->SR &=~(0X01<<5);                     //将由硬件置1的ADC状态寄存器的溢出标志位清除，不然容易出错
       ADC_DMACmd(RHEOSTAT_ADC, ENABLE);  
			
			 TIM_ITConfig(TIM_ADC1,TIM_IT_Update,ENABLE);    
			 TIM_ITConfig(SENIOR_TIM,TIM_IT_Update,ENABLE); 

		
		}
	  //printf("2.sequenceCount:%d,adcSequenceCount:%d\r\n",sequenceCount,adcSequenceCount);
    
 		 TIM_Cmd(SENIOR_TIM, ENABLE);//开始采集
}
	 
 
 

 
 
 

 
 
 