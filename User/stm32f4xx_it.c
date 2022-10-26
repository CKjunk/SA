 #include "stm32f4xx_it.h"
#include "./usart/bsp_debug_usart.h"
#include "./time/bsp_time.h"
#include "./adc/bsp_adc.h"
#include "./key/key.h"
#include "./adc_dma/adc_dma.h"
#include "./usart_dma/usart_dma.h"
#include "./HadamardOutput/bsp_dma_gpio.h"
#include "./sequence/sequence.h"
#include "./systick/bsp_SysTick.h"
#include "stdlib.h"
#include "math.h"
extern void beginSample(int number);
extern int recSize;
extern  int flag;
volatile int* createInitialCoef(char Usart_Rx_Buf[],int* orderLength);
extern void TimingDelay_Decrement(void);
extern  u8* sendSequence;
extern int numberOfZero;
extern int* porderLength;
extern int* psequenceLength;
extern int  sequenceLength;
extern  int sequenceCount;
extern  int adcSequenceCount;
extern int buffsize;
extern volatile int  frame_count;
extern  int  orderLength;
extern volatile uint16_t* HT_IMS_ADC_ConvertedValue;
extern int errorSample;
u8 MODIFY_TIM8_PULSE = 1;
void TOTAL_PROJECT_INT_FUNCTION(void){
	if(EXTI_GetITStatus(KEY_INT_EXTI_LINE)!= RESET){
	  beginSample((sequenceLength+errorSample)*(numberOfZero+1));
		EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位
	}
}
void DMA_ADC_COMP_INT_FUNCTION(void){
    if(DMA_GetITStatus(RHEOSTAT_ADC_DMA_STREAM,DMA_IT_TCIF0))
	  {
		 
 			flag = 2;
 			DMA_Cmd(RHEOSTAT_ADC_DMA_STREAM, DISABLE);          //先使能DMA通道

      DMA_ClearITPendingBit(RHEOSTAT_ADC_DMA_STREAM,DMA_FLAG_TCIF0);		   			 
	  }
}

 
void SENIOR_TIM_INT_FUNCTION(void){
 	if(TIM_GetITStatus(SENIOR_TIM, TIM_IT_Update)){	
    if(sequenceCount == 0){
		  SENIOR_TIM->CCR1  = 0;
		}
    
    
	 else{
			SENIOR_TIM->CCR1  =  SEQUENCE_PERIOD+1;	 
	 }  	
 
	
		if(sequenceCount == sequenceLength+1){
		   SENIOR_TIM->CR1 &= (uint16_t)~TIM_CR1_CEN;//序列的时钟disable

		   sequenceCount = -1;
		
		}
		sequenceCount++; 

		}	 
		
		
		TIM_ClearITPendingBit(SENIOR_TIM, TIM_IT_Update); //清除 TIM2 更新中断标志
     
   }
 
void TIM_ADC1_IRQHandler(){
  if(TIM_GetITStatus(TIM_ADC1, TIM_IT_Update)){	
    
    
     
		    adcSequenceCount++; 
		if(adcSequenceCount == sequenceLength+errorSample+1){
		   TIM_ADC1->CR1 &= (uint16_t)~TIM_CR1_CEN; //adc的时钟disable
  			adcSequenceCount = 0;
		}

		}	 
		
		
		TIM_ClearITPendingBit(TIM_ADC1, TIM_IT_Update); //清除 TIM2 更新中断标志
 


}
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
}

 