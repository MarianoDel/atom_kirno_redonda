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
//ESTADOS DEL PROGRAMA PRINCIPAL EN MODO MQTT
#ifdef WIFI_TO_MQTT_BROKER
typedef enum {
  wifi_state_reset = 0,
  wifi_state_ready,
  wifi_state_sending_conf,
  wifi_state_wait_ip,
  wifi_state_wait_ip1,
  wifi_state_idle,
  wifi_state_connecting,
  wifi_state_connected,
  wifi_state_disconnected,
  wifi_state_error,
  wifi_state_socket_close,
  mqtt_socket_create,
  client_conn,
  mqtt_connect,
  mqtt_sub,
  mqtt_pub,
  mqtt_device_control,
  wifi_undefine_state       = 0xFF,
} wifi_state_t;
#endif

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


//--- Temas con el sync de relay
#define TT_DELAYED_OFF		5600
#define TT_DELAYED_ON		6560
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



#endif /* HARD_H_ */
