/*
 * FreeRTOS V202112.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <lpc21xx.h>

#include "GPIO.H"
#include "TraceDef.h"

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE. 
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION		1
#define configUSE_IDLE_HOOK			0
#define configUSE_TICK_HOOK			1
#define configCPU_CLOCK_HZ			( ( unsigned long ) 60000000 )	/* =12.0MHz xtal multiplied by 5 using the PLL. */
#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES		( 4 )
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 90 )
#define configTOTAL_HEAP_SIZE		( ( size_t ) 13 * 1024 )
#define configMAX_TASK_NAME_LEN		( 8 )
#define configUSE_TRACE_FACILITY	1   /////////////////////////////////////////
#define configUSE_16_BIT_TICKS		0
#define configIDLE_SHOULD_YIELD		0

#define IDLE_PERIOD								200		// this is the idle period and must be larger than any other period to use EDF scheduler

//////////////////////////////////////////////////////////////////////
// Definition 
#define configGENERATE_RUN_TIME_STATS         	1   ////////////////////////
#define configUSE_STATS_FORMATTING_FUNCTIONS		1
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()			
#define portGET_RUN_TIME_COUNTER_VALUE() T1TC				// this macro will return the T1TC register which read the current timer count


//////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////
//  Task Tags and trace hooks
#define configUSE_APPLICATION_TASK_TAG		1

#define traceTASK_SWITCHED_IN()				do\
																				{switch((int)pxCurrentTCB->pxTaskTag){\
																					case Button_1_Monitor_Tag:\
																						GPIO_write(TaskMonitor,Button_1_Monitor_PIN,PIN_IS_HIGH);\
																						TaskTraceArr[Button_1_Monitor_TimeStampIn] = T1TC;\
																						break;\
																					case Button_2_Monitor_Tag:\
																						GPIO_write(TaskMonitor,Button_2_Monitor_PIN,PIN_IS_HIGH);\
																						TaskTraceArr[Button_2_Monitor_TimeStampIn] = T1TC;\
																						break;\
																					case Periodic_Transmitter_Tag:\
																						GPIO_write(TaskMonitor,Periodic_Transmitter_PIN,PIN_IS_HIGH);\
																						TaskTraceArr[Periodic_Transmitter_TimeStampIn] = T1TC;\
																						break;\
																					case Uart_Receiver_Tag:\
																						GPIO_write(TaskMonitor,Uart_Receiver_PIN,PIN_IS_HIGH);\
																						TaskTraceArr[Uart_Receiver_TimeStampIn] = T1TC;\
																						break;\
																					case Load_1_Simulation_Tag:\
																						GPIO_write(TaskMonitor,Load_1_Simulation_PIN,PIN_IS_HIGH);\
																						TaskTraceArr[Load_1_Simulation_TimeStampIn] = T1TC;\
																						break;\
																					case Load_2_Simulation_Tag:\
																						GPIO_write(TaskMonitor,Load_2_Simulation_PIN,PIN_IS_HIGH);\
																						TaskTraceArr[Load_2_Simulation_TimeStampIn] = T1TC;\
																						break;\
																					case Idle_Tag:\
																						GPIO_write(TaskMonitor,Idle_PIN,PIN_IS_HIGH);\
																						TaskTraceArr[Idle_TimeStampIn] = T1TC;\
																						break;\
																							}\
																						}while(0) 

#define traceTASK_SWITCHED_OUT()		do\
																				{switch((int)pxCurrentTCB->pxTaskTag){\
																					case Button_1_Monitor_Tag:\
																						GPIO_write(TaskMonitor,Button_1_Monitor_PIN,PIN_IS_LOW);\
																						TaskTraceArr[Button_1_Monitor_TimeStampOut] = T1TC;\
																						TaskTraceArr[Button_1_Monitor_EcutionTime] = TaskTraceArr[Button_1_Monitor_TimeStampOut]-TaskTraceArr[Button_1_Monitor_TimeStampIn];\
																						TaskTraceArr[Button_1_Monitor_TotalEcutionTime] += TaskTraceArr[Button_1_Monitor_EcutionTime];\
																						break;\
																					case Button_2_Monitor_Tag:\
																						GPIO_write(TaskMonitor,Button_2_Monitor_PIN,PIN_IS_LOW);\
																						TaskTraceArr[Button_2_Monitor_TimeStampOut] = T1TC;\
																						TaskTraceArr[Button_2_Monitor_EcutionTime] = TaskTraceArr[Button_2_Monitor_TimeStampOut]-TaskTraceArr[Button_2_Monitor_TimeStampIn];\
																						TaskTraceArr[Button_2_Monitor_TotalEcutionTime] += TaskTraceArr[Button_2_Monitor_EcutionTime];\
																						break;\
																					case Periodic_Transmitter_Tag:\
																						GPIO_write(TaskMonitor,Periodic_Transmitter_PIN,PIN_IS_LOW);\
																						TaskTraceArr[Periodic_Transmitter_TimeStampOut] = T1TC;\
																						TaskTraceArr[Periodic_Transmitter_EcutionTime] = TaskTraceArr[Periodic_Transmitter_TimeStampOut]-TaskTraceArr[Periodic_Transmitter_TimeStampIn];\
																						TaskTraceArr[Periodic_Transmitter_TotalEcutionTime] += TaskTraceArr[Periodic_Transmitter_EcutionTime];\
																						break;\
																					case Uart_Receiver_Tag:\
																						GPIO_write(TaskMonitor,Uart_Receiver_PIN,PIN_IS_LOW);\
																						TaskTraceArr[Uart_Receiver_TimeStampOut] = T1TC;\
																						TaskTraceArr[Uart_Receiver_EcutionTime] = TaskTraceArr[Uart_Receiver_TimeStampOut]-TaskTraceArr[Uart_Receiver_TimeStampIn];\
																						TaskTraceArr[Uart_Receiver_TotalEcutionTime] += TaskTraceArr[Uart_Receiver_EcutionTime];\
																						break;\
																					case Load_1_Simulation_Tag:\
																						GPIO_write(TaskMonitor,Load_1_Simulation_PIN,PIN_IS_LOW);\
																						TaskTraceArr[Load_1_Simulation_TimeStampOut] = T1TC;\
																						TaskTraceArr[Load_1_Simulation_EcutionTime] = TaskTraceArr[Load_1_Simulation_TimeStampOut]-TaskTraceArr[Load_1_Simulation_TimeStampIn];\
																						TaskTraceArr[Load_1_Simulation_TotalEcutionTime] += TaskTraceArr[Load_1_Simulation_EcutionTime];\
																						break;\
																					case Load_2_Simulation_Tag:\
																						GPIO_write(TaskMonitor,Load_2_Simulation_PIN,PIN_IS_LOW);\
																						TaskTraceArr[Load_2_Simulation_TimeStampOut] = T1TC;\
																						TaskTraceArr[Load_2_Simulation_EcutionTime] = TaskTraceArr[Load_2_Simulation_TimeStampOut]-TaskTraceArr[Load_2_Simulation_TimeStampIn];\
																						TaskTraceArr[Load_2_Simulation_TotalEcutionTime] += TaskTraceArr[Load_2_Simulation_EcutionTime];\
																						break;\
																					case Idle_Tag:\
																						GPIO_write(TaskMonitor,Idle_PIN,PIN_IS_LOW);\
																						TaskTraceArr[Idle_TimeStampOut] = T1TC;\
																						TaskTraceArr[Idle_EcutionTime] = TaskTraceArr[Idle_TimeStampOut]-TaskTraceArr[Idle_TimeStampIn];\
																						TaskTraceArr[Idle_TotalEcutionTime] += TaskTraceArr[Idle_EcutionTime];\
																						break;\
																							}\
																						}while(0)  
/////////////////////////////////////////////////////////

#define configQUEUE_REGISTRY_SIZE 	10

#define configUSE_EDF_SCHEDULER     1			// EDF SCHEDULER 
/////////////////////////////////////////////////////////////////

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 		0
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	0
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1



#endif /* FREERTOS_CONFIG_H */
