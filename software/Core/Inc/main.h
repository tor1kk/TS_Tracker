/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

#include "neo-6m.h"
#include "sim800.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */


typedef struct
{
	uint8_t number[20];
	uint8_t message[100];
}SMS_t;


typedef struct
{
	double latitude;
	double longitude;
}Coordinates_t;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define NEO6_PWR_CNTRL_Pin GPIO_PIN_14
#define NEO6_PWR_CNTRL_GPIO_Port GPIOB
#define SIM_PWR_CNTRL_Pin GPIO_PIN_10
#define SIM_PWR_CNTRL_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

#define COMMAND_QUEUE_LEGHT 		5
#define SEND_GPS_QUEUE_LENGHT		5
#define SEND_VOLTAGE_QUEUE_LENGHT	5
#define SMS_QUEUE_LEGHT				5
#define GPS_QUEUE_LEGHT				1
#define ALARM_TIME_QUEUE_LENGHT		1
#define ADC_QUEUE_LEGHT 			1

#define PHONE_NUMBER_MAX_LENGHT		20

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
