#include <STC15F2K60S2.H>
#include "usart.h"
#include "intrins.h"
#include "ADC.h"
#include "stdio.h"
void Delay10ms()		//@20.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 1;
	j = 195;
	k = 136;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
void main()
{
	unsigned char PM25,MQ7,MQ135;
	char xdata buf[60];
	EA = 1;
	UsartInit();
	ADC_Init();
	while (1)
	{
		PM25 = GatValue_PM25();
		MQ7 = GatValue_MQ7();
		MQ135 = GatValue_MQ135();
		Delay10ms();
		sprintf(buf,"%d",(int)PM25+1000);
		UsartSendString(buf);
		Delay10ms();
		sprintf(buf,"%d",(int)MQ7+2000);
		UsartSendString(buf);
		Delay10ms();
		sprintf(buf,"%d",(int)MQ135+3000);
		UsartSendString(buf);
	}
}
	