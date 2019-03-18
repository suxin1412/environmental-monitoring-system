#include <STC15F2K60S2.H>
#include "usart.h"
unsigned char RXData;
bit UsartBusy = 0;
void UsartInit(void)		//115200bps@20.000MHz
{
	SCON = 0x50;		//8???,?????
	AUXR |= 0x01;		//??1?????2???????
	AUXR |= 0x04;		//???2???Fosc,?1T
	T2L = 0xD5;		//??????
	T2H = 0xFF;		//??????
	AUXR |= 0x10;		//?????2
	ES  = 1;
}
void UsartEX() interrupt 4
{
	if (RI)
	{
		RXData = SBUF;
		RI = 0;
	}
	if (TI)
	{
		TI = 0;
		UsartBusy = 0;
	}
}
void UsartSend8Bit(unsigned char dat)
{
	while (UsartBusy);
	UsartBusy = 1;
	SBUF = dat;
}
void UsartSendString(unsigned char *str)
{
	while (*str)
	{
		UsartSend8Bit(*str);
		str++;
	}
}