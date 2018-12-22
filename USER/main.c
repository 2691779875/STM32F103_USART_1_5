#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
 
 
extern u8 SEND1_Ready;  
extern u8 SEND2_Ready;
extern u8 SEND3_Ready;
extern u8 SEND4_Ready;
extern u8 SEND5_Ready;
 
 
void mymemcpy(void *des,void *src,u32 n)  
{  
    u8 *xdes=des;
	u8 *xsrc=src; 
    while(n--)*xdes++=*xsrc++;  
}  
//�����ڴ�
//*s:�ڴ��׵�ַ
//c :Ҫ���õ�ֵ
//count:��Ҫ���õ��ڴ��С(�ֽ�Ϊ��λ)
void mymemset(void *s,u8 c,u32 count)  
{  
    u8 *xs = s;  
    while(count--)*xs++=c;  
}	

 int main(void)
{		
	u8 i=10;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4
	uart_init_1(115200);	 
	uart_init_2(38400);
	uart_init_3(115200); 
////	uart_init_2(115200);
////	uart_init_3(115200);
//	uart_init_4(9600);
//	uart_init_5(9600);
 	LED_Init();			     //LED�˿ڳ�ʼ��
	ZIGBEE_RES=1;
	ZIGBEE_SW=1; 
	BLE_WAKEUP=1;  
	BLE_RST=1;
//	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
	LED_RUN=1;
 	while(1)
	{
					if(SEND1_Ready==1)  //����1�յ�����
					{	
               SEND1_Ready=0;
					                
					USART_SendChars(USART2,USART1_RX_BUF,USART1_RecLEN);	
					USART_SendChars(USART3,USART1_RX_BUF,USART1_RecLEN);
//					USART_SendChars(UART4,USART1_RX_BUF,USART1_RecLEN);
//					USART_SendChars(UART5,USART1_RX_BUF,USART1_RecLEN);
						
					mymemset(USART1_RX_BUF,0,USART1_LEN);
					USART1_RecLEN=0;  
					LED_RUN=~LED_RUN;
					}
					//delay_ms(i);
					if(SEND2_Ready==1)  //����2�յ�����
					{
                  SEND2_Ready=0;
					
					USART_SendChars(USART1,USART2_RX_BUF,USART2_RecLEN);	
					USART_SendChars(USART3,USART2_RX_BUF,USART2_RecLEN);
//					USART_SendChars(UART4,USART2_RX_BUF,USART2_RecLEN);
//					USART_SendChars(UART5,USART2_RX_BUF,USART2_RecLEN);	
						
					mymemset(USART2_RX_BUF,0,USART2_LEN);
					USART2_RecLEN=0;
					LED_RUN=~LED_RUN;
					}
			//		delay_ms(i);
					if(SEND3_Ready==1)  //����3�յ�����
					{
					USART_SendChars(USART2,USART3_RX_BUF,USART3_RecLEN);	
					USART_SendChars(USART1,USART3_RX_BUF,USART3_RecLEN);
//					USART_SendChars(UART4,USART3_RX_BUF,USART3_RecLEN);
//					USART_SendChars(UART5,USART3_RX_BUF,USART3_RecLEN);
						
					mymemset(USART3_RX_BUF,0,USART3_LEN);
					SEND3_Ready=0;
					USART3_RecLEN=0;
					LED_RUN=~LED_RUN;
					}
//			//		delay_ms(i);
//					if(SEND4_Ready==1)  //����4�յ�����
//					{
//					USART_SendChars(USART2,USART4_RX_BUF,USART4_RecLEN);	
//					USART_SendChars(USART3,USART4_RX_BUF,USART4_RecLEN);
//					USART_SendChars(USART1,USART4_RX_BUF,USART4_RecLEN);
//					USART_SendChars(UART5,USART4_RX_BUF,USART4_RecLEN);
//						
//					mymemset(USART4_RX_BUF,0,USART4_LEN);
//					SEND4_Ready=0;
//					USART4_RecLEN=0;
//					LED_RUN=~LED_RUN;
//					}
//			//		delay_ms(i);
//					if(SEND5_Ready==1)  //����4�յ�����
//					{
//					USART_SendChars(USART2,USART5_RX_BUF,USART5_RecLEN);	
//					USART_SendChars(USART3,USART5_RX_BUF,USART5_RecLEN);
//					USART_SendChars(UART4,USART5_RX_BUF,USART5_RecLEN);
//					USART_SendChars(USART1,USART5_RX_BUF,USART5_RecLEN);
//						
//					mymemset(USART5_RX_BUF,0,USART5_LEN);
//					SEND5_Ready=0;
//					USART5_RecLEN=0;
//					LED_RUN=~LED_RUN;
//					}
			//		delay_ms(i);

	}	
	
}

