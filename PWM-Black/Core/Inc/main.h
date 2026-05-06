/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Black_Pin GPIO_PIN_13
#define LED_Black_GPIO_Port GPIOC
#define LED_2E0_Pin GPIO_PIN_0
#define LED_2E0_GPIO_Port GPIOA
#define LED_2E1_Pin GPIO_PIN_1
#define LED_2E1_GPIO_Port GPIOA
#define LED_2E2_Pin GPIO_PIN_2
#define LED_2E2_GPIO_Port GPIOA
#define LED_2E3_Pin GPIO_PIN_3
#define LED_2E3_GPIO_Port GPIOA
#define LED_2E4_Pin GPIO_PIN_4
#define LED_2E4_GPIO_Port GPIOA
#define Bot_Red_Pin GPIO_PIN_0
#define Bot_Red_GPIO_Port GPIOB
#define Bot_Aum_Pin GPIO_PIN_1
#define Bot_Aum_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
