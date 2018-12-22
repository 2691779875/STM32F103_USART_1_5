#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define USAR_LEN					1024
#define USART1_LEN  			USAR_LEN  	//
#define USART2_LEN  		  USAR_LEN  	//
#define USART3_LEN  			USAR_LEN  	//
#define USART4_LEN  			USAR_LEN
#define USART5_LEN  			USAR_LEN

extern u16 USART1_RecLEN;
extern u16 USART2_RecLEN;
extern u16 USART3_RecLEN;
extern u16 USART4_RecLEN;
extern u16 USART5_RecLEN;

extern u8 USART1_RX_BUF[USART1_LEN];     //接收缓冲
extern u8 USART2_RX_BUF[USART2_LEN];
extern u8 USART3_RX_BUF[USART3_LEN];  
extern u8 USART4_RX_BUF[USART4_LEN];
extern u8 USART5_RX_BUF[USART5_LEN]; 

#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
void uart_init_1(u32 bound);
void uart_init_2(u32 bound);
void uart_init_3(u32 bound);
void uart_init_4(u32 bound);
void uart_init_5(u32 bound);
void USART_SendByte(USART_TypeDef* USARTx,uint8_t SendByte);
void USART_SendString(USART_TypeDef* USARTx,const uint8_t* SendString);
void USART_SendChars(USART_TypeDef* USARTx,const uint8_t* SendChars,uint16_t len);
#endif




