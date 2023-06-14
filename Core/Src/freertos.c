/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Bluetooth.h"
#include "ChessClock.h"
#include "Utilities.h"
#include "Interface.h"
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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId taskScanPieceHandle;
uint32_t taskScanPieceBuffer[ 4096 ];
osStaticThreadDef_t taskScanPieceControlBlock;
osSemaphoreId binarySemGetFRAMHandle;
osSemaphoreId binarySemMasterGetDataHandle;
osSemaphoreId binarySemI2C_StatusHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
extern void StartTaskScanPiece(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of binarySemGetFRAM */
  osSemaphoreDef(binarySemGetFRAM);
  binarySemGetFRAMHandle = osSemaphoreCreate(osSemaphore(binarySemGetFRAM), 1);

  /* definition and creation of binarySemMasterGetData */
  osSemaphoreDef(binarySemMasterGetData);
  binarySemMasterGetDataHandle = osSemaphoreCreate(osSemaphore(binarySemMasterGetData), 1);

  /* definition and creation of binarySemI2C_Status */
  osSemaphoreDef(binarySemI2C_Status);
  binarySemI2C_StatusHandle = osSemaphoreCreate(osSemaphore(binarySemI2C_Status), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityRealtime, 0, 512);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of taskScanPiece */
  osThreadStaticDef(taskScanPiece, StartTaskScanPiece, osPriorityHigh, 0, 4096, taskScanPieceBuffer, &taskScanPieceControlBlock);
  taskScanPieceHandle = osThreadCreate(osThread(taskScanPiece), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
extern uint8_t ping_chessclock;
extern uint8_t data_chessclock[15];
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
  ChessClock_Init();

  /* Infinite loop */
  for(;;)
  {
	  // kiểm tra trạng thái kết nối với đồng hồ qua I2C
	  osDelay(1000);
	  if(ping_chessclock > 0) {
		  ping_chessclock--;
	  }
	  /* ping_chessclock sẽ đếm ngược đến 0
	   * khi nó đếm đến 0 sẽ set chessclock status = disconnect */
	  if(ping_chessclock==0){
		  data_chessclock[9]=STATUS_DISCONNECT_CLOCK;
		  HAL_NVIC_SystemReset();
	  }

  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
