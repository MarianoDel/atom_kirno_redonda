/*
 * hard.h
 *
 *  Created on: 28/11/2013
 *      Author: Mariano
 */

#ifndef HARD_H_
#define HARD_H_


//----------- Defines For Configuration -------------
//----------- Hardware Board Version -------------
#define VER_1_0

//-------- Type of Program ----------------
//#define USE_MQTT_LIB
//#define USE_GPS
//#define USE_GSM
//#define USE_GSM_GATEWAY

//#define WIFI_TO_MQTT_BROKER
//#define USE_CERT_PROGRAM
#define USE_PROD_PROGRAM

//-------- End Of Defines For Configuration ------

#ifdef VER_1_0
//GPIOA pin0	SYNC
#define SYNC	((GPIOA->IDR & 0x0001) != 0)

//GPIOA pin1	Light_Sense
//#define LIGHT	((GPIOA->IDR & 0x0002) != 0)	//en adc

//GPIOA pin2
//GPIOA pin3	usart2 tx rx (para debug)

//GPIOA pin4
//GPIOA pin5

//GPIOA pin6	para PWM_CH1

//GPIOA pin7
//GPIOB pin0
//GPIOB pin1

//GPIOA pin8
//GPIOA pin9
//GPIOA pin10

//GPIOA pin11
#define RELAY ((GPIOA->ODR & 0x0800) != 0)
#define RELAY_ON	GPIOA->BSRR = 0x00000800
#define RELAY_OFF GPIOA->BSRR = 0x08000000

//GPIOA pin12
#define LED ((GPIOA->ODR & 0x1000) != 0)
#define LED_ON	GPIOA->BSRR = 0x00001000
#define LED_OFF GPIOA->BSRR = 0x10000000

//GPIOA pin13
//GPIOA pin14
//GPIOA pin15
//GPIOB pin3
//GPIOB pin4
//GPIOB pin5
//GPIOB pin6
//GPIOB pin7

#endif	//


//#define DOOR_ROOF	200
//#define DOOR_THRESH	180


//ESTADOS DEL PROGRAMA PRINCIPAL
#define MAIN_INIT				0
#define MAIN_INIT_1				1
#define MAIN_SENDING_CONF		2
#define MAIN_WAIT_CONNECT_0		3
#define MAIN_WAIT_CONNECT_1		4
#define MAIN_WAIT_CONNECT_2		5
#define MAIN_READING_TCP		6
#define MAIN_TRANSPARENT		7
#define MAIN_AT_CONFIG_2B		8
#define MAIN_ERROR				9

#define MAIN_STAND_ALONE		10
#define MAIN_GROUPED			11
#define MAIN_NETWORKED			12
#define MAIN_NETWORKED_1		13
#define MAIN_IN_MAIN_MENU		14


#define SIZEOF_DATA1	512
#define SIZEOF_DATA		256
#define SIZEOF_DATA512	SIZEOF_DATA1
#define SIZEOF_DATA256	SIZEOF_DATA
#define SIZEOF_BUFFTCP	SIZEOF_DATA





// ------- de los switches -------
//void UpdateSwitches (void);
//unsigned char CheckS1 (void);
//unsigned char CheckS2 (void);
//void UpdateACSwitch (void);
//unsigned char CheckACSw (void);


#endif /* HARD_H_ */
