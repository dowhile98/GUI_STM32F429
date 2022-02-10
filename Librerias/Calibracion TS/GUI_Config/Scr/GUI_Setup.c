/*
 * GUI_Setup.c
 *
 *  Created on: Jan 7, 2022
 *      Author: Lenovo
 */


#include "GUI_Setup.h"


TIM_HandleTypeDef TimHandle;
uint32_t uwPrescalerValue = 0;
extern uint8_t GUI_Initialized;

/**
  * @brief  Initializes the STM32F429I-DISCO's LCD and LEDs resources.
  * @param  None
  * @retval None
  */
void BSP_Config(void)
{
  /* Initialize STM32F429I-DISCO's LEDs */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /* Initializes the SDRAM device */
  BSP_SDRAM_Init();

  /* Initialize the Touch screen */
  BSP_TS_Init(240, 320);

  /* Enable the CRC Module */
  __HAL_RCC_CRC_CLK_ENABLE();
}

/**
  * @brief  BSP_Background.
  * @param  None
  * @retval None
  */
void BSP_Background(void)
{
  /* Capture input event and update cursor */
  if(GUI_Initialized == 1)
  {
    BSP_Pointer_Update();
  }
}


/**
  * @brief  Provide the GUI with current state of the touch screen
  * @param  None
  * @retval None
  */


void BSP_Pointer_Update(void)
{
  GUI_PID_STATE TS_State;
  static TS_StateTypeDef prev_state;
  __IO TS_StateTypeDef  ts;
  uint16_t xDiff, yDiff;

  BSP_TS_GetState((TS_StateTypeDef *)&ts);

  TS_State.Pressed = ts.TouchDetected;

  xDiff = (prev_state.X > ts.X) ? (prev_state.X - ts.X) : (ts.X - prev_state.X);
  yDiff = (prev_state.Y > ts.Y) ? (prev_state.Y - ts.Y) : (ts.Y - prev_state.Y);

  if((prev_state.TouchDetected != ts.TouchDetected )||
     (xDiff > 3 )||
       (yDiff > 3))
  {
    prev_state.TouchDetected = ts.TouchDetected;

    if((ts.X != 0) &&  (ts.Y != 0))
    {
      prev_state.X = ts.X;
      prev_state.Y = ts.Y;
    }

    if(k_CalibrationIsDone())
    {
      TS_State.Layer = 0;
      TS_State.x = k_CalibrationGetX (prev_state.X);
      TS_State.y = k_CalibrationGetY (prev_state.Y);
    }
    else
    {
      TS_State.Layer = 0;
      TS_State.x = prev_state.X;
      TS_State.y = prev_state.Y;
    }

    GUI_TOUCH_StoreStateEx(&TS_State);
  }
}






void TIM3_Config(void){
	/* Compute the prescaler value to have TIM3 counter clock equal to 10 KHz */
	uwPrescalerValue = (uint32_t) ((SystemCoreClock /2) / 10000) - 1;

	/* Set TIMx instance */
	TimHandle.Instance = TIM3;

	   /* Initialize TIM3 peripheral as follows:
	        + Period = 500 - 1
	        + Prescaler = ((SystemCoreClock/2)/10000) - 1
	        + ClockDivision = 0
	        + Counter direction = Up
	   */
	TimHandle.Init.Period = 500 - 1;
	TimHandle.Init.Prescaler = uwPrescalerValue;
	TimHandle.Init.ClockDivision = 0;
	TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
	TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if(HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
	{
		while(1)
		{
		}
	}

	   /*##-2- Start the TIM Base generation in interrupt mode ####################*/
	   /* Start Channel1 */
	if(HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)
	{
		while(1)
		{
		}
	}
}


/**
  * @brief TIM MSP Initialization
  *        This function configures the hardware resources used in This application:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* TIMx Peripheral clock enable */
  __HAL_RCC_TIM3_CLK_ENABLE();

  /*##-2- Configure the NVIC for TIMx ########################################*/
  /* Set the TIMx priority */
  HAL_NVIC_SetPriority(TIM3_IRQn, 0, 1);

  /* Enable the TIMx global Interrupt */
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
}
