/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "eth.h"
#include "i2c.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

int _write(int file, char *ptr, int len)
{
	HAL_UART_Transmit(&huart3, (uint8_t*)ptr, len, 500);
	return len;
}

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_10
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_10  \
		+  GetSectorSize(ADDR_FLASH_SECTOR_10) -1 /* End @ of user Flash area : sector start address + sector size -1 */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
CLK_State_t clock_state = NORMAL_STATE;

uint8_t rx3Data, rx4Data;

RTC_AlarmTypeDef sAlarm;
uint32_t Joycon[2];

char showTime[16] = {0};
char showDate[16] = {0};
char ampm[2][3] = {"AM", "PM"};
char copyTime[16];
char controlTime[16] = {0,};
bleBuffer_t ble = {0};
uint8_t cpyflag = 0, oneClick;
bool isSave = false;

uint8_t curr_ap, curr_h, curr_m, curr_s;

int adc1_val, adc2_val;

unsigned int stime, etime = 0, interval, ctime, count = 0;
unsigned int dbclk, holdclk;
buttonState buttonPosition;
timeFormat_t timeFormatter;
unsigned char buf[120];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


#define DATA_32                 ((uint32_t)0x12345678)

uint32_t SECTORError = 0;
__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;

//Status_flashRW readFlash(uint32_t StartADDR)
//{
//	unsigned int value = *(unsigned int*)StartADDR;
//	printf("addr[0x%08x] = %08x\r\n", StartADDR, value);
//	return RW_OK;
//}
//Status_flashRW eraseFlash(uint32_t ADDR_FLASH_SECTOR_x)
//{
//	uint32_t Address = 0;
//	HAL_FLASH_Unlock();
//	FirstSector = GetSector(FLASH_USER_START_ADDR);
//	NbOfSectors = GetSector(FLASH_USER_END_ADDR) - FirstSector + 1;
//	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
//	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
//	EraseInitStruct.Sector        = FirstSector;
//	EraseInitStruct.NbSectors     = NbOfSectors;
//
//	Address = FLASH_USER_START_ADDR;
//	while(Address < FLASH_USER_END_ADDR)
//	{
//		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, DATA_32) == HAL_OK)
//		{
//			printf("Changed: ");
//			readFlash(Address);
//			Address = Address + 4;
//		}
//		else
//		{
//			printf("error occurred");
//		}
//	}
//
//
//	HAL_FLASH_Lock();
//	return RW_OK;
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
  MX_DMA_Init();
  MX_ETH_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_I2C1_Init();
  MX_RTC_Init();
  MX_TIM2_Init();
  MX_UART4_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
	HAL_ADC_Start_DMA(&hadc1, Joycon, sizeof(Joycon)/sizeof(Joycon[0]));
	HAL_UART_Receive_IT(&huart3, (uint8_t*)&rx3Data, sizeof(rx3Data));
	HAL_UART_Receive_IT(&huart4, (uint8_t*)&rx4Data, sizeof(rx4Data));

	//unsigned int value, addr = FLASH_USER_START_ADDR, cnt = 0;
	//unsigned char buf[30];
	HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN);
	init();
	LCD_Clear();
	set_date(RTC_WEEKDAY_MONDAY, 11, 13, 23);
	set_time(13,25,0);
	set_alarm(0, 0, 5);
	//eraseFlash(FLASH_USER_START_ADDR);
	//springWater_song();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
		//printf("x: %d\r\ny: %d\r\n",Joycon[0],Joycon[1]);
		//printf("current state: %d\r\n", clock_state);
		switch(clock_state)
		{
		case NORMAL_STATE:
			get_time();
			break;
		case TIME_SETTING:
			if(cpyflag == 1)
			{
				memcpy(copyTime, showTime, sizeof(showTime)/sizeof(showTime[0]));
				cpyflag = 0;
			}
			// 1. timer setting
			// 2. save at flash
			timeSetter();
			if(isSave == true)
			{
				saveCurrentTime();
			}
			printf("setting mode\r\n");
			break;
		case ALARM_TIME_SETTING:
			// 1. alarm time setting
			// 2. save at flash
			printf("alarm mode\r\n");
			break;
		case MUSIC_SELECT:
			// 1. music select
			// 2. save at flash
			printf("music mode\r\n");
			break;
		default:
			clock_state = NORMAL_STATE;
			break;
		}
		HAL_Delay(300);
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* TIM2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* RTC_Alarm_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
  /* EXTI9_5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
  /* ADC_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(ADC_IRQn);
  /* USART3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* UART4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(UART4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(UART4_IRQn);
  /* EXTI15_10_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/* USER CODE BEGIN 4 */
void set_time(uint8_t hh, uint8_t mm, uint8_t ss)
{
	RTC_TimeTypeDef sTime;

	sTime.Hours = hh + 1;
	sTime.Minutes = mm;
	sTime.Seconds = ss;
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
}
void set_alarm(uint8_t hh, uint8_t mm, uint8_t ss)
{
	sAlarm.AlarmTime.Hours = hh;
	sAlarm.AlarmTime.Minutes = mm;
	sAlarm.AlarmTime.Seconds = ss;
	sAlarm.AlarmTime.SubSeconds = 0x0;
	sAlarm.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	sAlarm.AlarmDateWeekDay = 0x1;
	sAlarm.Alarm = RTC_ALARM_A;
	if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN);
}
void set_date(uint8_t ww, uint8_t mm, uint8_t dd, uint8_t yy)
{
	RTC_DateTypeDef sDate;

	sDate.WeekDay = ww;
	sDate.Month = mm;
	sDate.Date = dd;
	sDate.Year = yy;
	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
}

void get_time(void)
{
	RTC_DateTypeDef sDate;
	RTC_TimeTypeDef sTime;
	int longer;
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	curr_ap = sTime.TimeFormat;
	curr_h = sTime.Hours;
	curr_m = sTime.Minutes;
	curr_s = sTime.Seconds;
	memset(showTime, 0, sizeof(showTime)/sizeof(showTime));
	memset(showDate, 0, sizeof(showDate)/sizeof(showDate));
	sprintf((char*)showTime, "%s %02d:%02d:%02d",ampm[sTime.TimeFormat], sTime.Hours, sTime.Minutes, sTime.Seconds);
	sprintf((char*)showDate, "  %04d/%02d/%02d",2000+sDate.Year, sDate.Month, sDate.Date);
	LCD_PrintAll(showDate, showTime);
	//printf("%s %s\r\n", showTime, showDate);
}


void timeSetter(void)
{
	static bool toggleChar = false;
	static uint8_t choice = 0;

	if(IsRight())
	{
		choice ++;
		if(choice > 3)
		{
			choice = 0;
		}
	}
	else if(IsLeft())
	{
		choice --;
		if(choice > 4)
		{
			choice = 3;
		}
	}
	//"%s %02d:%02d:%02d"
	switch (choice)
	{
		case AMPM:
			if(toggleChar)
			{
				sprintf(controlTime, "__ %02d:%02d:%02d",curr_h,curr_m,curr_s);
			}
			else
			{
				if(IsUP())
				{
					curr_ap++;
				}
				else if(IsDown())
				{
					curr_ap--;
				}
				curr_ap = curr_ap % 2;
				sprintf(controlTime, "%s %02d:%02d:%02d",ampm[curr_ap],curr_h,curr_m,curr_s);
			}
			break;
		case HOUR:
			if(toggleChar)
			{
				sprintf(controlTime, "%s __:%02d:%02d",ampm[curr_ap%2],curr_m,curr_s);
			}
			else
			{
				if(IsUP())
				{
					curr_h++;
				}
				else if(IsDown())
				{
					curr_h--;
				}
				curr_h %= 24;
				sprintf(controlTime, "%s %02d:%02d:%02d",ampm[curr_ap],curr_h,curr_m,curr_s);
			}
			break;
		case MIN:
			if(toggleChar)
			{
				sprintf(controlTime, "%s %02d:__:%02d",ampm[curr_ap],curr_h,curr_s);
			}
			else
			{
				if(IsUP())
				{
					curr_m++;
				}
				else if(IsDown())
				{
					curr_m--;
				}
				curr_m %= 60;
				sprintf(controlTime, "%s %02d:%02d:%02d",ampm[curr_ap],curr_h,curr_m,curr_s);
			}
			break;
		case SEC:
			if(toggleChar)
			{
				sprintf(controlTime, "%s %02d:%02d:__",ampm[curr_ap],curr_h,curr_m);
			}
			else
			{
				if(IsUP())
				{
					curr_s++;
				}
				else if(IsDown())
				{
					curr_s--;
				}
				curr_s %= 60;
				sprintf(controlTime, "%s %02d:%02d:%02d",ampm[curr_ap],curr_h,curr_m,curr_s);
			}
			break;
		default:
			break;
	}
	LCD_PrintAll("Set Time      ", controlTime);
	toggleChar = !toggleChar;
}
void saveCurrentTime(void)
{
	isSave = false;
	clock_state = NORMAL_STATE;
}
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	HAL_GPIO_TogglePin(GPIOB, LD1_Pin|LD2_Pin|LD3_Pin);
	printf("RINGRINGRINGRING!!!!!!!!!!!!!!!!!!!!\r\n");
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	//stime = HxAL_GetTick();
	// joystick switch
	GPIO_PinState pinstate;
	if(GPIO_Pin == SWITCH_Pin)
	{
		//printf("switch toggle\r\n");
		stime = HAL_GetTick();
		interval = stime - etime;
		etime = stime;
		if(interval > 100)
		{
			memset(buf,0, sizeof(buf));
			pinstate = HAL_GPIO_ReadPin(SWITCH_GPIO_Port, SWITCH_Pin);
			if(pinstate == GPIO_PIN_SET)
			{
				printf("OFF\r\n");
				holdclk = interval;
				oneClick++;
								oneClick %= 2;
			}
			else if(pinstate == GPIO_PIN_RESET)
			{
				printf("ON\r\n");
				holdclk = 0;
				if(interval < 1000)
				{
					dbclk++;
				}
				else
				{
					dbclk = 0;
				}

			}
			//printf("intvl time: %d\r\n",interval);
			// Alarm time Setting State
			if(holdclk >= 3000)
			{
				printf("***held at least 3 sec ***\r\n");
			}
			// Morning call Setting State
			else if(dbclk >= 2)
			{
				printf("***double click activate***\r\n");
				dbclk = 0;
			}
			// Timer Setting State
			if(oneClick == 1)
			{
				printf("timer setting\r\n");
				cpyflag = 1;
				if(clock_state == NORMAL_STATE)
				{
					clock_state = TIME_SETTING;
					printf("set to Time\r\n");
				}
				else if(clock_state == TIME_SETTING)
				{
					printf("set to Save\r\n");
					isSave = true;
				}
			}
		}
	}
}

bool IsRight(void)
{
	return Joycon[0] >= RIGHT ? true : false;
}
bool IsLeft(void)
{
	return Joycon[0] <= LEFT ? true : false;
}
bool IsUP(void)
{
	return Joycon[1] >= UP ? true : false;
}
bool IsDown(void)
{
	return Joycon[1] <= DOWN ? true : false;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	// From COM3 ->(UART3)ST(UART4) ->BLE
	if(huart->Instance == USART3)
	{
		// received enter key
		if(rx3Data == '\r' || ble.cur_BLE_Index > 64)
		{
			printf("\r\n send following data to BLE \r\n");
			printf(ble.bleBuffer);
			printf("\r\n");
			HAL_UART_Transmit(&huart4, (uint8_t*)&ble.bleBuffer, ble.cur_BLE_Index, 500);
			memset(ble.bleBuffer, 0, sizeof(ble.bleBuffer)/sizeof(ble.bleBuffer[0]));
			ble.cur_BLE_Index = 0;
		}
		else
		{

			ble.bleBuffer[ble.cur_BLE_Index] = rx3Data;
			printf("collecting: %c\r\n", (char)rx3Data);
			ble.cur_BLE_Index++;
		}

		HAL_UART_Receive_IT(&huart3, (uint8_t*)&rx3Data, sizeof(rx3Data));
	}
	// From BLE ->(UART4)ST(UART3)->COM3
	else if(huart->Instance == UART4)
	{
		HAL_UART_Transmit(&huart3, (uint8_t*)&rx4Data, sizeof(rx4Data), 100);
		HAL_UART_Receive_IT(&huart4, (uint8_t*)&rx4Data, sizeof(rx4Data));
#if 0
		if(ble.comBuffer[ble.cur_COM_Index] == '\0' | ble.cur_COM_Index > 64)
		{
			HAL_UART_Transmit(&huart3, (uint8_t*)&rx3Data, sizeof(rx3Data), 100);
			printf("data received from BLE\r\n");
			HAL_UART_Transmit(&huart4, (uint8_t*)&rx4Data, sizeof(rx4Data), 100);
			ble.cur_COM_Index = 0;
		}
		else
		{
			ble.comBuffer[ble.cur_COM_Index] = rx4Data;

			printf("something wrong here..%c\r\n", ble.comBuffer[ble.cur_COM_Index]);
			ble.cur_COM_Index++;
		}
		HAL_UART_Receive_IT(&huart4, (uint8_t*)&rx4Data, sizeof(rx4Data));
#endif
	}

}

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
