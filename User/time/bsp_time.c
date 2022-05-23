#include "./time/bsp_time.h"
static void SENIOR_TIMx_GPIO_Config(void)
{
    /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*������ʱ����ص�GPIO����ʱ��*/
    RCC_AHB1PeriphClockCmd (ADVANCE_OCPWM1_GPIO_CLK  |
                            ADVANCE_OCNPWM1_GPIO_CLK ,
														ENABLE);
    /* ָ�����Ÿ��ù��� */
    GPIO_PinAFConfig(ADVANCE_OCPWM1_GPIO_PORT,
                    ADVANCE_OCPWM1_PINSOURCE,
                    ADVANCE_OCPWM1_AF);
    GPIO_PinAFConfig(ADVANCE_OCNPWM1_GPIO_PORT,
                    ADVANCE_OCNPWM1_PINSOURCE,
                    ADVANCE_OCNPWM1_AF);
    GPIO_PinAFConfig(ADVANCE_OCPWM8_GPIO_PORT,
                    ADVANCE_OCPWM8_PINSOURCE,
                    ADVANCE_OCPWM8_AF);
    GPIO_PinAFConfig(ADVANCE_OCNPWM8_GPIO_PORT,
                    ADVANCE_OCNPWM8_PINSOURCE,
                    ADVANCE_OCNPWM8_AF);

    /* ��ʱ���������ų�ʼ�� */
    GPIO_InitStructure.GPIO_Pin = ADVANCE_OCPWM1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(ADVANCE_OCPWM1_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = ADVANCE_OCNPWM1_PIN;
    GPIO_Init(ADVANCE_OCNPWM1_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = ADVANCE_OCPWM8_PIN;
    GPIO_Init(ADVANCE_OCPWM8_GPIO_PORT, &GPIO_InitStructure);
		
    GPIO_InitStructure.GPIO_Pin = ADVANCE_OCNPWM8_PIN;
    GPIO_Init(ADVANCE_OCNPWM8_GPIO_PORT, &GPIO_InitStructure);
}

static void SENIOR_TIMx_Mode_Config(u16 Arr,u16 Psc,u16 Pwm1_lowTime,u16 Pwm2_lowTime)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
	  //������ʼ���ṹ��
    TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
    /* ================== ʱ���ṹ���ʼ��=================== */
    // ����TIMx_CLK,x[1,8]
    RCC_APB2PeriphClockCmd(ADVANCE_TIM1_CLK|ADVANCE_TIM8_CLK, ENABLE);

    // �ۼ� TIM_Period+1�����������һ�����»����ж�
    // ����ʱ����0������1023����Ϊ1024�Σ�Ϊһ����ʱ����
    TIM_TimeBaseStructure.TIM_Period = Arr;
    // �߼����ƶ�ʱ��ʱ��ԴTIMxCLK = HCLK=168MHz
    // �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=100KHz
    TIM_TimeBaseStructure.TIM_Prescaler = Psc;
    // ʱ�ӷ�Ƶ���ڼ�������ʱ���ʱ����õ�
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    // ������ʽ
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    // �ظ�������������ûʹ��
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
    // ��ʼ����ʱ��TIMx, x[1,8]
    TIM_TimeBaseInit(ADVANCE_TIM1, &TIM_TimeBaseStructure);
    TIM_TimeBaseInit(ADVANCE_TIM8, &TIM_TimeBaseStructure);
    /* ================== ����ṹ���ʼ��=================== */
    // ����ΪPWMģʽ2��������͵�ƽ���ﵽ�Ƚ�ֵ��ʱ���ٸı��ƽ
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    // �����ʹ��
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    // �������ʹ��
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    // ���ñȽ�ֵ
    TIM_OCInitStructure.TIM_Pulse = Pwm1_lowTime;
    // ������ߵ�ƽ��Ч
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    // ��������ߵ�ƽ��Ч
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    // ������ڱ���ֹʱΪ�͵�ƽ
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCNIdleState_Reset;
    // ��������ڱ���ֹʱΪ�ߵ�ƽ
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Set;
    // ͨ����ʼ��
    TIM_OC1Init(ADVANCE_TIM1, &TIM_OCInitStructure);

    // ʹ��ͨ����װ��
    TIM_OC1PreloadConfig(ADVANCE_TIM1, TIM_OCPreload_Enable);
		
    TIM_OCInitStructure.TIM_Pulse = Pwm2_lowTime;
		
		TIM_OC1PreloadConfig(ADVANCE_TIM8, TIM_OCPreload_Enable);
		
    /* ================ ��·�������ṹ���ʼ��================ 
    // �Զ����ʹ�ܣ���·������ʱ�����������
    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
    // ��������ʱ��
    TIM_BDTRInitStructure.TIM_DeadTime = 11;
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
    // ����ɲ�����ŵ�ƽ��������Ϊ���õĵ�ƽʱ�����ͻ������������ֹ
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
    TIM_BDTRConfig(ADVANCE_TIM, &TIM_BDTRInitStructure);
*/
    // ʹ�ܶ�ʱ��
    TIM_Cmd(ADVANCE_TIM1, ENABLE);

    // �������ʹ�ܣ�ֻ��TIM1��TIM8�����������
    TIM_CtrlPWMOutputs(ADVANCE_TIM1, ENABLE);
		TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);//����ģʽMSM
		TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Enable);//Tim1ʹ�������Ϊ�����ź�        
		TIM_SelectMasterSlaveMode(TIM8, TIM_MasterSlaveMode_Enable);//����ģʽMSM
		TIM_SelectInputTrigger(TIM8, TIM_TS_ITR0);//����ѡ��,ITR0��TIM1
		TIM_SelectSlaveMode(TIM8, TIM_SlaveMode_Trigger);//����ģʽֻ����;��ģʽ��ͣ������
	 
		
    TIM_CtrlPWMOutputs(TIM8, ENABLE);///////�����ʹ��
}

static void COMMON_TIMx_Config(u16 arr,u16 psc){

  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	
  TIM_SelectMasterSlaveMode(TIM8, TIM_MasterSlaveMode_Enable);//����ģʽMSM
	TIM_SelectOutputTrigger(TIM8, TIM_TRGOSource_Enable);//Tim8ʹ�������Ϊ�����ź�        
	TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);//����ģʽMSM
	TIM_SelectInputTrigger(TIM2, TIM_TS_ITR1);//����ѡ��,ITR0��TIM1
	TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Trigger);//����ģʽֻ����;��ģʽ��ͣ������
 
		
	TIM_CtrlPWMOutputs(TIM2, ENABLE);///////�����ʹ��


}