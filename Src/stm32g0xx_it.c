/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dataMGR.h"
#include "CE32_HJ580.h"
#include "CE32_sqrs.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern int16_t data_buf[BUF_SIZE/2];		
extern dataMGR MGR;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_lpuart1_tx;
extern UART_HandleTypeDef hlpuart1;
extern TIM_HandleTypeDef htim14;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32G0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel 1 interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_lpuart1_tx);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles TIM14 global interrupt.
  */
void TIM14_IRQHandler(void)
{
  /* USER CODE BEGIN TIM14_IRQn 0 */
	ADC1->CR|=ADC_CR_ADSTART;//Start conversion once
	TIM14->SR&=~TIM_SR_UIF; //Clear interrupt flag(to make next interrupt could happen)
	short temp=ADC1->DR+0x8000;
	dataMGR_enQueue_halfword(&MGR,temp); //Move newly acquired data to buffer
	char resp=sqrs(temp);
	if(resp==1)
	{
		HAL_GPIO_WritePin(SD_CS_GPIO_Port,SD_CS_Pin,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(SD_CS_GPIO_Port,SD_CS_Pin,GPIO_PIN_RESET);
	}
  /* USER CODE END TIM14_IRQn 0 */
  /* USER CODE BEGIN TIM14_IRQn 1 */

  /* USER CODE END TIM14_IRQn 1 */
}

/**
  * @brief This function handles USART3, USART4 and LPUART1 interrupts / LPUART1 wake-up interrupt through EXTI line 28.
  */
void USART3_4_LPUART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_4_LPUART1_IRQn 0 */
//		uint32_t isrflags   = READ_REG(LPUART1->ISR); //read ISR flag
//	if((isrflags&(uint32_t)(USART_ISR_RXNE))!=0)
//	{
//		char data=HJ_UART_DR&0xff; //read data
//		if((BLE_handle.state&HJ580_STATE_CMDRESP)==0) //if not in command receiving mode
//		{
//			if(data=='<')
//			{
//				BLE_handle.state|=HJ580_STATE_CMDRESP; //Enter command mode
//				BLE_handle.cmd_ptr_in=0; 
//				BLE_handle.cmd_RX_len=1;
//			}
//		}
//		else
//		{
//			if(BLE_handle.state&HJ580_STATE_CONFIG)					// IF in HJ580 Config mode
//			{
//				if(data=='>')
//				{
//					BLE_handle.state|=HJ580_STATE_CMD_RESP_PEND; //set command pending tag
//				}
//				else //command body
//				{
//					if(BLE_handle.cmd_ptr_in<HJ_CMD_BUFSIZE)
//					{
//						BLE_handle.cmd_buf[BLE_handle.cmd_idx_rec][BLE_handle.cmd_ptr_in++]=data;//copy data to command buffer ,inc ptr
//					}
//				}
//			}
//			else 																						// IF not in HJ580 Config mode
//			{
//				if(BLE_handle.cmd_ptr_in>BLE_handle.cmd_RX_len)	//If command length exceed protocol length
//				{
//					BLE_handle.state&=~HJ580_STATE_CMDRESP; //QUIT command mode
//					BLE_handle.cmd_ptr_in=0; 
//						
//				}
//				else
//				{
//					if((data=='>'&&(BLE_handle.cmd_ptr_in==BLE_handle.cmd_RX_len))) //Only terminate command if command length meets designed length
//					{
//						BLE_handle.state&=~HJ580_STATE_CMDRESP; //QUIT command mode
//						//BLE_handle.state|=HJ580_STATE_CE32_CMD_PEND; 				//set CE32 command pending tag
//						BLE_handle.cmd_len[BLE_handle.cmd_idx_rec]=BLE_handle.cmd_ptr_in;
//						MSG_CMD_INC(&BLE_handle.cmd_idx_rec);								//Increase incoming cmd pointer
//						BLE_handle.cmd_pending++;
//					}
//					else //command body
//					{
//						if(BLE_handle.cmd_ptr_in==0)
//						{
//							switch((uint8_t)data)
//							{
//								case 0x01:
//									BLE_handle.cmd_RX_len=513;
//									break;
//								case 0x02:
//									BLE_handle.cmd_RX_len=513;
//									break;
//								case 0x03:
//									BLE_handle.cmd_RX_len=513;								
//									break;
//								case 0x04:
//									BLE_handle.cmd_RX_len=1;
//									break;
//								case 0x05:
//									BLE_handle.cmd_RX_len=1;
//									break;		
//								case 0x11:
//									BLE_handle.cmd_RX_len=2;
//									break;									
//								case 0x12:
//									BLE_handle.cmd_RX_len=5;
//									break;
//								case 0x13:
//									BLE_handle.cmd_RX_len=3;
//									break;
//								case 0x14:
//									BLE_handle.cmd_RX_len=6;
//									break;
//								case 0x20:
//									BLE_handle.cmd_RX_len=18;
//									break;
//								case 0x21:
//									BLE_handle.cmd_RX_len=18;
//									break;
//								case 0x22:
//									BLE_handle.cmd_RX_len=25;
//									break;
//								case 0x23:
//									BLE_handle.cmd_RX_len=25;
//									break;
//								case 0x30: //start logging
//									BLE_handle.cmd_RX_len=1;
//									break;
//								case 0x31: //Stop logging
//									BLE_handle.cmd_RX_len=1;
//									break;
//								case 0x40:
//									BLE_handle.cmd_RX_len=1;
//									break;
//								case 0x41:
//									BLE_handle.cmd_RX_len=1;
//									break;
//								case 0x42:
//									BLE_handle.cmd_RX_len=2;
//									break;
//								case 0x50:
//									BLE_handle.cmd_RX_len=1;
//									break;
//								case 0x80:
//									BLE_handle.cmd_RX_len=1;
//									break;
//								case 0x83:
//									BLE_handle.cmd_RX_len=2;
//									break;
//								case 0x90:   //Send system parameter
//									BLE_handle.cmd_RX_len=1;
//									break;
//								case 0x91:	//Send DSP1
//									BLE_handle.cmd_RX_len=1;
//									break;
//								case 0x92:	//Send DSP2
//									BLE_handle.cmd_RX_len=1;
//									break;
//								case 0xAB:	//Software reset
//									BLE_handle.cmd_RX_len=2;
//									break;
//								case 0xF0: //LPF test
//									BLE_handle.cmd_RX_len=513;
//									break;
//								case 0xF1: //mainFil1 test
//									BLE_handle.cmd_RX_len=513;
//									break;
//								case 0xF2: //mainFIl2 test
//									BLE_handle.cmd_RX_len=513;
//									break;
//								case 0xF3: //maFIL1 test
//									BLE_handle.cmd_RX_len=513;
//									break;
//								case 0xF4: //maFil2 test
//									BLE_handle.cmd_RX_len=513;
//									break;
//								default:	//For error received codes
//									BLE_handle.cmd_RX_len=1;
//									break;
//							}
//						}
//						if(BLE_handle.cmd_ptr_in<HJ_CMD_BUFSIZE)
//						{
//							BLE_handle.cmd_buf[BLE_handle.cmd_idx_rec][BLE_handle.cmd_ptr_in++]=data;//copy data to command buffer ,inc ptr
//						}
//					}
//				}
//			}
//		}
//	}
//	//TX ISR
//	if((huart6.Instance->CR1&USART_CR1_TXEIE)!=0)
//	{		
//		if((isrflags&(uint32_t)(USART_SR_TXE))!=0)
//		{
//			if(huart6.TxXferCount>0){
//				huart6.Instance->DR=*(uint8_t*)huart6.pTxBuffPtr; //if buffer not empty, send one byte from TX buffer
//				huart6.TxXferCount--;
//				huart6.pTxBuffPtr++;
//			}
//			else
//			{
//				CLEAR_BIT(huart6.Instance->CR1, USART_CR1_TXEIE); //clear TXE interrupt if buffer is empty
//				while(UART_CHECK_TXE==0){};
//				HJ_UART->DR = 0xDA; //send header
//				while(UART_CHECK_TXE==0){};
//				HJ_UART->DR = 0xDA; //send header		
//				dataMGR_deQueue(&MGR_prev,PREV_TRANS_BYTES,0);	
//				MGR.logState&=~MGR_STATE_PREV_TRANSBUSY;
//			}
//		}
//	}
//	MGR.logState&=~MGR_STATE_SLEEP;//Wake up main logging cycle
  /* USER CODE END USART3_4_LPUART1_IRQn 0 */
  /* USER CODE BEGIN USART3_4_LPUART1_IRQn 1 */

  /* USER CODE END USART3_4_LPUART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
