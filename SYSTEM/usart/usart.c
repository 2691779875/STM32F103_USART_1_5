#include "sys.h"
#include "usart.h"	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	

u8 SEND1_Ready;  
u8 SEND2_Ready;
u8 SEND3_Ready;
u8 SEND4_Ready;
u8 SEND5_Ready;

u8 USART1_RX_BUF[USART1_LEN];     //���ջ���
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
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
	//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=8 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);    //ʹ�����߿��м���ж� 
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
}

void uart_init_2(u32 bound){
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//
   
	//USART1_RX	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);// 

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=7 ;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART2, &USART_InitStructure); //��ʼ������
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);    //ʹ�����߿��м���ж�
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���1 
}

void uart_init_3(u32 bound){
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//
   
	//USART1_RX	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOA.10  

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=9 ;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity =USART_Parity_No ;//żУ��λ   USART_Parity_Even
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART3, &USART_InitStructure); //��ʼ������
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);    //ʹ�����߿��м���ж�
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���1 
}


void uart_init_4(u32 bound){
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);//
   
	//USART1_RX	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOA.10  

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=10 ;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity =USART_Parity_No ;//żУ��λ   USART_Parity_Even
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(UART4, &USART_InitStructure); //��ʼ������
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_ITConfig(UART4,USART_IT_IDLE,ENABLE);    //ʹ�����߿��м���ж�
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	
	USART_Cmd(UART4, ENABLE);                    //ʹ�ܴ���1 
}


void uart_init_5(u32 bound){
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	//USART1_TX   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);//
   
	//USART1_RX	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIOA.10  

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=11 ;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity =USART_Parity_No ;//żУ��λ   USART_Parity_Even
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(UART5, &USART_InitStructure); //��ʼ������
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_ITConfig(UART5,USART_IT_IDLE,ENABLE);    //ʹ�����߿��м���ж�
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	
	USART_Cmd(UART5, ENABLE);                    //ʹ�ܴ���1 
}


void USART1_IRQHandler(void)                	//����1�жϷ������
{
//	static u8 Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{
		
	USART1_RX_BUF[USART1_RecLEN++]=USART_ReceiveData(USART1);	//��ȡ���յ�������			
	}
	
	
	
		if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  
			{   
			USART1->SR;
			USART1->DR;
			
			SEND1_Ready=1;
				//	Res=0; 
			}  
	
	
} 

void USART2_IRQHandler(void)                	//�����жϷ������
{

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
	USART2_RX_BUF[USART2_RecLEN++]=USART_ReceiveData(USART2);	//��ȡ���յ�������
	}

		if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)  
			{  
					//������߿����жϱ�־λ   
				USART2->SR;
				USART2->DR;
				SEND2_Ready=1;
			}  
	
}	
	
void USART3_IRQHandler(void)                	//�����жϷ������
{

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
	USART3_RX_BUF[USART3_RecLEN++]=USART_ReceiveData(USART3);	//��ȡ���յ�������
	}
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)  
    {  
        //������߿����жϱ�־λ   
			USART3->SR;
			USART3->DR;
			SEND3_Ready=1;
    }  
	
}

void UART4_IRQHandler(void)                	//�����жϷ������
{
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
	USART4_RX_BUF[USART4_RecLEN++]=USART_ReceiveData(UART4);	//��ȡ���յ�������
	}
	if(USART_GetITStatus(UART4, USART_IT_IDLE) != RESET)  
    {  
        //������߿����жϱ�־λ   
			UART4->SR;
			UART4->DR;
			SEND4_Ready=1;
    }  
	
}

void UART5_IRQHandler(void)                	//�����жϷ������
{
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
	USART5_RX_BUF[USART5_RecLEN++]=USART_ReceiveData(UART5);	//��ȡ���յ�������
	}
	if(USART_GetITStatus(UART5, USART_IT_IDLE) != RESET)  
    {  
        //������߿����жϱ�־λ   
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


