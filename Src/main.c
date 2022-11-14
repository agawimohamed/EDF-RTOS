/*
 * FreeRTOS Kernel V10.2.0
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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

/* 
	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used.
*/


/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the demo application tasks.
 * 
 * Main.c also creates a task called "Check".  This only executes every three 
 * seconds but has the highest priority so is guaranteed to get processor time.  
 * Its main function is to check that all the other tasks are still operational.
 * Each task (other than the "flash" tasks) maintains a unique count that is 
 * incremented each time the task successfully completes its function.  Should 
 * any error occur within such a task the count is permanently halted.  The 
 * check task inspects the count of each task to ensure it has changed since
 * the last time the check task executed.  If all the count variables have 
 * changed all the tasks are still executing error free, and the check task
 * toggles the onboard LED.  Should any task contain an error at any time 
 * the LED toggle rate will change from 3 seconds to 500ms.
 *
 */

/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "lpc21xx.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"

/* Trace Hooks Definitions*/
#include  "TraceDef.h"


/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )


/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */

///////////////////////////////////////////////////////////////
#define Q_MessageLen		30
#define QueueItemNumb		50

typedef struct {
	unsigned char MessageID;
	unsigned char Message[Q_MessageLen];
} Q_Message;

QueueHandle_t MessageQueue = 0;
///////////////////////////////////////////////////////////////


static void prvSetupHardware( void );

uint8_t getExcutionPercentage(uint32_t var);
/*-----------------------------------------------------------*/

TaskHandle_t Button_1_Monitor_Handle = NULL;
TaskHandle_t Button_2_Monitor_Handle = NULL;
TaskHandle_t Periodic_Transmitter_Handle = NULL;
TaskHandle_t Uart_Receiver_Handle = NULL;
TaskHandle_t Load_1_Simulation_Handle = NULL;
TaskHandle_t Load_2_Simulation_Handle = NULL;


void Button_1_Monitor(void* pvparams);
void Button_2_Monitor(void* pvparams);
void Periodic_Transmitter(void* pvparams);
void Uart_Receiver(void* pvparams);
void Load_1_Simulation(void* pvparams);
void Load_2_Simulation(void* pvparams);


#define Button_1_Monitor_Period  			50
#define Button_2_Monitor_Period  			50
#define Periodic_Transmitter_Period  	100
#define Uart_Receiver_Period  				20
#define Load_1_Simulation_Period  		10
#define Load_2_Simulation_Period  		100

//will hold the FreeRTOS statistics
char runTimeStatsBuffer[200];


/////////////////////////////////////////////////////////////////////
// will hold the variable that will be used in run-time calculations
uint32_t TaskTraceArr[MAX_TRACE_NUMB];

uint32_t Button_1_Monitor_ExcutionTime ,Button_2_Monitor_ExcutionTime,Periodic_Transmitter_ExcutionTime,
					Uart_Receiver_ExcutionTime , Load_1_Simulation_ExcutionTime , Load_2_Simulation_ExcutionTime;
					
uint8_t CPU_Load,Button_1_Monitor_LoadPercent,Button_2_Monitor_LoadPercent,Periodic_Transmitter_LoadPercent,
				Uart_Receiver_LoadPercent,Load_1_Simulation_LoadPercent,Load_2_Simulation_LoadPercent;

////////////////////////////////////////////
// worst case execution time 
#define WCETcounterThreshold	20		// wait 20 cycle befor start sampling periodic excution time
int WCET_counter =0;
int WCET_Monitor = 0;		// to monitor max periodic tasks load for selecting item = worst case excustion time
/////////////////////////////
/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */
int main( void )
{

	/// creatig queue
	MessageQueue = xQueueCreate(QueueItemNumb , sizeof(Q_Message));
	
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();


    /* Create Tasks here */
	
	
	
	xTaskPeriodicCreate(Button_1_Monitor,
							"Button1",
							100,
							(void*)0,
							0,
							&Button_1_Monitor_Handle,
							Button_1_Monitor_Period);

	xTaskPeriodicCreate(Button_2_Monitor,
							"Button2",
							100,
							(void*)0,
							0,
							&Button_2_Monitor_Handle,
							Button_2_Monitor_Period);


	xTaskPeriodicCreate(Periodic_Transmitter,
							"PeriodicT",
							100,
							(void*)0,
							0,
							&Periodic_Transmitter_Handle,
							Periodic_Transmitter_Period);
							
	
		xTaskPeriodicCreate(Uart_Receiver,
							"UartR",
							100,
							(void*)0,
							0,
							&Uart_Receiver_Handle,
							Uart_Receiver_Period);

	xTaskPeriodicCreate(Load_1_Simulation,
							"Load1",
							100,
							(void*)0,
							0,
							&Load_1_Simulation_Handle,
							Load_1_Simulation_Period);


	xTaskPeriodicCreate(Load_2_Simulation,
							"Load2",
							100,
							(void*)0,
							0,
							&Load_2_Simulation_Handle,
							Load_2_Simulation_Period);

							
							/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

/* Function to reset timer 1 */
void timer1Reset(void)
{
	T1TCR |= 0x2;
	T1TCR &= ~0x2;
}

/* Function to initialize and start timer 1 */
static void configTimer1(void)
{
	T1PR = 100;					///////////////////////
	T1TCR |= 0x1;				// enable
}

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);

	/* Configure GPIO */
	GPIO_init();
	
	/* Config trace timer 1 and read T1TC to get current tick */
	configTimer1();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/




///////////////////////////////////////////////////////////////
// func to monitor Button 1 
// periodicity = 50 ms , excution time in 0,03 us , deadline 50
// it will send string to UART through queue "Button1 is HIGH" and "Button1 is LOW"
// 
void Button_1_Monitor(void* pvparams){
	vTaskSetApplicationTaskTag( NULL, (void*) Button_1_Monitor_Tag );			// giving tag number to the task

	TickType_t currentTickCounter = xTaskGetTickCount();
	uint32_t a ;
	//////////////////////////////////////////////////////////////////////
	Q_Message Button1Message;
	Button1Message.MessageID =1;
	
	unsigned char Old_PIN_Status_value=0;
	
	for(;;){
		
		//for(a=0;a<= 7000;a++){a=a; }
		
		memset(&(Button1Message.Message),0,Q_MessageLen);    // ZEROING ALL OLD MESSAGE ARRAY
		if(MessageQueue !=0 ){
			if(GPIO_read(PORT_0, PIN0 )== PIN_IS_HIGH){
#if(config_Monitor_PIN_Change == 1)
				if(Old_PIN_Status_value != PIN_IS_HIGH){
								strcpy(Button1Message.Message,"Button1 is HIGH");
								Old_PIN_Status_value = PIN_IS_HIGH;}
#else
				strcpy(Button1Message.Message,"Button1 is HIGH");		
#endif
			}	
			else if(GPIO_read(PORT_0 ,PIN0)== PIN_IS_LOW){
				
#if(config_Monitor_PIN_Change == 1)
				if(Old_PIN_Status_value != PIN_IS_LOW){
								strcpy(Button1Message.Message,"Button1 is LOW");
								Old_PIN_Status_value = PIN_IS_LOW;
				
			}
#else
				strcpy(Button1Message.Message,"Button1 is LOW");
				
#endif
		}
	}
		
		xQueueSend(MessageQueue,(void*)&Button1Message,0);   ///////////////////////////////////////////////////

		vTaskDelayUntil(&currentTickCounter,Button_1_Monitor_Period);
		
		Button_1_Monitor_LoadPercent = getExcutionPercentage(TaskTraceArr[Button_1_Monitor_TotalEcutionTime]);
		Button_1_Monitor_ExcutionTime = TaskTraceArr[Button_1_Monitor_TotalEcutionTime];

// used to calculate worest case excution time and periodic excution time			
#if (configTaskExcutionTimePeriodic ==1)		
{
	#ifdef	Task1_WCET
	if(WCET_counter >= WCETcounterThreshold){
		if(WCET_Monitor < TaskTraceArr[Button_1_Monitor_TotalEcutionTime]){
			WCET_Monitor = TaskTraceArr[Button_1_Monitor_TotalEcutionTime];
			WCET_counter = WCETcounterThreshold;
			}}
	WCET_counter++;
	#endif
	TaskTraceArr[Button_1_Monitor_TotalEcutionTime] =0;
	}

#endif	

	}
}

///////////////////////////////////////////////////////////////
// func to monitor Button 2 
// periodicity = 50 ms , excution time in 0,03 us , deadline 50
// it will send string to UART through queue "Button2 is HIGH" and "Button2 is LOW"
// 
void Button_2_Monitor(void* pvparams){
	vTaskSetApplicationTaskTag( NULL, (void*) Button_2_Monitor_Tag );			// giving tag number to the task
	
	TickType_t currentTickCounter = xTaskGetTickCount();
	uint32_t a ;
	
	Q_Message Button2Message;
	Button2Message.MessageID =2;
	
	unsigned char Old_PIN_Status_value = 0;


	for(;;){
		
		//for(a=0;a<= 12000;a++){a=a; }

		memset(&(Button2Message.Message),0,Q_MessageLen);    // ZEROING ALL OLD MESSAGE ARRAY
		if(MessageQueue !=0 ){
			if(GPIO_read(PORT_0, PIN1 )== PIN_IS_HIGH){
#if(config_Monitor_PIN_Change == 1)
				if(Old_PIN_Status_value != PIN_IS_HIGH){
								strcpy(Button2Message.Message,"Button2 is HIGH");
								Old_PIN_Status_value = PIN_IS_HIGH;}
#else
				strcpy(Button2Message.Message,"Button2 is HIGH");		
#endif
			}	
			else if(GPIO_read(PORT_1 ,PIN1)== PIN_IS_LOW){
#if(config_Monitor_PIN_Change == 1)
				if(Old_PIN_Status_value != PIN_IS_LOW){
								strcpy(Button2Message.Message,"Button2 is LOW");
								Old_PIN_Status_value = PIN_IS_LOW;
								
				}
#else
				strcpy(Button2Message.Message,"Button2 is LOW");

#endif
			}
		}
		
			xQueueSend(MessageQueue,(void*)&Button2Message,0);   ///////////////////////////////////////////////////
	
		
		vTaskDelayUntil(&currentTickCounter,Button_2_Monitor_Period);
		
		Button_2_Monitor_LoadPercent = getExcutionPercentage(TaskTraceArr[Button_2_Monitor_TotalEcutionTime]);
		
		Button_2_Monitor_ExcutionTime = TaskTraceArr[Button_2_Monitor_TotalEcutionTime];
	
// used to calculate worest case excution time and periodic excution time			
#if (configTaskExcutionTimePeriodic ==1)		
{
	#ifdef	Task2_WCET
	if(WCET_counter >= WCETcounterThreshold){
		if(WCET_Monitor < TaskTraceArr[Button_2_Monitor_TotalEcutionTime]){
			WCET_Monitor = TaskTraceArr[Button_2_Monitor_TotalEcutionTime];
			WCET_counter = WCETcounterThreshold;
			}}
	WCET_counter++;
	#endif
	TaskTraceArr[Button_2_Monitor_TotalEcutionTime] =0;
	}

#endif	

	}
}

/////////////////////////////////////////////////////////////////////////////////
// periodic func that will transmit string "Message from Periodic Func"
// to UART receiver function
// periodictity 100 ms , ecutioon time about 10/60 ms , deadline 100
void Periodic_Transmitter(void* pvparams){
	vTaskSetApplicationTaskTag( NULL, (void*) Periodic_Transmitter_Tag );			// giving tag number to the task
	
	TickType_t currentTickCounter = xTaskGetTickCount();
	uint32_t a ;
	
	//////////////////////////////////////////////////////////////////////
	Q_Message PeriodicMessage;
	PeriodicMessage.MessageID =3;
	
	for(;;){
		
		for(a=0;a<= 1000;a++){a=a; }				//some load

		
		if(MessageQueue !=0 ){
			memset(PeriodicMessage.Message,0,Q_MessageLen);    // ZEROING ALL OLD MESSAGE ARRAY
					strcpy(PeriodicMessage.Message,"Message from Periodic Func");
					xQueueSend(MessageQueue,(void*)&PeriodicMessage,0); 
		}	
		

		vTaskDelayUntil(&currentTickCounter,Periodic_Transmitter_Period);

	
		Periodic_Transmitter_LoadPercent = getExcutionPercentage(TaskTraceArr[Periodic_Transmitter_TotalEcutionTime]);
		
		Periodic_Transmitter_ExcutionTime = TaskTraceArr[Periodic_Transmitter_TotalEcutionTime];

// used to calculate worest case excution time and periodic excution time			
#if (configTaskExcutionTimePeriodic ==1)		
{
	#ifdef	Task3_WCET
	if(WCET_counter >= WCETcounterThreshold){
		if(WCET_Monitor < TaskTraceArr[Periodic_Transmitter_TotalEcutionTime]){
			WCET_Monitor = TaskTraceArr[Periodic_Transmitter_TotalEcutionTime];
			WCET_counter = WCETcounterThreshold;
			}}
	WCET_counter++;
	#endif
	TaskTraceArr[Periodic_Transmitter_TotalEcutionTime] =0;
	}

#endif	

	}
}

//////////////////////////////////////////////////////////////////////////////////
//

char testingqueue;			// VARIABLE TO MONITOR NUMBER OF QUEUE WAITING MESSAGES
//  periodicity 20 ms , dead line = 20 ms  excution time varies according to messeges in queue because we will wait untill 
//  slow uart send the data  ecution time will be retrived from var WCET_Monitor 
void Uart_Receiver(void* pvparams){
	vTaskSetApplicationTaskTag( NULL, (void*) Uart_Receiver_Tag );			// giving tag number to the task
	
	Q_Message ReceivedMessage;
	
	TickType_t currentTickCounter = xTaskGetTickCount();
	uint32_t a ;
	for(;;){
		
		//for(a=0;a<= 2000;a++){a=a; }
		a= 0;
#if (PrintFreeRTOS_Statistics != 1 || PrintUART_RECEIVER == 1)	
		{
			while(xQueueReceive(MessageQueue,(void*)&ReceivedMessage,0)== pdTRUE){ // we will wait untill all queue messages sent to UART
			
			testingqueue = uxQueueMessagesWaiting( MessageQueue);		// monitoring number of queue messages waiting to be received
			
			if	(ReceivedMessage.MessageID==1){ 
				xSerialPutChar('\n');
				//vSerialPutString(" ", 1);
				while (!vSerialPutString(ReceivedMessage.Message, Q_MessageLen));}
			if	(ReceivedMessage.MessageID==2){
				//xSerialPutChar('\n');
				vSerialPutString(" ", 1);
				while (!vSerialPutString(ReceivedMessage.Message, Q_MessageLen));}
			if	(ReceivedMessage.MessageID==3){
				//xSerialPutChar('\n');
				vSerialPutString(" ", 1);
				while (!vSerialPutString(ReceivedMessage.Message, Q_MessageLen));}
			}
		}	
#else
		{
			for(a=0;a<= 33300;a++){a=a; }			//dummy work to load cpu for 5 ms Depends on WCET
		}
		
#endif	
		
		vTaskDelayUntil(&currentTickCounter,Uart_Receiver_Period);		// periodicity 20 ms
		
		Uart_Receiver_LoadPercent = getExcutionPercentage(TaskTraceArr[Uart_Receiver_TotalEcutionTime]);
		Uart_Receiver_ExcutionTime = TaskTraceArr[Uart_Receiver_TotalEcutionTime];
		
// used to calculate worest case excution time and periodic excution time	 		
#if (configTaskExcutionTimePeriodic ==1)		
{
	#ifdef	Task4_WCET
	if(WCET_counter >= WCETcounterThreshold){
		if(WCET_Monitor < TaskTraceArr[Uart_Receiver_TotalEcutionTime]){
			WCET_Monitor = TaskTraceArr[Uart_Receiver_TotalEcutionTime];
			WCET_counter = WCETcounterThreshold;
			}}
	WCET_counter++;
	#endif
	
	TaskTraceArr[Uart_Receiver_TotalEcutionTime] =0;
	}

#endif	

	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// Heavy load simulator function which have P = 10 ms, Ecution time = 5 ms , Deadline = 10 MS
// this task will utilize 50% from cpu load
void Load_1_Simulation(void* pvparams){
	vTaskSetApplicationTaskTag( NULL, (void*) Load_1_Simulation_Tag );			// giving tag number to the task
	
	TickType_t currentTickCounter = xTaskGetTickCount();
	uint32_t a ;
	for(;;){
		
		for(a=0;a<= 33300;a++){a=a; }			//dummy work to load cpu for 5 ms


		
		vTaskDelayUntil(&currentTickCounter,Load_1_Simulation_Period);
		
		Load_1_Simulation_LoadPercent = getExcutionPercentage(TaskTraceArr[Load_1_Simulation_TotalEcutionTime]);
		
		Load_1_Simulation_ExcutionTime = TaskTraceArr[Load_1_Simulation_TotalEcutionTime];
		
// used to calculate worest case excution time and periodic excution time		
#if (configTaskExcutionTimePeriodic ==1)		
{
	#ifdef	Task5_WCET
	if(WCET_counter >= WCETcounterThreshold){
		if(WCET_Monitor < TaskTraceArr[Load_1_Simulation_TotalEcutionTime]){
			WCET_Monitor = TaskTraceArr[Load_1_Simulation_TotalEcutionTime];
			WCET_counter = WCETcounterThreshold;
			}}
	WCET_counter++;
	#endif
	TaskTraceArr[Load_1_Simulation_TotalEcutionTime] =0;
	}

#endif	

	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Heavy load simulator function which have P = 100 ms, Ecution time = 12 ms , Deadline = 100 MS
//
void Load_2_Simulation(void* pvparams){
	vTaskSetApplicationTaskTag( NULL, (void*) Load_2_Simulation_Tag );			// giving tag number to the task
	
	TickType_t currentTickCounter = xTaskGetTickCount();
	uint32_t a ;
	for(;;){
//////////////////////////////////////////////////////////		
/// Print FreeRTOS statistics on UART terminal
#if (PrintFreeRTOS_Statistics ==1)	
		
				vTaskGetRunTimeStats(runTimeStatsBuffer);
			  vSerialPutString(runTimeStatsBuffer, 200);
				vSerialPutString(" ", 1);
				xSerialPutChar('\n');		
#endif	
	////////////////////////////////////////////////////////////////////////
		
		CPU_Load = 100 - getExcutionPercentage(TaskTraceArr[Idle_TotalEcutionTime]);  // get CPU load from Idle task
		
		for(a=0;a<= 80000;a++){a=a; }				// loading CPU FOR 12 MS

		vTaskDelayUntil(&currentTickCounter,Load_2_Simulation_Period);			// periodicity 100 ms 
		
		// monitoring system in real time
		Load_2_Simulation_LoadPercent = getExcutionPercentage(TaskTraceArr[Load_2_Simulation_TotalEcutionTime]);
		Load_2_Simulation_ExcutionTime = TaskTraceArr[Load_2_Simulation_TotalEcutionTime];
		
// used to calculate worest excution time and periodic excution time
#if (configTaskExcutionTimePeriodic ==1)		
{
	#ifdef	Task6_WCET
	if(WCET_counter >= WCETcounterThreshold){
		if(WCET_Monitor < TaskTraceArr[Load_2_Simulation_TotalEcutionTime]){
			WCET_Monitor = TaskTraceArr[Load_2_Simulation_TotalEcutionTime];
			WCET_counter = WCETcounterThreshold;
			}}
	WCET_counter++;
	#endif

	TaskTraceArr[Load_2_Simulation_TotalEcutionTime] =0;
	}

#endif	

	}
}


///////////////////////////////////////////////////////////////////
// display ticks on logic analyser
void vApplicationTickHook(){
	GPIO_write(TaskMonitor,Tick_PIN,PIN_IS_HIGH);
	GPIO_write(TaskMonitor,Tick_PIN,PIN_IS_LOW);
}



//////////////////////////////////////////////////////////////////
// return excution time percentage in int form
uint8_t getExcutionPercentage(uint32_t var){
	return ((var*100)/T1TC);
}
