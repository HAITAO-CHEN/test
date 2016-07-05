#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "beep.h"	 	 
#include "key.h"	 	 
#include "exti.h"	 	 
#include "wdg.h" 	 
#include "timer.h"		 	 
#include "tpad.h"
#include "oled.h"		 	 
#include "lcd.h"
#include "usmart.h"	
#include "rtc.h"	 	 
#include "wkup.h"	
#include "adc.h" 	 
//ALIENTEKս��STM32������ʵ��17
//ADC ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾   				 	

int main(void)
{	
	u16 adc[2]={0};
	u16 adcx,adcx2;
	float temp,temp2;
  	Stm32_Clock_Init(9);	//ϵͳʱ������
	uart_init(72,9600);	 	//���ڳ�ʼ��Ϊ9600
	delay_init(72);	   	 	//��ʱ��ʼ�� 
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
 	LCD_Init();			   	//��ʼ��LCD
	usmart_dev.init(72);	//��ʼ��USMART				 	
 //	Adc_Init();		  		//ADC��ʼ��	
	MYDMA_Config(DMA1_Channel1,(u32)&ADC1->DR,(u32)adc,2);//DMA1ͨ��4,����Ϊ����1,�洢��ΪSendBuff,����2.
		Adc_Init();		  		//ADC��ʼ��	
	POINT_COLOR=RED;//��������Ϊ��ɫ 
	LCD_ShowString(60,50,200,16,16,"WarShip STM32");	
	LCD_ShowString(60,70,200,16,16,"ADC TEST");	
	LCD_ShowString(60,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(60,110,200,16,16,"2012/9/7");	
	//��ʾ��ʾ��Ϣ
	POINT_COLOR=BLUE;//��������Ϊ��ɫ
	LCD_ShowString(60,130,200,16,16,"ADC_CH0_VAL:");	      
	LCD_ShowString(60,150,200,16,16,"ADC_CH0_VOL:0.000V");	    
	LCD_ShowString(60,180,200,16,16,"ADC_CH0_VOL:0.000V");	 
	while(1)
	{
// 		adcx=Get_Adc_Average(ADC_CH1,10);
// 		adcx2=Get_Adc_Average(2,10);
		LCD_ShowxNum(156,130,adc[0],4,16,0);//��ʾADC��ֵ
		temp=(float)adc[0]*(3.3/4096);
		temp2=(float)adc[1]*(3.3/4096);
//		if(temp<1.35)	temp-=1.22;
		printf("��ǰ1��ѹΪ��%dV\n",adc[0]);
		printf("��ǰ2��ѹΪ��%dV\n",adc[1]);
		adcx=temp;
		adcx2=temp2;
		LCD_ShowxNum(156,150,adcx,1,16,0);//��ʾ��ѹֵ
		LCD_ShowxNum(156,180,adcx2,1,16,0);//��ʾ��ѹֵ
		temp-=adcx;
		temp*=1000;
		LCD_ShowxNum(172,150,temp,3,16,0X80);
		temp2-=adcx2;
		temp2*=1000;
		LCD_ShowxNum(172,180,temp2,3,16,0X80);
//		printf("��ǰ��ѹΪ��%dV",&adcx);
		LED0=!LED0;
		LED1=!LED1;
		delay_ms(250);	
		delay_ms(350);
	}
}









