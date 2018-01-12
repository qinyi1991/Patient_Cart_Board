#ifndef  MODBUS_H
#define  MODBUS_H

//#include <includes.h>	/* ���ļ������˱����.h�ļ� */
#include <modus_Controltargetconf.h>
#include <bsp.h>

//��Ȧ�ͼĴ�����ַ�趨
//u16Ϊ�ο�
#define MAX_REG_ADR         251     //0-200
#define MAX_KEEPREG_ADR     100     //0-100 ���ּĴ���
//u8Ϊ�ο�
#define MAX_COIL_ADR        800     //0-800
#define MAX_KEEPCOIL_ADR    400     //0-400 ������Ȧ
//-----------------------------------------------------------------
//	Modbus eeprom
//-----------------------------------------------------------------
#define   ModbusCoilAdd       400	   //400-800
#define   ModbusRegAdd        0             //0-400
#define   OperateAdd(a,b)     (a+b)

//��ַ0��ʹ��
#define   ModbusAdd(n)   ((n*2)-1)  

typedef  struct 
{
	CPU_INT16U       Add;
	CPU_INT16U       num;

} Coil_Dis;

typedef  struct 
{
	CPU_INT16U   Add;
	CPU_INT16U   num;

} Regis_Dis;

extern CPU_INT08U	receCount;	            //���յ����ֽڸ�
extern CPU_INT08U  sendBuf[100],receBuf[100];   //���ͽ��ջ���
extern CPU_INT08U  redrxBuf[20];
extern CPU_INT08U	redrxCount;	            //���յ����ֽڸ�

extern CPU_INT08U	receTimeOut;                //���ճ�ʱ

extern CPU_INT08U  CoilData[MAX_COIL_ADR];              //modbus ��Ȧ״̬   1--400 ����EEPRAM
extern CPU_INT16S  RegData[MAX_REG_ADR];	             //modbus �Ĵ���״̬ 1-200  ����EEPRAM

extern Coil_Dis   Coil;
extern Regis_Dis  Rdis;
/*======================================================================================================*/

void beginSend(CPU_INT16U sendCount);
void checkComm0Modbus(void);
void readCoil(void);
void readRegisters(void);
void forceSingleCoil(void);
void presetSingleRegister(void);
void presetMultipleRegisters(void);
void forceMultipleCoils(void);
CPU_INT16U SaveRegisterToEEPROM(CPU_INT16U Addr);
CPU_INT16U Write_4byte(void *Add,void *d);

#endif



