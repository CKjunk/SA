#ifndef BSP_TIME_H
#define	BSP_TIME_H

#include "stm32f4xx.h"
#include <stdio.h>
 /* ��ʱ�� */
#define ADVANCE_TIM1                   TIM1
#define ADVANCE_TIM1_CLK               RCC_APB2Periph_TIM1
#define ADVANCE_TIM8                   TIM8
#define ADVANCE_TIM8_CLK               RCC_APB2Periph_TIM8
#define ADVANCE_TIM2                   TIM2
#define ADVANCE_TIM2_CLK               RCC_APB1Periph_TIM2
/* TIM1ͨ��1������� */
#define ADVANCE_OCPWM1_PIN             GPIO_Pin_8
#define ADVANCE_OCPWM1_GPIO_PORT       GPIOA
#define ADVANCE_OCPWM1_GPIO_CLK        RCC_AHB1Periph_GPIOA
#define ADVANCE_OCPWM1_PINSOURCE       GPIO_PinSource8
#define ADVANCE_OCPWM1_AF              GPIO_AF_TIM1

/* TIM1ͨ��1����������� */
#define ADVANCE_OCNPWM1_PIN            GPIO_Pin_7
#define ADVANCE_OCNPWM1_GPIO_PORT      GPIOA
#define ADVANCE_OCNPWM1_GPIO_CLK       RCC_AHB1Periph_GPIOA
#define ADVANCE_OCNPWM1_PINSOURCE      GPIO_PinSource7
#define ADVANCE_OCNPWM1_AF             GPIO_AF_TIM1

/* TIM8ͨ��1������� */
#define ADVANCE_OCPWM8_PIN             GPIO_Pin_6
#define ADVANCE_OCPWM8_GPIO_PORT       GPIOC
#define ADVANCE_OCPWM8_GPIO_CLK        RCC_AHB1Periph_GPIOC
#define ADVANCE_OCPWM8_PINSOURCE       GPIO_PinSource6
#define ADVANCE_OCPWM8_AF              GPIO_AF_TIM8

/* TIM8ͨ��1����������� */
#define ADVANCE_OCNPWM8_PIN            GPIO_Pin_5
#define ADVANCE_OCNPWM8_GPIO_PORT      GPIOA
#define ADVANCE_OCNPWM8_GPIO_CLK       RCC_AHB1Periph_GPIOA
#define ADVANCE_OCNPWM8_PINSOURCE      GPIO_PinSource5
#define ADVANCE_OCNPWM8_AF             GPIO_AF_TIM8


/* TIM2ͨ��1������� */

/*ͨ�ö�ʱ��*/
/* ͨ�ö�ʱ�� */
#define GENERAL_TIM                   TIM2
#define GENERAL_TIM_CLK               RCC_APB1Periph_TIM2
 
#define COMMON_TIM_IRQn                TIM2_IRQHandler


/*������ʱ��*/
#define BASIC_TIM               TIM6
#define BASIC_TIM_CLK           RCC_APB1Periph_TIM6

#define BASIC_TIM_IRQn          TIM6_DAC_IRQn
#define BASIC_TIM_IRQHandler    TIM6_DAC_IRQHandler

static void SENIOR_TIMx_GPIO_Config(void);
static void COMMON_TIMx_Config(u16 arr,u16 psc);
static void BASIC_TIMx_Config(void);

#endif 