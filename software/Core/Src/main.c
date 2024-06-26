/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

#include "application_tasks.h"

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
ADC_HandleTypeDef hadc1;

RTC_HandleTypeDef hrtc;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */

UART_HandleTypeDef *sim800_uart = &huart2;
UART_HandleTypeDef *gps_uart = &huart6;


SIM800_Handle_t sim800h = {0};
NEO6M_Handle_t neo6mh = {0};


QueueHandle_t xCommandQueue;
QueueHandle_t xSMSToSendQueue;
QueueHandle_t xNumbersToSendGPSQueue;
QueueHandle_t xNumbersToSendVoltageQueue;
QueueHandle_t xTimeToRTCAlarmQueue;
QueueHandle_t xGPSCoordinatesQueue;
QueueHandle_t xADCValueQueue;

TaskHandle_t xSim800ResetTaskHandle;
TaskHandle_t xCmdProcessTaskHandle;
TaskHandle_t xSendGPSTaskHandle;
TaskHandle_t xSendVoltageTaskHandle;
TaskHandle_t xSendCurrentAlarmTaskHandle;
TaskHandle_t xSendSMSTaskHandle;
TaskHandle_t xConfigRTCAlarmTaskHandle;


uint32_t currentTime = 0;

char phoneNumberForNotify[PHONE_NUMBER_MAX_LENGHT];


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */

static void set_current_time(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART6_UART_Init();
  MX_ADC1_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

    set_current_time();

    HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);

    xCommandQueue = xQueueCreate(COMMAND_QUEUE_LEGHT, sizeof( SMS_t ));
    xSMSToSendQueue = xQueueCreate(SMS_QUEUE_LEGHT, sizeof( SMS_t ));
    xNumbersToSendGPSQueue = xQueueCreate(SEND_GPS_QUEUE_LENGHT, PHONE_NUMBER_MAX_LENGHT);
    xNumbersToSendVoltageQueue = xQueueCreate(SEND_VOLTAGE_QUEUE_LENGHT, PHONE_NUMBER_MAX_LENGHT);
    xTimeToRTCAlarmQueue = xQueueCreate(ALARM_TIME_QUEUE_LENGHT, sizeof( uint32_t ));
    xGPSCoordinatesQueue = xQueueCreate(GPS_QUEUE_LEGHT, sizeof( Coordinates_t ));
    xADCValueQueue = xQueueCreate(ADC_QUEUE_LEGHT, sizeof( uint32_t ));

    create_tasks();

    vTaskStartScheduler();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0;
  sTime.Minutes = 0;
  sTime.Seconds = 0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 1;
  sDate.Year = 0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the Alarm A
  */
  sAlarm.AlarmTime.Hours = 0;
  sAlarm.AlarmTime.Minutes = 0;
  sAlarm.AlarmTime.Seconds = 0;
  sAlarm.AlarmTime.SubSeconds = 0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(NEO6_PWR_CNTRL_GPIO_Port, NEO6_PWR_CNTRL_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SIM_PWR_CNTRL_GPIO_Port, SIM_PWR_CNTRL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : NEO6_PWR_CNTRL_Pin */
  GPIO_InitStruct.Pin = NEO6_PWR_CNTRL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(NEO6_PWR_CNTRL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SIM_PWR_CNTRL_Pin */
  GPIO_InitStruct.Pin = SIM_PWR_CNTRL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SIM_PWR_CNTRL_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
 * @brief  Callback function called when data is received on UART.
 * @param  huart Pointer to the UART handle structure.
 * @retval None
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Check if the received data is from the SIM800 module */
  if(huart == sim800_uart)
  {
    /* Call the SIM800 message handler */
    SIM800_MessageHandler(&sim800h);
  }
  /* Check if the received data is from the NEO6M GPS module */
  else if( huart == gps_uart )
  {
    /* Call the NEO6M message handler */
    NEO6M_MessageHandler(&neo6mh);
  }
}

/**
 * @brief  Callback function called when ADC conversion is completed.
 * @param  hadc Pointer to the ADC handle structure.
 * @retval None
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  uint32_t adc_value = 0;

  /* Get the ADC conversion value */
  adc_value = HAL_ADC_GetValue(hadc);

  /* Send the ADC value to the queue from the interrupt context */
  xQueueSendFromISR( xADCValueQueue, ( void * )&adc_value, NULL );
}

/**
 * @brief  Callback function called when an RMC message is received from the NEO6M GPS module.
 * @param  package Pointer to the RMC package data structure.
 * @retval None
 */
void NEO6M_RMCCallBack(void *package)
{
    RMC_Package_t *rmc_package = (RMC_Package_t *)package;
    Coordinates_t cor = {0};

    /* Check if valid latitude and longitude values are received */
    if( rmc_package->latitude != 0 && rmc_package->longitude != 0 )
    {
        /* Store the coordinates in the local structure */
        cor.latitude = rmc_package->latitude;
        cor.longitude = rmc_package->longitude;

        /* Send the coordinates to the queue from the interrupt context */
        xQueueSendFromISR( xGPSCoordinatesQueue, ( void * ) &cor, NULL );
    }
}

/**
 * @brief  Callback function called when a GLL message is received from the NEO6M GPS module.
 * @param  package Pointer to the GLL package data structure.
 * @retval None
 */
void NEO6M_GLLCallBack(void *package)
{
    GLL_Package_t *gll_package = (GLL_Package_t *)package;

    /* Check if valid latitude and longitude values are received */
    if( gll_package->latitude != 0 && gll_package->longitude != 0 )
    {
        /* Update the current time based on the received time from the GLL message */
        currentTime = gll_package->time + 30000;
        if(currentTime > 240000)
        {
            currentTime -= 240000;
        }
    }
}

/**
 * @brief  Callback function called when a new SMS notification is received by the SIM800 module.
 * @param  handle Pointer to the SIM800 handle structure.
 * @param  sms_index Index of the received SMS message.
 * @retval None
 */
void SIM800_NewSMSNotificationCallBack(SIM800_Handle_t *handle, uint32_t sms_index)
{
    /* Request the SIM800 module to send the received SMS message */
    SIM800_RequestSMSMessage(handle, sms_index);
}

/**
 * @brief  Callback function called when an SMS message is received by the SIM800 module.
 * @param  handle Pointer to the SIM800 handle structure.
 * @param  message Pointer to the received SMS message structure.
 * @retval None
 */
void SIM800_RcvdSMSCallBack(SIM800_Handle_t *handle, SIM800_SMSMessage_t *message)
{
    SMS_t cmd_buff = {0};
    size_t phone_number_size = 0, sms_message_size = 0;

    /* Get the length of the phone number and SMS message */
    phone_number_size = strlen(message->sender);
    sms_message_size = strlen(message->text);

    /* Truncate the phone number if it exceeds the maximum allowed length */
    if( phone_number_size > SMS_SENDER_MAX_LEN)
    {
        message->sender[SMS_SENDER_MAX_LEN] = '\0';
    }

    /* Truncate the SMS message if it exceeds the maximum allowed length */
    if( sms_message_size > SMS_TEXT_MAX_LEN)
    {
        message->text[SMS_TEXT_MAX_LEN] = '\0';
    }

    /* Copy the phone number and SMS message to the command buffer */
    strcpy((char *)&cmd_buff.number, message->sender);
    strcpy((char *)&cmd_buff.message, message->text);

    /* Send the command buffer to the queue from the interrupt context */
    xQueueSendFromISR( xCommandQueue, ( void * ) &cmd_buff, NULL );
}

/**
 * @brief  Callback function called when the RTC alarm event occurs.
 * @param  hrtc Pointer to the RTC handle structure.
 * @retval None
 */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
    /* Send the phone number for notification to the queue for sending GPS coordinates */
    xQueueSendFromISR( xNumbersToSendGPSQueue, phoneNumberForNotify, 0 );
}

/**
 * @brief  Function to set the current time in the RTC using the GPS module.
 * @param  None
 * @retval None
 */
static void set_current_time(void)
{
    RTC_TimeTypeDef sTime;
    uint8_t hrs = 0, min = 0, sec = 0;

    /* Enable power to the GPS module */
    HAL_GPIO_WritePin(NEO6_PWR_CNTRL_GPIO_Port, NEO6_PWR_CNTRL_Pin, RESET);
    /* Add the GLL message type to the expected message list */
    NEO6M_AddExpectedMessage(&neo6mh, GLL);

    /* Wait until the current time is updated from the GLL message */
    while( !currentTime );

    /* Remove the GLL message type from the expected message list */
    NEO6M_RemoveExpectedMessage(&neo6mh, GLL);
    /* Disable power to the GPS module */
    HAL_GPIO_WritePin(NEO6_PWR_CNTRL_GPIO_Port, NEO6_PWR_CNTRL_Pin, SET);

    /* Extract the hours, minutes, and seconds from the current time */
    hrs = currentTime / 10000;
    min = ( currentTime - hrs * 10000 ) / 100;
    sec = currentTime - ( hrs * 10000 + min * 100 );

    /* Configure the RTC time structure */
    sTime.Hours = hrs;
    sTime.Minutes = min;
    sTime.Seconds = sec;
    sTime.TimeFormat = RTC_HOURFORMAT12_AM;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;

    /* Set the RTC time */
    HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
}


/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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

#ifdef  USE_FULL_ASSERT
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
