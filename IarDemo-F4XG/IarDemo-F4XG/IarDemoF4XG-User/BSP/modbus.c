#include <modbus.h>
#include "app_serial.h"
#include  <stm32f4xx.h>

extern  I2C_HandleTypeDef  I2cHandle; //ʵ����I2cHandle
extern  UART_HandleTypeDef UartHandle;
/*======================================================================================================*/
CPU_INT08U	localAddr =1;	          //��Ƭ�����ư�ĵ�
CPU_INT08U	receCount;	          //���յ����ֽڸ�
CPU_INT08U      sendBuf[100],receBuf[100]; //���ͽ��ջ���
CPU_INT08U      redrxBuf[20];
CPU_INT08U	redrxCount;	          //���յ����ֽڸ�
CPU_INT08U	checkoutError;	          // ==2 żУ���  
CPU_INT08U	receTimeOut;              //���ճ�ʱ
/*======================================================================================================*/

Coil_Dis   Coil;
Regis_Dis  Rdis;

/*======================================================================================================*/
CPU_INT08U   CoilData[MAX_COIL_ADR];        //modbus ��Ȧ״̬   1--400 ����EEPRAM
CPU_INT16S   RegData[MAX_REG_ADR];	    //modbus �Ĵ���״̬ 1-200  ����EEPRAM
 
/*======================================================================================================*/
//�ֵ�ַ 0 - 255 (ֻȡ��8λ)
//λ��ַ 0 - 255 (ֻȡ��8λ)


/* CRC ��λ�ֽ�ֵ�� */ 
const CPU_INT08U  auchCRCHi[] = { 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 
} ; 

/* CRC��λ�ֽ�ֵ��*/ 
const CPU_INT08U  auchCRCLo[] = { 
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
0x43, 0x83, 0x41, 0x81, 0x80, 0x40 
} ;

/***************************************************************************
;** ��������: void ReceiveOver(void)
;** ��������:  ���ճ�ʱ
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   
;** �ա�  ��: 2014-7-19
;** �޸�ԭ��
;** ˵    ��: �ο�
;***************************************************************************/
void ReceiveOver(void)
{
        if(receTimeOut>0)
        {
              receTimeOut--; //&& UartHandle.RxXferCount<8
              if(receTimeOut==0 )   //�ж�ͨѶ�����Ƿ�ʱ
              {
                  receCount = 0;                   //�����յ�ַƫ�ƼĴ�������
                  checkoutError = 0;	           //У�����
                // UartHandle.RxXferCount=8;
                  UartHandle.pRxBuffPtr=(uint8_t*)receBuf;
              }
        }
} 
/***************************************************************************
;** ��������:  
;** ��������:  
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   
;** �ա�  ��: 2014-7-19
;** �޸�ԭ��
;** ˵    ��:
;***************************************************************************/
CPU_INT16U crc16(CPU_INT08U *puchMsg, CPU_INT16U usDataLen) 
{ 
	CPU_INT08U  uchCRCHi = 0xFF ; /* ��CRC�ֽڳ�ʼ�� */ 
	CPU_INT08U  uchCRCLo = 0xFF ; /* ��CRC �ֽڳ�ʼ�� */ 
	CPU_INT32U uIndex ;        /* CRCѭ���е����� */ 

	while (usDataLen--) /* ������Ϣ������ */ 
	{ 
		uIndex = uchCRCHi ^ *puchMsg++ ; /* ����CRC */ 
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ; 
		uchCRCLo = auchCRCLo[uIndex] ; 
	} 
	return (uchCRCHi << 8 | uchCRCLo) ; 
}

/***************************************************************************
;** ��������:  
;** ��������:  ��ʼ����
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   
;** �ա�  ��:  2014-7-19
;** �޸�ԭ��
;** ˵    ��:
;***************************************************************************/
void beginSend(CPU_INT16U sendCount)
{
    if(HAL_UART_Transmit_IT(&UartHandle, (uint8_t*)sendBuf, sendCount+1)!= HAL_OK){ }
    receCount = 0;  
    checkoutError = 0;
}

/***************************************************************************
;** ��������:  
;** ��������:  ȡ��Ȧ״̬ ����0��ʾ�ɹ�
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   
;** �ա�  ��:  2014-7-19
;** �޸�ԭ��
;** ˵    ��:
;***************************************************************************/
CPU_INT16S getCoilVal(CPU_INT16U addr,CPU_INT16S *tempData)
{
    CPU_INT16S result = 0;
    CPU_INT16U tempAddr;
    CPU_INT08U  *p;
    
    tempAddr = addr & 0xfff;
    p=CoilData;

//    if(tempAddr < MAX_KEEPCOIL_ADR)	 //0--400��ַ д��EEPRAM ModbusCoilAdd == 400 && MAX_KEEPCOIL_ADR== 400
//    {
//      //OperateAdd(ModbusAdd(MAX_REG_ADR)+1,tempAddr)
//      IIC_Read_1Byte(OperateAdd(ModbusAdd(MAX_KEEPREG_ADR+1),tempAddr),p+tempAddr);
//    }  
    
    *tempData = *(p+tempAddr);
	
    return result;
}
/***************************************************************************
;** ��������: 
;** ��������: ����Ȧ״̬  /������01
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   
;** �ա�  ��:  2014-7-19
;** �޸�ԭ��
;** ˵    ��:
;***************************************************************************/
void readCoil(void)
{
    CPU_INT08U  i,k,byteCount,exit=0;
    CPU_INT16U addr,tempAddr,CoilNum,crcData;
    CPU_INT16S tempData=0;
	
    addr = ((CPU_INT16U)(receBuf[2]<<8)) | ((CPU_INT16U)receBuf[3]); //���ַ	
    tempAddr = addr;

    CoilNum = ((CPU_INT16U)(receBuf[4]<<8))|((CPU_INT16U)receBuf[5]);
    byteCount = (CPU_INT08U)((CoilNum+7)/8);         //��ȡ��λ��������N+7��/8��
										
	for(k=0;k<byteCount;k++)//�ֽ�λ��
	{
		sendBuf[k+3] = 0;
		for(i=0;i<8;i++) //ÿ8 ����Ȧ��Ϊһ���ֽڣ����һ��������8 λ��δ���岿����0.ǰ8 ����Ȧ�ڵ�һ���ֽڣ����ַ��С����Ȧ�����λ����������
		{
			getCoilVal(tempAddr,&tempData);			
			sendBuf[k+3] |= (CPU_INT08U)(tempData << i);
			tempAddr++;

			if(tempAddr >= (addr+CoilNum))
			{	//����
				exit = 1;
				break;
			}	
		}
		if(exit == 1)
			break;
	}
	
	sendBuf[0] = localAddr;
	sendBuf[1] = 0x01;	
	sendBuf[2] = byteCount;
	byteCount += 3;
	crcData = crc16(sendBuf,byteCount);	//
	sendBuf[byteCount] = crcData >> 8;
	byteCount++;
	sendBuf[byteCount] = crcData & 0xff;

	beginSend(byteCount);  //���ݷ���	
}

/***************************************************************************
;** ��������:  
;** ��������: ȡ�Ĵ���ֵ ����0��ʾ�ɹ� 
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   
;** �ա�  ��:  2014-7-19
;** �޸�ԭ��
;** ˵    ��:
;***************************************************************************/
CPU_INT16S getRegisterVal(CPU_INT16U addr,CPU_INT16S *tempData,CPU_INT08U Counter)
{
	CPU_INT16U tempAddr;
	CPU_INT16S  *p,result = 0;
	
	tempAddr = addr & 0xfff;//4096
	p = RegData;
        

//	if(tempAddr<MAX_KEEPREG_ADR)		
//	{                                     
//           //1--400 address read in EEPRAM
//           IIC_Read_2Byte(ModbusAdd(tempAddr),p+tempAddr);
//        } 
        *tempData=*(p+tempAddr);

	return result;
}
/***************************************************************************
;** ��������:  
;** ��������:  ���Ĵ���  /������03
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   
;** �ա�  ��:  2014-7-19
;** �޸�ԭ��
;** ˵    ��:
;***************************************************************************/
void readRegisters(void)
{
	CPU_INT16U readCount,byteCount;
        CPU_INT16U addr,tempAddr,crcData;
	//CPU_INT08U  finsh;	//1���  0����
	CPU_INT16U i;
	CPU_INT16S  tempData = 0;	
	
	addr = ((CPU_INT16U)(receBuf[2]<<8)) | ((CPU_INT16U)receBuf[3]); //���ַ	
        tempAddr = addr;

        readCount = ((CPU_INT16U)(receBuf[4]<<8))|((CPU_INT16U)receBuf[5]);//Ҫ���ĸ���	
	byteCount = readCount * 2;
	
	for(i=0;i<byteCount;i+=2,tempAddr++)
	{
		getRegisterVal(tempAddr,&tempData,readCount);				
		sendBuf[i+3] = (CPU_INT08U)(tempData >> 8);				   		
		sendBuf[i+4] = (CPU_INT08U)(tempData & 0xff);			
	}
	
	sendBuf[0] = localAddr;
	sendBuf[1] = 3;
	sendBuf[2] = byteCount;
	byteCount += 3;
	crcData = crc16(sendBuf,byteCount);
	sendBuf[byteCount] = crcData >> 8;
	byteCount++;
	sendBuf[byteCount] = crcData & 0xff;

	beginSend(byteCount);      //��������
}
/***************************************************************************
;** ��������:  
;** ��������: �趨��Ȧ״̬ ����0��ʾ�ɹ� 
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   
;** �ա�  ��:  2014-7-19
;** �޸�ԭ��
;** ˵    ��:
;***************************************************************************/
CPU_INT16S setCoilVal(CPU_INT16U addr,CPU_INT16S tempData)
{
	CPU_INT16S  result = 0;
	CPU_INT16U tempAddr;
        CPU_INT08U  *p;

	p = CoilData;
	tempAddr = addr & 0xfff;
	
	*(p+tempAddr)=(CPU_INT08U)tempData;
        
	if(tempAddr < MAX_KEEPCOIL_ADR)	{ //0--400��ַ д��EEPRAM ModbusCoilAdd == 400
          if(HAL_I2C_Mem_Write_IT(&I2cHandle,0xA0,OperateAdd(ModbusAdd(MAX_KEEPREG_ADR+1),tempAddr),I2C_MEMADD_SIZE_16BIT,p+tempAddr, 1) != HAL_OK)
          {
            /* BSP_I2C1_Error_Handler() function is called in case of error. */
            BSP_I2C1_Error_Handler();
          }
        }
        
	return result;
}
/***************************************************************************
;** ��������:  
;** ��������:  ǿ�Ƶ�����Ȧ /������05
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   
;** �ա�  ��:  2014-7-19
;** �޸�ԭ��
;** ˵    ��:
;***************************************************************************/
void forceSingleCoil(void)
{
	CPU_INT16U addr,tempAddr;
	CPU_INT16S  tempData=0;
	CPU_INT16U onOff;
	CPU_INT08U  i;
	
	addr = ((CPU_INT16U)(receBuf[2]<<8)) | ((CPU_INT16U)receBuf[3]); //���ַ	
        tempAddr = addr;

	onOff = ((CPU_INT16U)(receBuf[4]<<8)) | ((CPU_INT16U)receBuf[5]);//Ҫ���ĸ���		
	if(onOff == 0xff00) 
	{	//��ΪON
		tempData = 1;
	} 
	else if(onOff == 0x0000)
	{	//��ΪOFF
		tempData = 0;
	}

        Coil.Add  = tempAddr;	 //������Ҫ����ĵ�ַ
	Coil.num  = 1;	         //������Ҫ������� 

	setCoilVal(tempAddr,tempData);	
	
	for(i=0;i<receCount;i++)
	{
		sendBuf[i] = receBuf[i];
	}
        
        if(receCount>0)
        {receCount--;}
	beginSend(receCount); //��������	
}

/***************************************************************************
;** ��������:  
;** ��������: д�����Ĵ��� ����0��ʾ�ɹ� 
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   
;** �ա�  ��:  2014-7-19
;** �޸�ԭ��
;** ˵    ��:
;***************************************************************************/
CPU_INT16S SetCoilReg(CPU_INT16U addr,CPU_INT16S tempData)
{
	CPU_INT16S  result = 0,*p;
	static CPU_INT16U tempAddr;
	  
        p = RegData;
	tempAddr = addr & 0xfff;
	*(p+tempAddr)=tempData;


	if(tempAddr<MAX_KEEPREG_ADR){
          if(HAL_I2C_Mem_Write_IT(&I2cHandle,0xA0,ModbusAdd(tempAddr),I2C_MEMADD_SIZE_16BIT,(uint8_t *)(p+tempAddr), 2) != HAL_OK)
          {
            /* BSP_I2C1_Error_Handler() function is called in case of error. */
             BSP_I2C1_Error_Handler();
          }
        }
	return result;
}
/***************************************************************************
;** ��������:  
;** ��������:  д�����Ĵ��� /������06
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   
;** �ա�  ��:  2014-7-19
;** �޸�ԭ��
;** ˵    ��:  
;***************************************************************************/
void WritetSingleRegister(void)
{
	CPU_INT16U addr,tempAddr;
	CPU_INT16S tempData=0;
	CPU_INT16S ReVal;
	CPU_INT08U  i;
	
	addr = ((CPU_INT16U)(receBuf[2]<<8)) | ((CPU_INT16U)receBuf[3]); //���ַ	
        tempAddr = addr;

	ReVal = ((CPU_INT16S)(receBuf[4]<<8)) | ((CPU_INT16S)receBuf[5]);//�Ĵ���ֵ		
	tempData = ReVal;

	Rdis.Add = tempAddr;  //���ص�ַ
	Rdis.num = 1;         //���ؼĴ�������

	SetCoilReg(tempAddr,tempData);	
	
	for(i=0;i<receCount;i++)
	{
		sendBuf[i] = receBuf[i];
	}

        if(receCount>0)
        {receCount--;}
	beginSend(receCount); //��������
}

/***************************************************************************
;** ��������:  
;** ��������: �趨����Ȧ�еĵ�����Ȧ�� ����0��ʾ�ɹ� 
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:  L-W 
;** �ա�  ��:  2014-7-19
;** �޸�ԭ��
;** ˵    ��:
;***************************************************************************/
CPU_INT16U SetMultipleCoilVal(CPU_INT16U addr,CPU_INT08U tempData)
{
	CPU_INT16U  result = 0;
	CPU_INT16U  tempAddr;
        CPU_INT08U   *p;

	p = CoilData;
	tempAddr = addr & 0xfff;
	
	*(p+tempAddr)=tempData;

	if(tempAddr<MAX_KEEPCOIL_ADR){	 //400-800��ַ д��EEPRAM ModbusCoilAdd == 400
          if(HAL_I2C_Mem_Write_IT(&I2cHandle,0xA0,OperateAdd(ModbusAdd(MAX_KEEPREG_ADR+1),tempAddr),I2C_MEMADD_SIZE_16BIT,p+tempAddr, 1) != HAL_OK)
          {
              /* BSP_I2C1_Error_Handler() function is called in case of error. */
              BSP_I2C1_Error_Handler();
          }	
        }
	return result;
}

/***************************************************************************
;** ��������:  
;** ��������:  ǿ�ƶ����Ȧ  /������15
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:  L-W 
;** �ա�  ��:  2014-7-19
;** �޸�ԭ��
;** ˵    ��:
;***************************************************************************/
void ForceMultipleCoils(void)
{
	CPU_INT16U addr,tempAddr,crcData;
	CPU_INT16U setCount;
	CPU_INT08U  byteCount,i,j=0,k,w;

	addr = ((CPU_INT16U)(receBuf[2]<<8)) | ((CPU_INT16U)receBuf[3]); //���ַ	
	tempAddr = addr;

	setCount = ((CPU_INT16U)(receBuf[4]<<8))|((CPU_INT16U)receBuf[5]);//��Ȧ����
	
        byteCount = receBuf[6];	//�ֽ��� 
	
	w = byteCount + 7;	//���ֽ���ǰ�����ֽ��ں�

        Coil.Add  = tempAddr;	//������Ҫ����ĵ�ַ
	Coil.num  = setCount;	//������Ҫ������� 

	for(i=0;i<setCount;i++)
	{				
		k=(receBuf[w]>>j)&0x01;
		SetMultipleCoilVal(tempAddr,k);
		tempAddr++;
		j++;
		if(j>=7)
		{
			j=0;
			w--;   //���ֽ���ǰ�����ֽ��ں�
		}
	}

	sendBuf[0] = localAddr;
	sendBuf[1] = 15;
	sendBuf[2] = addr >> 8;
	sendBuf[3] = addr & 0xff;
	sendBuf[4] = setCount >> 8;
	sendBuf[5] = setCount & 0xff;
	crcData = crc16(sendBuf,6);
	sendBuf[6] = crcData >> 8;
	sendBuf[7] = crcData & 0xff;
	beginSend(7);	
}

/***************************************************************************
;** ��������:  
;** ��������:  ���üĴ���ֵ ����0��ʾ�ɹ�
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   
;** �ա�  ��:  2014-7-19
;** �޸�ԭ��
;** ˵    ��:
;***************************************************************************/

CPU_INT16S setMultiRegisterVal(CPU_INT16U addr,CPU_INT16S tempData,CPU_INT16U counter)
{
	CPU_INT16S result = 0,*p;//,Tdata;
	CPU_INT16U tempAddr;

	p=RegData;
	tempAddr = addr & 0xfff; //4096
	*(p+tempAddr)=tempData;
        
        
	if(tempAddr<MAX_KEEPREG_ADR){  //1--400 address write in EEPRAM ModbusRegAdd ==0
          if(HAL_I2C_Mem_Write_IT(&I2cHandle,0xA0,ModbusAdd(tempAddr),I2C_MEMADD_SIZE_16BIT,(uint8_t *)&tempData, 2) != HAL_OK)
          {
            /* BSP_I2C1_Error_Handler() function is called in case of error. */
            BSP_I2C1_Error_Handler();
          }	
        }
	return result;
}
/***************************************************************************
;** ��������:  
;** ��������:  ���ö���Ĵ���  /������16
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   
;** �ա�  ��:  2014-7-19
;** �޸�ԭ��
;** ˵    ��:
;***************************************************************************/
void presetMultipleRegisters(void)
{
	CPU_INT16U addr,tempAddr,crcData;
	CPU_INT16S  tempData;
	CPU_INT16U setCount,byteCount;
	CPU_INT16U i;

	addr = ((CPU_INT16U)(receBuf[2]<<8)) | ((CPU_INT16U)receBuf[3]); //���ַ	
	tempAddr = addr;

	setCount = ((CPU_INT16U)(receBuf[4]<<8))|((CPU_INT16U)receBuf[5]);//�Ĵ�������
	
         byteCount = receBuf[6];	//�ֽ���  ֵ��N*2
         byteCount = byteCount;

	Rdis.Add = tempAddr;  //���ص�ַ
	Rdis.num = setCount;  //���ؼĴ�������

	for(i=0;i<setCount;i++)
	{
		tempData = (((CPU_INT16S)(receBuf[i*2+7]<<8)) | ((CPU_INT16S)receBuf[i*2+8]));	
		setMultiRegisterVal(tempAddr,tempData,setCount);	
		tempAddr++;		
	}

	sendBuf[0] = localAddr;
	sendBuf[1] = 16;
	sendBuf[2] = addr >> 8;
	sendBuf[3] = addr & 0xff;
	sendBuf[4] = setCount >> 8;
	sendBuf[5] = setCount & 0xff;
	crcData = crc16(sendBuf,6);
	sendBuf[6] = crcData >> 8;
	sendBuf[7] = crcData & 0xff;
	beginSend(7);	
}

/***************************************************************************
;** ��������:  
;** ��������:  ���uart0����
;** �������: 
;** �� �� ֵ: 
;** ȫ�ֱ���: 
;** ����ģ��: 
;** ����  ��:   
;** �ա�  ��:  2014-7-19
;** �޸�ԭ��
;** ˵    ��:
;***************************************************************************/
void checkComm0Modbus(void)
{
	CPU_INT16U crcData,TcrcD;
	CPU_INT16U tempData;
	
	if(receCount > 4)
	{
		switch(receBuf[1])
		{
			case 1://��ȡ��Ȧ״̬(��ȡ�� 16λ����)
			case 3://��ȡ���ּĴ���(һ������)
			case 5://ǿ�Ƶ�����Ȧ
			case 6://���õ����Ĵ���
					if(receCount >= 8)
					{//�������һ������
						//Ӧ�ùرս����ж�						
						if(receBuf[0]==localAddr && checkoutError==0)
						{
                                                        crcData = crc16(receBuf,6);
                                                        TcrcD = (CPU_INT16U)(receBuf[6]<<8) | (CPU_INT16U)receBuf[7];
							if(crcData == TcrcD)
							{ //У����ȷ
                                
								if(receBuf[1] == 1)
								{ //��ȡ��Ȧ״̬(��ȡ�� 16λ����)
									readCoil();	  						
								}
								else if(receBuf[1] == 3)
								{ //��ȡ�Ĵ���(���)
									readRegisters();   	
								}
								else if(receBuf[1] == 5)
								{ //ǿ�Ƶ�����Ȧ
									forceSingleCoil();  								
								}
								else if(receBuf[1] == 6)
								{  //д���Ĵ���
									WritetSingleRegister();	 
								}

							}
						}						
						receCount = 0;	
						checkoutError = 0;
                                                UartHandle.RxXferCount=8;
                                                UartHandle.pRxBuffPtr=(uint8_t*)receBuf;
					}
					break;
			
			case 15://���ö����Ȧ
					tempData = receBuf[6]; 
					tempData += 9;	//���ݸ���
					if(receCount >= tempData)
					{
						if(receBuf[0]==localAddr && checkoutError==0)
						{
							crcData = crc16(receBuf,tempData-2);
							if(crcData == (receBuf[tempData-2]<<8)+ receBuf[tempData-1])
							{
								ForceMultipleCoils();			
							}
						}	
						receCount = 0;  
						checkoutError = 0;
                                                UartHandle.RxXferCount=8;
                                                UartHandle.pRxBuffPtr=(uint8_t*)receBuf;
					}
					break;
			
			case 16://���ö���Ĵ���
					tempData = ((CPU_INT16U)receBuf[4]<<8) | (CPU_INT16U)receBuf[5];
					tempData = tempData * 2;	//���ݸ���
					tempData += 9;
					if(receCount >= tempData)
					{
						if(receBuf[0]==localAddr && checkoutError==0)
						{
                                                        crcData = crc16(receBuf,tempData-2);
							if(crcData == ((CPU_INT16U)receBuf[tempData-2]<<8) | (CPU_INT16U)receBuf[tempData-1])
							{
								presetMultipleRegisters();			
							}
						}	
						receCount = 0;  
						checkoutError = 0;
                                                UartHandle.RxXferCount=8;
                                                UartHandle.pRxBuffPtr=(uint8_t*)receBuf;
					}
					break;
								
			default:
					break;			
		}
	}
        
}







