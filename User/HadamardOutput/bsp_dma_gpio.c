#include "./HadamardOutput/bsp_dma_gpio.h"

u8 array[15]={1 ,1 ,0 ,0 ,0 ,1, 0 ,0, 1 ,1 ,0 ,1 ,0, 1 ,1 };
u8 count = 0;
void HadamardDmaGPIO(void){
	
	   GPIO_InitTypeDef GPIO_InitStructure;
	   
     DMA_InitTypeDef DMA_InitStructure;
     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOK,ENABLE);
	   		/*ѡ��Ҫ���Ƶ�GPIO����*/															   
		 GPIO_InitStructure.GPIO_Pin = HADAMARD_GPIO_PIN;	

		/*��������ģʽΪ���ģʽ*/
		 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
    
    /*�������ŵ��������Ϊ�������*/
     GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    
    /*��������Ϊ����ģʽ*/
     GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

		/*������������Ϊ2MHz */   
		 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 

		/*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
		 GPIO_Init(HADAMARD_GPIO_PORT, &GPIO_InitStructure);	
	
 		 DMA_DeInit(HADAMARD_GPIO_DMA_STREAM);
		 while (DMA_GetCmdStatus(HADAMARD_GPIO_DMA_STREAM) != DISABLE){}                                     // �ȴ�DMA������

		 DMA_InitStructure.DMA_Channel = HADAMARD_GPIO_INT_CHANNEL;                                              // ͨ��ѡ��
		 DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&HADAMARD_GPIO_PORT->ODR; // DMA�����ַ(SOURCE ADDR)
		 DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(array);                      // DMA �洢��0��ַ
		 DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                             // �ڴ浽����
		 DMA_InitStructure.DMA_BufferSize = HADAMARD_BUFFSIZE;                                                                   // ���ݴ�����
		 DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                   // ��������ģʽ
		 DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                          // �洢��������ģʽ
		 DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;  // �������ݳ���:16λ
		 DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;        // �洢�����ݳ���:16λ
		 DMA_InitStructure.DMA_Mode = DISABLE; // ����ʹ��ѭ��ģʽ�������޷�ʵ��ѭ����������
		 DMA_InitStructure.DMA_Priority = DMA_Priority_High;                                          // ���ȼ�
		 DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;

		 DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
		 DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; // �洢��ͻ�����δ���
		 DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; // ����ͻ�����δ���
		 DMA_Init(HADAMARD_GPIO_DMA_STREAM, &DMA_InitStructure);

     DMA_ITConfig(HADAMARD_GPIO_DMA_STREAM,DMA_IT_TC, ENABLE);		//ʹ�ܴ�������ж�
     DMA_ClearFlag(HADAMARD_GPIO_DMA_STREAM,DMA_FLAG_TCIF0); /* ������״̬��ʶ */ 
	   DMA_ClearFlag(HADAMARD_GPIO_DMA_STREAM,DMA_FLAG_HTIF0);
	   // ʹ��DMA��
     DMA_Cmd(HADAMARD_GPIO_DMA_STREAM, ENABLE);
}