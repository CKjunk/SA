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
int numberOfZero = 0;  //����ĸ���
int overSampling = 1;  //������ϵ��
int timAdcPeriod = 100-1;   
int adcLowTime = 50;          //��ʱ��һ�������ڵ͵�ƽ����ʱ��
int sequencePeriod = 100 - 1;  //������ϵ��Ϊ1��ʱ���adc��ʱ������
int sequenceLowTime = 100;     //��ʱ��һ�������ڵ͵�ƽ����ʱ��
 
void initialSequence();
void beginSample(int number);
int framecount = 0;
extern  uint16_t* SA_IMS_ADC_ConvertedValue;// ADCת���ĵ�ѹֵͨ��DMA��ʽ����SRAM
int recSize = 0;  //���յ������С
int flag = 0;  //�ܵ�flag
volatile int frame_count = 0;  //֡������
int  sequenceLength = 511;
int  adcSequenceCount = 0; 
int  errorSample = 3;
int sequenceCount;
int main(void)
{	  
	  /*ѡ�����г���*/
	  sequenceLength = 511;
 	  SysTick_Init();
 	  EXTI_Key_Config(); //������ʼ��
	  Debug_USART_Config();//���ڳ�ʼ��
     SA_IMS_ADC_ConvertedValue = (uint16_t*)malloc((sequenceLength+errorSample) * (1+numberOfZero)*sizeof(uint16_t)); 
  	if(SA_IMS_ADC_ConvertedValue == NULL){
		   printf("SA_IMS_ADC_ConvertedValue allocate failure\r\n");
		}
 
    beginSample(sequenceLength+errorSample);
  
   // printf("over\r\n");
    while (1){
			
		 switch(flag){
			 case 1: //���ݽ��ձ�־λ
				 flag = 10;
			   
				 break;
			 case 2: //adc��ɱ�־λ
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

			 TIM1_GPIO_Config(sequencePeriod,SENIOR_PSC,0);//����ʱ�ӳ�ʼ��
			
  		 TIM8_ADC_Config(timAdcPeriod,SENIOR_PSC,adcLowTime);//adcʱ�ӳ�ʼ��
		}
		else{
			
			 TIM_ClearFlag(TIM_ADC1,TIM_FLAG_Update);
	     TIM_ClearITPendingBit(TIM_ADC1, TIM_IT_Update);    //��� TIM2 �����жϱ�־
			 TIM_ClearFlag(SENIOR_TIM,TIM_FLAG_Update);
       TIM_ClearITPendingBit(SENIOR_TIM, TIM_IT_Update);  //��� TIM2 �����жϱ�־
       
			 RHEOSTAT_ADC_DMA_STREAM->NDTR = number;            //����װ��DMA��Ҫ��������ݸ���
       DMA_Cmd(RHEOSTAT_ADC_DMA_STREAM, ENABLE);          //��ʹ��DMAͨ��
       RHEOSTAT_ADC->SR &=~(0X01<<5);                     //����Ӳ����1��ADC״̬�Ĵ����������־λ�������Ȼ���׳���
       ADC_DMACmd(RHEOSTAT_ADC, ENABLE);  
			
			 TIM_ITConfig(TIM_ADC1,TIM_IT_Update,ENABLE);    
			 TIM_ITConfig(SENIOR_TIM,TIM_IT_Update,ENABLE); 

		
		}
	  //printf("2.sequenceCount:%d,adcSequenceCount:%d\r\n",sequenceCount,adcSequenceCount);
    
 		 TIM_Cmd(SENIOR_TIM, ENABLE);//��ʼ�ɼ�
}
	 
 
 

 
 
 

 
 
 