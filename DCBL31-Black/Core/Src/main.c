/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "i2c_lcd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim4;

/* USER CODE BEGIN PV */
I2C_LCD_HandleTypeDef lcd1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM4_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* uint16_t MemEstadoSwitch = 0; //Co-Variavel para atualização do LCD
 uint16_t EstadoSwitch = 1; //Define o que aparece nos LCD, pode assumir ou 0 ou 1*/
 uint16_t PSCMod = 20; //Usada para modificar o PreScaler que consequentemente modifica o periodo
 uint16_t CBL = 1; //Contador Binario de LEDs -> valor real de LEDs Ascesos = CBL-1
 float PeriodoMS = ( (20 * 64200 ) / 12000); // Usada para medir o período de cada ciclo (em ms)
/* USER CODE END 0 */

 /* USER CODE BEGIN 1 */
void AtualizaPSCTimer(TIM_HandleTypeDef * Timer, uint16_t NovoPSC)
{
	Timer->Instance->PSC=NovoPSC+1;
	PeriodoMS = ( (NovoPSC * Timer->Instance->ARR ) / 12000);
	Timer->Instance->CNT = Timer->Instance->ARR-1;
}

/*void AtualizaLCD(I2C_LCD_HandleTypeDef * LCD, TIM_HandleTypeDef * Timer, uint16_t Estado)
{
	if(Estado)
	{
	  lcd_clear_all(LCD);
	  lcd_gotoxy(LCD, 0, 0);
	  lcd_put_string(LCD, "PSC: ");
	  //char TextPSC[6];
	  //sprintf(TextPSC, "%lu", Timer->Instance->PSC);
	  //lcd_put_ConvertedString(LCD, TextPSC);

	  lcd_gotoxy(LCD, 0, 1);
	  lcd_put_string(LCD, "T: ");
	  //uint16_t PMS = PeriodoMS;
	  //char TextPMS[6];
	  //sprintf(TextPMS, "%u", PMS);
	  //lcd_put_ConvertedString(LCD, TextPMS);
	}
	else
	{
	  lcd_clear_all(LCD);
	  lcd_gotoxy(LCD, 0, 0);
	  lcd_put_string(LCD, "Status:");
	  //Timer->State==HAL_TIM_STATE_RESET?lcd_put_string(LCD, "Ativo"):lcd_put_string(LCD, "Inativo");

	  lcd_gotoxy(LCD, 0, 1);
	  lcd_put_string(LCD, "Contando:");
	  //char TextCNT[6];
	  //sprintf(TextCNT, "%lu", Timer->Instance->CNT);
	  //lcd_put_ConvertedString(LCD, TextCNT);
	}
}*/
 /* USER CODE END 1 */
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM4_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim4); /*Permite fazer interrupções*/

  /*lcd1.hi2c = &hi2c1;
  lcd1.address = 0x4E;
  lcd_init(&lcd1);

  lcd_clear_all(&lcd1);
  lcd_gotoxy(&lcd1, 0, 0);
  lcd_put_string(&lcd1, "LCD STM32F411");
  lcd_gotoxy(&lcd1, 0, 1);
  lcd_put_string(&lcd1, "ATUALIZADO :)");*/
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
	{
	//Para que o usuário possa pressionar o botão sem se preocupar com a função HAL_Delay()
	for(int i=0;i<8/*8*10ms = 80 ms*/;i++)
		{
			//Caso pressione o botão de reduzir periodo em 107ms:
			if(HAL_GPIO_ReadPin(GPIOB, Bot_Red_Pin))
			{
				while((HAL_GPIO_ReadPin(GPIOB, Bot_Red_Pin))!=0){}

				// Caso esteja no limite e tente diminuir, irá ao valor máximo
				PSCMod==20?
					PSCMod=65520 //Vai ao valor máximo, Período ~= 350s ~= 5min + 50.5s
					: //Se não estiver no valor mínimo definido, então
					(PSCMod-=20); // Reduz 107 ms do periodo

				AtualizaPSCTimer(&htim4, PSCMod);

				/*
				lcd_clear_row(&lcd1, 0);
				lcd_gotoxy(&lcd1, 0, 0);
				lcd_put_string(&lcd1, "LCD");*/
			}

			//Caso pressione o botão de aumentar o periodo em 107 ms:
			if(HAL_GPIO_ReadPin(GPIOB, Bot_Aum_Pin))
			{
				while((HAL_GPIO_ReadPin(GPIOB, Bot_Aum_Pin))!=0){}

				// Caso esteja no limite e tente aumentar, irá ao valor mínimo
				PSCMod==65520?//Limite uint16_t == 65535
					PSCMod=20 // Valor mínimo definido, Período de 107 ms
					: //Se não estiver no valor máximo definido, então
					(PSCMod+=20); // Aumenta 107 ms do período

				AtualizaPSCTimer(&htim4, PSCMod);

				/*
				lcd_clear_row(&lcd1, 1);
				lcd_gotoxy(&lcd1, 0, 1);
				lcd_put_string(&lcd1, "411");*/
			}


		  //10ms*8=80ms
		  HAL_Delay(10);
		}
	  //Pisca LED Black (a cada 83ms):
	  HAL_Delay(3); //Queremos com 12Hz ~= 83ms
	  HAL_GPIO_TogglePin(GPIOC, LED_Black_Pin);

	  //Atualiza LCD apenas se houver atualização no switch:
	  /*EstadoSwitch = HAL_GPIO_ReadPin(GPIOB, SW_Mod_Pin);
	  if (MemEstadoSwitch!=EstadoSwitch)
	  {
		  MemEstadoSwitch = EstadoSwitch;
		  AtualizaLCD(&lcd1, &htim4, EstadoSwitch);
	  }*/
	}
    /* USER CODE END WHILE */
}
    /* USER CODE BEGIN 3 */
void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef * Timer) /*Ativada a cada 1 PeriodoMS || mudança em seu valor*/
{
	//Saídas A,B,C podem ser (des)ativadas ao mesmo tempo ao (des)ativar a soma delas
	HAL_GPIO_WritePin(GPIOA, ~(CBL), 0); //Apaga LEDs ~correspondentes ao contador CBL
	HAL_GPIO_WritePin(GPIOA, (CBL), 1); //Ascende os LEDs correspondentes ao contador CBL

	CBL==31?CBL=0:CBL++; //Incrementa o contador até resetá-lo quando chegar à 2^5-1
}
  /* USER CODE END 3 */

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV6;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 21-1;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 64201-1;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_Black_GPIO_Port, LED_Black_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_2E0_Pin|LED_2E1_Pin|LED_2E2_Pin|LED_2E3_Pin
                          |LED_2E4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Black_Pin */
  GPIO_InitStruct.Pin = LED_Black_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_Black_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_2E0_Pin LED_2E1_Pin LED_2E2_Pin LED_2E3_Pin
                           LED_2E4_Pin */
  GPIO_InitStruct.Pin = LED_2E0_Pin|LED_2E1_Pin|LED_2E2_Pin|LED_2E3_Pin
                          |LED_2E4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Bot_Red_Pin Bot_Aum_Pin */
  GPIO_InitStruct.Pin = Bot_Red_Pin|Bot_Aum_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : SW_Mod_Pin */
  GPIO_InitStruct.Pin = SW_Mod_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SW_Mod_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
