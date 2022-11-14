


#ifndef __TRACEDEF_H__
#define  __TRACEDEF_H__
#include "stdint.h"


#define MAX_TRACE_NUMB	35

#define TaskMonitor		PORT_0

////////////////////////
// INPUTS BUTTONS
#define Button1_in	PIN0
#define Button2_in	PIN1
////////////////////////
// OUTPUT MONITORING 
#define	Button_1_Monitor_PIN				PIN2
#define	Button_2_Monitor_PIN				PIN3
#define	Periodic_Transmitter_PIN		PIN4
#define	Uart_Receiver_PIN						PIN5
#define	Load_1_Simulation_PIN				PIN6
#define	Load_2_Simulation_PIN				PIN7
#define Idle_PIN										PIN8
#define	Tick_PIN										PIN9


///////////////////////////////////////////
// Task Tags that will be traced during running
#define	Idle_Tag										0
#define Button_1_Monitor_Tag				1
#define Button_2_Monitor_Tag				2
#define Periodic_Transmitter_Tag		3
#define Uart_Receiver_Tag						4
#define Load_1_Simulation_Tag				5
#define Load_2_Simulation_Tag				6


//////////////////////////////////////////////////////
// Array elements whcih will trace the time of tasks excution
// present in TaskTraceArr[MAX_TRACE_NUMB]
// Task1
#define Button_1_Monitor_TimeStampIn						0
#define Button_1_Monitor_TimeStampOut						1
#define	Button_1_Monitor_EcutionTime						2
#define	Button_1_Monitor_TotalEcutionTime				3
// Task2
#define Button_2_Monitor_TimeStampIn						4
#define Button_2_Monitor_TimeStampOut						5
#define	Button_2_Monitor_EcutionTime						6
#define	Button_2_Monitor_TotalEcutionTime				7
// Task3
#define Periodic_Transmitter_TimeStampIn				8
#define Periodic_Transmitter_TimeStampOut				9
#define	Periodic_Transmitter_EcutionTime				10
#define	Periodic_Transmitter_TotalEcutionTime		11
//Task4
#define Uart_Receiver_TimeStampIn								12
#define Uart_Receiver_TimeStampOut							13
#define	Uart_Receiver_EcutionTime								14
#define	Uart_Receiver_TotalEcutionTime					15
// Task5
#define Load_1_Simulation_TimeStampIn						16
#define Load_1_Simulation_TimeStampOut					17
#define	Load_1_Simulation_EcutionTime						18
#define	Load_1_Simulation_TotalEcutionTime			19
// Task6
#define Load_2_Simulation_TimeStampIn						20
#define Load_2_Simulation_TimeStampOut					21
#define	Load_2_Simulation_EcutionTime						22
#define	Load_2_Simulation_TotalEcutionTime			23
// Idle Task
#define Idle_TimeStampIn												24
#define Idle_TimeStampOut												25
#define	Idle_EcutionTime												26
#define	Idle_TotalEcutionTime										27
//
#define TotalCPU_Time														30


//////////////////////////////////////////////////////////
// to serial print the statistics of the system uncomment  PrintFreeRRTOS_Statistics
// Note if you enable this macro you will simulate WCET of the void Uart_Receiver(void* pvparams) function without printing
// the vlaues of queue messages on UART terminal
// you will just print FreeRTOS statistics
//#define PrintFreeRTOS_Statistics			1
// if you want to also print the Uart_Receiver(void* pvparams) function parameters un comment this macro
//#define PrintUART_RECEIVER	1
///////////////////////////////////////////////////////////


/////////////////////////////////////////////////////
//#define configTaskExcutionTimePeriodic	1

////////////////////////////////////////////////////////
// uncomment only (one) definition to monitor the variable 	WCET_Monitor
// according to task number the code to get the worst case excustion time will added
// to enable this variable you must define configTaskExcutionTimePeriodic = 1

//#define Task1_WCET
//#define Task2_WCET
//#define Task3_WCET
//#define Task4_WCET
//#define Task5_WCET
//#define Task6_WCET
//#define Task7_WCET
//#define Task8_WCET
//#define Task9_WCET
//#define Task10_WCET
//........................

//////////////////////////////////////////////////////////////
// defined only if you want to monitor the change in pin status
// uncomment to continous send the pin satus
//#define config_Monitor_PIN_Change 1



extern uint32_t TaskTraceArr[MAX_TRACE_NUMB];






#endif