#include "serial_bsp_rs485.h"

UART_HandleTypeDef RS485Handle;
uint8_t RFID_Buf[RFID_BUF_SIZE];

CPU_INT08U Get_Crc8(CPU_INT08U *ptr,CPU_INT08U len);
void Decoding_Data(CPU_INT08U *ptr,CPU_INT08U len);

void  BSP_RS485Port_Init (CPU_INT32U  baud_rate)
{
        /* ----------------- INIT USART STRUCT ---------------- */
        RS485Handle.Instance          = USART6;
        
        RS485Handle.Init.BaudRate     = baud_rate;
        RS485Handle.Init.WordLength   = UART_WORDLENGTH_8B;
        RS485Handle.Init.StopBits     = UART_STOPBITS_1;
        RS485Handle.Init.Parity       = UART_PARITY_NONE;
        RS485Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
        RS485Handle.Init.Mode         = UART_MODE_TX_RX;
        RS485Handle.Init.OverSampling = UART_OVERSAMPLING_8;
        
        /* ------------------ SETUP USART6-------------------- */
        if(HAL_UART_Init(&RS485Handle) != HAL_OK){
        }
        RS485_EN=0; //���_����
        if(HAL_UART_Receive_IT(&RS485Handle, (uint8_t*)RFID_Buf, RFID_BUF_SIZE)!= HAL_OK){
         
        }//ʹ�ܽ����ж�  
        
}

void RS485Processing()
{
    CPU_INT08U Data_crc;
//    1���������λ���Ƿ���ȷ
//    2�����֡ͷ�Ƿ���ȷ
//    3���������У��
//    4�����֡β�Ƿ��У��λһ��
//    5����������λ��ID������������ʾ�ڴ������ϡ�
//    6������λΪ12���ֽ�
    switch(1)
    {
        case 1:{if(RFID_Buf[0] != 0xAA) return;} //֡ͷ
        
        // case 2:{if(RFID_Buf[1] != 0xBB) return;} //�����1
        // case 3:{if(RFID_Buf[2] != 0xCC) return;} //�����2
        
        //if(RFID_Buf[10],�û�����λռ8���ֽڣ�������λ11�ֽ�
        case 4:{
            Data_crc     = Get_Crc8(&RFID_Buf[0],11);
            if(Data_crc != RFID_Buf[11]) {return;}  
        }    
        //CRC8У������λ
        case 5:{ Decoding_Data(&RFID_Buf[3],8);}   //����
        
       
    }
  
}

//cec8����У��
CPU_INT08U Get_Crc8(CPU_INT08U *ptr,CPU_INT08U len)
{
	CPU_INT08U crc;
	CPU_INT08U i;
	crc=0;
	while(len--)
	{
		crc^=*ptr++;
		for(i=0;i<8;i++)
		{
			if(crc&0x01)crc=(crc>>1)^0x8C;
			else crc >>= 1;
		}
	}
	return crc;
}

//���ܳ���
void Decoding_Data(CPU_INT08U *ptr,CPU_INT08U len)
{
      CPU_INT16U Data;
      CPU_INT08U i;
      for(i=0;i<len;i++)
      {
              Data=RFID_Buf[2]^(*ptr++);
              Data=Data^RFID_Buf[1];
              RFID_Buf[3+i]=Data;
              Data=0;
      }
      
  
}

void Clear_RFIDBuf(void)
{
    uint8_t  i;
    for(i=0;i<RFID_BUF_SIZE;i++){RFID_Buf[i]=0;}
  
  
  
}