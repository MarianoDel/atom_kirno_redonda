/*
 * hard.h
 *
 *  Created on: 28/11/2013
 *      Author: Mariano
 */

#ifndef HARD_H_
#define HARD_H_


//----------- Defines For Configuration -------------
//----------- Hardware Board Version ----------------
#define VER_1_0

//-------- Led Configuration ------------------------
//#define LED_AND_SYNC				//mueve el led siguiendo el sync de la red

//-------- Voltage Conf ------------------------
#define VOLTAGE_PHOTO_OFF	3322
#define VOLTAGE_PHOTO_ON	3722

//-------- Hysteresis Conf ------------------------
#define WITH_HYST

//-------- Type of Program ----------------
//#define USE_MQTT_LIB
//#define USE_GPS
//#define USE_GSM
//#define USE_GSM_GATEWAY

//#define WIFI_TO_MQTT_BROKER
//#define USE_CERT_PROGRAM
#define USE_PROD_PROGRAM

//-------- Others Configurations depending on the formers ------------
#ifdef WITH_HYST
#define HYST_MAX	400
#define HYST_2		340
#define HYST_4		240
#define HYST_6		140
#define HYST_MIN	40
#endif

//-------- End Of Defines For Configuration ------


#ifdef VER_1_0
//GPIOA pin0	SYNC

//GPIOA pin1	Light_Sense
//#define LIGHT	((GPIOA->IDR & 0x0002) != 0)	//en adc

//GPIOA pin2
//GPIOA pin3	usart2 tx rx (para debug)

//GPIOA pin4
//GPIOA pin5
#define RELAY ((GPIOA->ODR & 0x0020) != 0)
#define RELAY_ON	GPIOA->BSRR = 0x00000020
#define RELAY_OFF GPIOA->BSRR = 0x00200000

//GPIOA pin6

//GPIOA pin7
#define SYNC	((GPIOA->IDR & 0x0080) != 0)	//flanco descendente cuando sube la senoidal

//GPIOB pin0
//GPIOB pin1

//GPIOA pin8
//GPIOA pin9
//GPIOA pin10
//GPIOA pin11
//GPIOA pin12
//GPIOA pin13
//GPIOA pin14

//GPIOA pin15
#define SYNCP			((GPIOA->ODR & 0x8000) != 0)
#define SYNCP_ON		GPIOA->BSRR = 0x00008000
#define SYNCP_OFF		GPIOA->BSRR = 0x80000000


//GPIOB pin3
//GPIOB pin4	TIM3_CH1

//GPIOB pin5
//GPIOB pin6
//GPIOB pin7
#define LED ((GPIOB->ODR & 0x0080) != 0)
#define LED_ON	GPIOB->BSRR = 0x00000080
#define LED_OFF GPIOB->BSRR = 0x00800000



#endif	//


//#define DOOR_ROOF	200
//#define DOOR_THRESH	180


//ESTADOS DEL PROGRAMA PRINCIPAL
typedef enum
{
	MAIN_INIT = 0,
  	LAMP_OFF,
 //  	LAMP_TO_ON,
	LAMP_ON,
	// LAMP_TO_OFF

} main_state_t;



#define SIZEOF_DATA1	512
#define SIZEOF_DATA		256
#define SIZEOF_DATA512	SIZEOF_DATA1
#define SIZEOF_DATA256	SIZEOF_DATA


//--- Temas con el sync de relay
//#define TT_DELAYED_OFF		5600		//para relay metab
//#define TT_DELAYED_ON		6560		//para relay metab
#define TT_DELAYED_OFF		3600		//para relay placa redonda
#define TT_DELAYED_ON		4560		//para relay placa redonda
#define TT_RELAY			60		//timeout de espera antes de pegar o despegar el relay

enum Relay_State {

	ST_OFF = 0,
	ST_WAIT_ON,
	ST_DELAYED_ON,
	ST_ON,
	ST_WAIT_OFF,
	ST_DELAYED_OFF

};

/* Module Functions ------------------------------------------------------------*/
void RelayOn (void);
void RelayOff (void);
void UpdateRelay (void);
unsigned char RelayIsOn (void);
unsigned char RelayIsOff (void);
unsigned short GetHysteresis (unsigned char);



#endif /* HARD_H_ */
