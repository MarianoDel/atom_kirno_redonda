/* Includes ------------------------------------------------------------------*/
#include "messages.h"
#include "uart.h"
#include "stm32f0x_tim.h"
#include "hard.h"
#include "stm32f0xx.h"

#include <string.h>
#include <stdio.h>


//--- Private typedef ---//
//--- Private define ---//
//--- Private macro ---//

//--- Externals variables ---//
extern unsigned short mains_freq;
extern unsigned short mains_var;
extern unsigned char pwm;
extern unsigned char debug_secs;

extern volatile unsigned char usart1_pckt_ready;

//--- Private variables ---//
unsigned char debug = 0;

//Reception buffer.

//Transmission buffer.

//--- Module functions ---//

//Reviso los mensajes enviados desde la PC
void ProcessMessages (void)
{
	char msg [80];		//TODO: pasar a global
	unsigned char len;

	if (!usart1_pckt_ready)
		return;

	len = ReadUsart1Buffer ((unsigned char *)msg, sizeof(msg));
	usart1_pckt_ready = 0;

	if(!len)
		return;

	//si habia mensages los proceso
	if (!strncmp((const char *)msg, (const char *)"debug on", (sizeof("debug on") - 1)))
	{
		debug = 1;
		return;
	}


	if (!strncmp((const char *)msg, (const char *)"debug off", (sizeof("debug off") - 1)))
	{
		debug = 0;
		return;
	}


	if (!strncmp((const char *)msg, (const char *)"relay on", (sizeof("relay on") - 1)))
	{
		RELAY_ON;
		return;
	}

	if (!strncmp((const char *)msg, (const char *)"relay off", (sizeof("relay off") - 1)))
	{
		RELAY_OFF;
		return;
	}

	if (!strncmp((const char *)msg, (const char *)"pwm,", (sizeof("pwm,") - 1)))
	{
		unsigned short local_pwm = 0;

		//tiene que venir pwm,000 a pwm,255
		if (*(msg + 7) == '\r')
		{
			local_pwm = (*(msg + 4) - 48) * 100 + (*(msg + 4) - 48) * 10 + (*(msg + 4) - 48);
			if (local_pwm > 255)
				Update_TIM3_CH1 (255);
			else
				Update_TIM3_CH1 (local_pwm);
		}
	}
}

void UpdateMessages (void)
{
	char msg [80];		//TODO: pasar a global

	if ((debug) && (!debug_secs))
	{
		debug_secs = 20;
		sprintf (msg, "freq=%i var=%i pwm=%i", mains_freq, mains_var, pwm);
		if (RELAY)
			strcat (msg, " relay on");
		else
			strcat (msg, " relay off");
	}
}
