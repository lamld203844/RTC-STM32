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
#include "ssd1306.h"
#include "fonts.h"
#include "test.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CENTER_Pin GPIO_PIN_13
#define CENTER_GPIO_Port GPIOC

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN PV */

uint8_t time[10];
uint8_t date[10];
uint8_t day[10];
uint8_t today_1[10]="Monday";
uint8_t today_2[10]="Tuesday";
uint8_t today_3[10]="Wednesday";
uint8_t today_4[10]="Thursday";
uint8_t today_5[10]="Friday";
uint8_t today_6[10]="Saturday";
uint8_t today_7[10]="Sunday";
uint8_t alarm=0;
uint8_t adc_res=0;
uint8_t adc_val=0;
GPIO_PinState buttonState = GPIO_PIN_RESET;
GPIO_PinState lastButtonState = GPIO_PIN_RESET;
uint32_t buttonPressTime = 0;
int flag = 0;
uint8_t x=1;
uint16_t sensor1_data, sensor2_data, sensor3_data,sensor4_data;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//void set_time(void)
//{
//	RTC_TimeTypeDef sTime = {0};
//	RTC_DateTypeDef sDate = {0};
//
//	sTime.Hours = 0x10;
//	  sTime.Minutes = 0x35;
//	  sTime.Seconds = 0x40;
//	  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
//	  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
//	  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
//	  {
//	    Error_Handler();
//	  }
//
//	  sDate.WeekDay = RTC_WEEKDAY_TUESDAY;
//	    sDate.Month = RTC_MONTH_MAY;
//	    sDate.Date = 0x16;
//	    sDate.Year = 0x0;
//
//	    if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
//	    {
//	      Error_Handler();
//	    }
//	    HAL_RTCEx_BKUPWrite(&hrtc , RTC_BKP_DR1, 0x32F2);
//
//}
uint8_t day_function(void)
{
	if(day[0]==48&&day[1]==49)
	  {
		  memcpy(day,today_1,sizeof(day));

	  }
	  if(day[0]==48&&day[1]==50)
	    {
	  	  memcpy(day,today_2,sizeof(day));

	    }
	  if(day[0]==48&&day[1]==51)
	    {
	  	  memcpy(day,today_3,sizeof(day));

	    }
	  if(day[0]==48&&day[1]==52)
	    {
	  	  memcpy(day,today_4,sizeof(day));

	    }
	  if(day[0]==48&&day[1]==53)
	    {
	  	  memcpy(day,today_5,sizeof(day));

	    }
	  if(day[0]==48&&day[1]==54)
	    {
	  	  memcpy(day,today_6,sizeof(day));

	    }
	  if(day[0]==48&&day[1]==55)
	    {
	  	  memcpy(day,today_7,sizeof(day));

	    }
	  return day;
}


void get_time(void)
{
  RTC_DateTypeDef gDate;
  RTC_TimeTypeDef gTime;

  /* Get the RTC current Time */
  HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);

  /* Display time Format: hh:mm:ss */
  sprintf((char*)time,"%02d:%02d:%02d",gTime.Hours, gTime.Minutes, gTime.Seconds);

  /* Display date Format: mm-dd-yy */
  sprintf((char*)date,"%02d-%02d-%2d",gDate.Date, gDate.Month, 2000 + gDate.Year);  // I like the date first
  sprintf((char*)day,"%02d",gDate.WeekDay);
  day_function();
}

//void home_screen(void)
//{
//	/* USER CODE END WHILE */
//	  get_time();
//	  SSD1306_GotoXY (15, 25);
//	  SSD1306_Puts (time, &Font_11x18, 1);
//
//	  SSD1306_GotoXY (0, 0);
//	  SSD1306_Puts (date, &Font_7x10, 1);
//
//	  SSD1306_GotoXY (10, 10);
//	  SSD1306_Puts (day, &Font_7x10, 1);
//	  SSD1306_UpdateScreen(); //display
//	  HAL_Delay (200);
//}

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
  MX_I2C1_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

  SSD1306_Init();

//	if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1)!= 0x32F2)
//	{
//		set_time();
//	}
//    set_alarm();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	// ==== get time =========
	  RTC_DateTypeDef gDate;
	  RTC_TimeTypeDef gTime;

	  /* Get the RTC current Time */
	  HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
	  /* Get the RTC current Date */
	  HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);

	  /* Display time Format: hh:mm:ss */
	  sprintf((char*)time,"%02d:%02d:%02d",gTime.Hours, gTime.Minutes, gTime.Seconds);

	  /* Display date Format: mm-dd-yy */
	  sprintf((char*)date,"%02d-%02d-%2d",gDate.Date, gDate.Month, 2000 + gDate.Year);  // I like the date first
	  sprintf((char*)day,"%02d",gDate.WeekDay);
//	  day_function();

	  // ===== show time ======
	  SSD1306_GotoXY (15, 25);
	  SSD1306_Puts (time, &Font_11x18, 1);
	  SSD1306_UpdateScreen(); //display

	  SSD1306_GotoXY (0, 0);
	  SSD1306_Puts (date, &Font_7x10, 1);
	  SSD1306_UpdateScreen(); //display

//	  SSD1306_GotoXY (10, 10);
//	  SSD1306_Puts (day, &Font_7x10, 1);
//	  SSD1306_UpdateScreen(); //display
//	  HAL_Delay (200);


	// ==== Scan keypad and check for key press ====
//	key_pressed = keypad_scan();
//	if(key_pressed != 0)
//	{
//		SSD1306_GotoXY(0, 30);
//		SSD1306_Putc(key_pressed, &Font_7x10, 1);
//		SSD1306_UpdateScreen();
//		HAL_GPIO_TogglePin(GPIOC, LED_Pin);
//	}
//	HAL_Delay(20);

	  // =========== Test OLED ================
//	  SSD1306_GotoXY(0, 15);
//	  SSD1306_Puts("Count", &Font_7x10, 1);
//	  SSD1306_UpdateScreen();
//	  SSD1306_GotoXY(2, 15*2);
//
//	  char intTostr[20];
//
//	  sprintf(intTostr, "%d", count++);
//
//	  SSD1306_Puts(intTostr, &Font_7x10, 1);
//	  SSD1306_UpdateScreen();
//	  HAL_Delay(10);

//	  // ======= Test blink LED ============
//	  HAL_GPIO_TogglePin(GPIOC, LED_Pin);
//	  HAL_Delay(100);

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
  hi2c1.Init.ClockSpeed = 400000;
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
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 22;
  sTime.Minutes = 12;
  sTime.Seconds = 0;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  DateToUpdate.WeekDay = RTC_WEEKDAY_MONDAY;
  DateToUpdate.Month = RTC_MONTH_JULY;
  DateToUpdate.Date = 1;
  DateToUpdate.Year = 24;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the Alarm A
  */
  sAlarm.AlarmTime.Hours = 9;
  sAlarm.AlarmTime.Minutes = 0;
  sAlarm.AlarmTime.Seconds = 1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

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