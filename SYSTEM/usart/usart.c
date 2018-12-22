#include "sys.h"
#include "usart.h"	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	

u8 SEND1_Ready;  
u8 SEND2_Ready;
u8 SEND3_Ready;
u8 SEND4_Ready;
u8 SEND5_Ready;

u8 USART1_RX_BUF[USART1_LEN];     //接收缓冲
u8 USART2_RX_BUF[USART2_LEN];
u8 USART3_RX_BUF[USART3_LEN];  
u8 USART4_RX_BUF[USART4_LEN];
u8 USART5_RX_BUF[USART5_LEN]; 

u16 USART1_RecLEN;
u16 USART2_RecLEN;
u16 USART3_RecLEN;
u16 USART4_RecLEN;
u16 USART5_RecLEN;
void uart_init_1(u32 bound){
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=8 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);    //使能总线空闲检测中断 
	USART_Cmd(USART1, ENABLE);                    //使能串口1 
}

void uart_init_2(u32 bound){
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//
   
	//USART1_RX	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);// 

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=7 ;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART2, &USART_InitStructure); //初始化串口
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);    //使能总线空闲检测中断
	USART_Cmd(USART2, ENABLE);                    //使能串口1 
}

void uart_init_3(u32 bound){
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);//
   
	//USART1_RX	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA.10  

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=9 ;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity =USART_Parity_No ;//偶校验位   USART_Parity_Even
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART3, &USART_InitStructure); //初始化串口
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);    //使能总线空闲检测中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
	
	USART_Cmd(USART3, ENABLE);                    //使能串口1 
}


void uart_init_4(u32 bound){
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);//
   
	//USART1_RX	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOA.10  

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=10 ;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity =USART_Parity_No ;//偶校验位   USART_Parity_Even
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(UART4, &USART_InitStructure); //初始化串口
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_ITConfig(UART4,USART_IT_IDLE,ENABLE);    //使能总线空闲检测中断
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启串口接受中断
	
	USART_Cmd(UART4, ENABLE);                    //使能串口1 
}


void uart_init_5(u32 bound){
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	//USART1_TX   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);//
   
	//USART1_RX	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOA.10  

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=11 ;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
	//USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity =USART_Parity_No ;//偶校验位   USART_Parity_Even
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(UART5, &USART_InitStructure); //初始化串口
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_ITConfig(UART5,USART_IT_IDLE,ENABLE);    //使能总线空闲检测中断
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启串口接受中断
	
	USART_Cmd(UART5, ENABLE);                    //使能串口1 
}


void USART1_IRQHandler(void)                	//串口1中断服务程序
{
//	static u8 Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{
		
	USART1_RX_BUF[USART1_RecLEN++]=USART_ReceiveData(USART1);	//读取接收到的数据			
	}
	
	
	
		if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  
			{   
			USART1->SR;
			USART1->DR;
			
			SEND1_Ready=1;
				//	Res=0; 
			}  
	
	
} 

void USART2_IRQHandler(void)                	//串口中断服务程序
{

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
	USART2_RX_BUF[USART2_RecLEN++]=USART_ReceiveData(USART2);	//读取接收到的数据
	}

		if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)  
			{  
					//清除总线空闲中断标志位   
				USART2->SR;
				USART2->DR;
				SEND2_Ready=1;
			}  
	
}	
	
void USART3_IRQHandler(void)                	//串口中断服务程序
{

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
	USART3_RX_BUF[USART3_RecLEN++]=USART_ReceiveData(USART3);	//读取接收到的数据
	}
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)  
    {  
        //清除总线空闲中断标志位   
			USART3->SR;
			USART3->DR;
			SEND3_Ready=1;
    }  
	
}

void UART4_IRQHandler(void)                	//串口中断服务程序
{
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
	USART4_RX_BUF[USART4_RecLEN++]=USART_ReceiveData(UART4);	//读取接收到的数据
	}
	if(USART_GetITStatus(UART4, USART_IT_IDLE) != RESET)  
    {  
        //清除总线空闲中断标志位   
			UART4->SR;
			UART4->DR;
			SEND4_Ready=1;
    }  
	
}

void UART5_IRQHandler(void)                	//串口中断服务程序
{
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
	USART5_RX_BUF[USART5_RecLEN++]=USART_ReceiveData(UART5);	//读取接收到的数据
	}
	if(USART_GetITStatus(UART5, USART_IT_IDLE) != RESET)  
    {  
        //清除总线空闲中断标志位   
			UART5->SR;
			UART5->DR;
			SEND5_Ready=1; 
    }  
	
}
#endif	



void USART_SendByte(USART_TypeDef* USARTx,uint8_t SendByte)
{

    	USARTx->DR = (SendByte & (uint16_t)0x01FF);
  		/* Loop until the end of transmission */
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
}

void USART_SendString(USART_TypeDef* USARTx,const uint8_t* SendString)
{
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	while(* SendString )
												
	{
    	USARTx->DR = (*SendString & (uint16_t)0x01FF);
  		/* Loop until the end of transmission */
		SendString ++;
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}
}

void USART_SendChars(USART_TypeDef* USARTx,const uint8_t* SendChars,uint16_t len)
{
	uint16_t i = 0;
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	for(i = 0; i < len; i++)
	{
    	USARTx->DR = (*(SendChars + i) & (uint16_t)0x01FF);
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}
}


