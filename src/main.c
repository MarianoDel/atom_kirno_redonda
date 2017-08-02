/**
  ******************************************************************************
  * @file    Template_2/main.c
  * @author  Nahuel
  * @version V1.0
  * @date    22-August-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Use this template for new projects with stm32f0xx family.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//--- My includes ---//
#include "gpio.h"
#include "tim.h"
#include "uart.h"
#include "hard.h"

#include "core_cm0.h"
#include "adc.h"
#include "flash_program.h"
#include "synchro.h"
#include "messages.h"


//------ EXTERNAL VARIABLES ------//

// ------- Externals of USARTs  ----------
#ifdef USE_USART1
volatile unsigned char tx1buff[SIZEOF_DATA];
volatile unsigned char rx1buff[SIZEOF_DATA];
volatile unsigned char usart1_pckt_ready;
volatile unsigned char usart1_have_data;
#endif

#ifdef USE_USART2
volatile unsigned char tx2buff[SIZEOF_DATA];
volatile unsigned char rx2buff[SIZEOF_DATA];
volatile unsigned char usart2_pckt_ready;
volatile unsigned char usart2_have_data;
#endif

// ------- Externals of Messages -------
//const char __attribute__ ((section (".nombreProg"))) nombrep [20] = {"Redonda Kirno v1.0  "};
unsigned short mains_freq = 0;
unsigned short mains_var = 0;
unsigned char pwm = 0;
unsigned char debug_secs = 0;

// ------- Externals for the timers -------
volatile unsigned short timer_relay = 0;			//para relay default (si no hay synchro)
volatile unsigned short take_temp_sample = 0;
volatile unsigned short tt_take_photo_sample;
volatile unsigned short tt_relay_on_off;

#ifdef ADC_WITH_TEMP_SENSE
volatile unsigned short tt_take_temp_sample;
#endif


//------ GLOBAL VARIABLES ------//
parameters_typedef param_struct;

// ------- for the timers -------
volatile unsigned short wait_ms_var = 0;
volatile unsigned short timer_standby;

#ifdef WITH_HYST
volatile unsigned short secs = 0;		//OJO cuenta en ms
volatile unsigned char hours = 0;
volatile unsigned char minutes = 0;
#endif



//--- Functions in this Moodule ---//
void TimingDelay_Decrement(void);
void Update_PWM (unsigned short);
void UpdatePackets (void);

//-------------------------------------------//
// @brief  Main program.
// @param  None
// @retval None
//------------------------------------------//
int main(void)
{
	unsigned char i,ii;
	unsigned short local_meas;
	main_state_t main_state = MAIN_INIT;
	unsigned short ts_cal1, ts_cal2;
	char s_lcd [64];
#ifdef WITH_HYST
	unsigned short hyst;
#endif
#ifdef WITH_1_TO_10_VOLTS
	unsigned char one_to_ten;
#endif

	//!< At this stage the microcontroller clock setting is already configured,
    //   this is done through SystemInit() function which is called from startup
    //   file (startup_stm32f0xx.s) before to branch to application main.
    //   To reconfigure the default setting of SystemInit() function, refer to
    //   system_stm32f0xx.c file

	//GPIO Configuration.
	GPIO_Config();

	//ACTIVAR SYSTICK TIMER
	if (SysTick_Config(48000))
	{
		while (1)	/* Capture error */
		{
			if (LED)
				LED_OFF;
			else
				LED_ON;

			for (i = 0; i < 255; i++)
			{
				asm (	"nop \n\t"
						"nop \n\t"
						"nop \n\t" );
			}
		}
	}

	//--- Prueba de LED ---//
	// while (1)
	// {
	// 	if (LED)
	// 		LED_OFF;
	// 	else
	// 		LED_ON;
	//
	// 	Wait_ms(1000);
	// }
	//--- Fin Prueba de LED ---//

	//--- PRUEBA CHANNELS PWM
	// TIM_3_Init ();
	// while (1)
	// {
	// 	for (i = 0; i < 255; i++)
	// 	{
	// 		Update_TIM3_CH1 (i);
	// 		// Update_TIM3_CH2 (i);
	// 		// Update_TIM3_CH3 (i);
	// 		// Update_TIM3_CH4 (i);
	//
	// 		Wait_ms(100);
	// 	}
	// }
	//--- FIN PRUEBA CHANNELS PWM

	//--- Prueba de LED con SYNC ---//
	// while (1)
	// {
	// 	if (SYNC)
	// 		LED_ON;
	// 	else
	// 		LED_OFF;
	//
	// }
	//--- Fin Prueba de LED con SYNC ---//

	//--- Prueba de LED con RELAY ---//
	// while (1)
	// {
	// 	if (RELAY)
	// 	{
	// 		LED_OFF;
	// 		RELAY_OFF;
	// 	}
	// 	else
	// 	{
	// 		LED_ON;
	// 		RELAY_ON;
	// 	}
	// 	Wait_ms(5000);
	// }
	//--- Fin Prueba de LED y RELAY ---//

	//--- Prueba de RELAY con SYNC para ajuste ---//
	// TIM_16_Init();
	// LED_OFF;
	// TIM16Enable();
	// while (1)
	// {
	// 	if (!timer_standby)
	// 	{
	// 		if (RelayIsOn())
	// 		{
	// 			LED_OFF;
	// 			RelayOff ();
	// 			timer_standby = 5000;
	// 		}
	// 		else
	// 		{
	// 			LED_ON;
	// 			RelayOn ();
	// 			timer_standby = 30;
	// 		}
	// 	}
	//
	// 	UpdateRelay ();
	// }
	//--- Fin Prueba de RELAY con SYNC para ajuste ---//

	//---------- Prueba USART1 Single Byte --------//
	// USART1Config();
	// while (1)
	// {
	// 	LED_ON;
	// 	Usart1SendSingle('M');
	// 	Wait_ms(100);
	// 	LED_OFF;
	// 	Wait_ms(2900);
	// }
	//---------- Fin Prueba USART1 Single Byte --------//

	//---------- Prueba USART1 Multiple Bytes --------//
	// USART1Config();
   // while( 1 )
   // {
	// 	LED_ON;
	// 	Usart1Send((char *) (const char *) "Kirno debug placa redonda\r\n");
	// 	Wait_ms(100);
	// 	LED_OFF;
	// 	Wait_ms(2900);
   // }
   //---------- Fin Prueba USART1 Single Bytes --------//

	//---------- Prueba USART1 Rx -> Tx Multiple Bytes --------//
	// USART1Config();
	// Usart1Send((char *) (const char *) "\r\nKirno debug placa redonda\r\n");
   // while( 1 )
   // {
	// 	if (usart1_pckt_ready)
	// 	{
	// 		LED_ON;
	// 		usart1_pckt_ready = 0;
	// 		ii = ReadUsart1Buffer (s_lcd, 64);
	// 		if (ii)
	// 		{
	// 			*(s_lcd + ii) = '\0';
	// 			Usart1Send(s_lcd);
	// 		}
	//
	// 	}
	// 	Wait_ms(100);
	// 	LED_OFF;
   // }
   //---------- Fin Prueba USART1 Rx -> Tx Multiple Bytes --------//

	//---------- Prueba PhotoTransistor y USART1  --------//
	// USART1Config();
	// AdcConfig();
	// Usart1Send((char *) (const char *) "\r\nKirno debug placa redonda\r\n");
   // while( 1 )
   // {
	// 	LED_ON;
	// 	local_meas = ReadADC1_SameSampleTime (ADC_CH1);
	// 	sprintf(s_lcd, "medida: %d\r\n", local_meas);
	// 	Usart1Send(s_lcd);
	// 	LED_OFF;
	// 	Wait_ms(1000);
   // }
   //---------- Fin Prueba PhotoTransistor y USART1 --------//

	//---------- Prueba Temperatura y USART1  --------//
	// USART1Config();
	// AdcConfig();		//recordar habilitar sensor en adc.h
	// Usart1Send((char *) (const char *) "\r\nKirno debug placa redonda\r\n");
	//
	// //calibracion de fabrica del sensor
	// ts_cal1 = *((uint16_t*)0x1FFFF7B8);		//30°
	// ts_cal2 = *((uint16_t*)0x1FFFF7C2);
	// sprintf(s_lcd, "ts_cal1_30: %d, ts_cal2_110: %d\r\n", ts_cal1, ts_cal2);
	// Usart1Send(s_lcd);
	// Wait_ms(2000);
	//
   // while( 1 )
   // {
	// 	if (!timer_standby)
	// 	{
	// 		LED_ON;
	// 		local_meas = GetTemp();
	//
	// 		sprintf(s_lcd, "temp: %d, %d\r\n", ConvertTemp(local_meas), ii);
	// 		// sprintf(s_lcd, "temp: %d\r\n", ii);
	// 		if (ii < 99)
	// 			ii++;
	// 		else
	// 			ii = 0;
	//
	// 		Usart1Send(s_lcd);
	// 		timer_standby = 2000;
	// 		LED_OFF;
	// 	}
	// 	UpdateTemp();
   // }
   //---------- Fin Prueba Temperatura y USART1 --------//

	//---------- Inicio Programa de Produccion --------//
	USART1Config();
	AdcConfig();		//recordar habilitar sensor en adc.h

#ifdef WITH_1_TO_10_VOLTS
	TIM_3_Init ();
#endif

	TIM_16_Init();
	TIM16Enable();

	Usart1Send((char *) (const char *) "\r\nKirno Placa Redonda - Basic V1.0\r\n");
	Usart1Send((char *) (const char *) "  Features:\r\n");
#ifdef WITH_1_TO_10_VOLTS
	Usart1Send((char *) (const char *) "  Dimmer 1 to 10V\r\n");
#endif
#ifdef WITH_HYST
	Usart1Send((char *) (const char *) "  Night Hysteresis\r\n");
#endif
#ifdef WITH_TEMP_CONTROL
	Usart1Send((char *) (const char *) "  Temp Control\r\n");
#endif

	timer_standby = 2000;

	while (1)
	{
		switch (main_state)
		{
			case MAIN_INIT:
				RelayOff();
				LED_OFF;
				FillPhotoBuffer();
				FillTempBuffer();
#ifdef WITH_1_TO_10_VOLTS
				Update_TIM3_CH1 (0);
#endif
				main_state = LAMP_OFF;
				break;

			case LAMP_OFF:
				if (!tt_relay_on_off)
				{
					if (GetPhoto() > VOLTAGE_PHOTO_ON)
					{
						main_state = LAMP_ON;
						tt_relay_on_off = 10000;
#ifdef WITH_1_TO_10_VOLTS
						Update_TIM3_CH1 (PWM_MIN);
#endif

						RelayOn();
						LED_ON;
#ifdef WITH_HYST
						hours = 0;
#endif
					}
				}
				break;

			case LAMP_ON:
				if (!tt_relay_on_off)
				{
#ifdef WITH_HYST		//con Hysteresis apaga casi en el mismo punto en el que prende
					hyst = GetHysteresis (hours);
					if (GetPhoto() < (VOLTAGE_PHOTO_ON - hyst))
#else
					if (GetPhoto() < VOLTAGE_PHOTO_OFF)
#endif
					{
						main_state = LAMP_OFF;
#ifdef WITH_1_TO_10_VOLTS
						Update_TIM3_CH1 (0);
#endif
						tt_relay_on_off = 10000;
						RelayOff();
						LED_OFF;
					}
				}

#ifdef WITH_1_TO_10_VOLTS
				if (main_state == LAMP_ON)
				{
					one_to_ten = GetNew1to10 (GetPhoto());
					Update_TIM3_CH1 (one_to_ten);
				}
#endif
				break;

			default:
				main_state = MAIN_INIT;
				break;
		}

		if (!timer_standby)
		{
			sprintf(s_lcd, "temp: %d, photo: %d\r\n", GetTemp(), GetPhoto());
			//sprintf(s_lcd, "temp: %d, photo: %d\r\n", GetTemp(), ReadADC1_SameSampleTime (ADC_CH1));
			Usart1Send(s_lcd);
			timer_standby = 2000;
		}

		//Cosas que no dependen del estado del programa
		UpdateRelay ();
		UpdateTemp();
		UpdatePhotoTransistor();
	}

	//---------- Fin Programa de Procduccion --------//

	return 0;
}

//--- End of Main ---//



void TimingDelay_Decrement(void)		//1ms tick
{
	if (wait_ms_var)
		wait_ms_var--;

	if (timer_standby)
		timer_standby--;

	if (timer_relay)
		timer_relay--;

	if (tt_take_photo_sample)
		tt_take_photo_sample--;

	if (tt_relay_on_off)
		tt_relay_on_off--;

#ifdef ADC_WITH_TEMP_SENSE
	if (tt_take_temp_sample)
		tt_take_temp_sample--;
#endif

#ifdef WITH_HYST
	//cuenta de a 1 minuto
	if (secs > 59999)	//pasaron 1 min
	{
		minutes++;
		secs = 0;
	}
	else
		secs++;

	if (minutes > 60)
	{
		hours++;
		minutes = 0;
	}
#endif
}

//------ EOF -------//
