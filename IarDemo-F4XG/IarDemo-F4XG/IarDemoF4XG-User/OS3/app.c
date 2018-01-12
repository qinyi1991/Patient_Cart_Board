/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                           STM3240G-EVAL
*                                         Evaluation Board
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>
#include  <math.h>
#include  <stm32f4xx.h>

#include  <cpu.h>
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <os.h>
#include  <os_app_hooks.h>

#include  <app_cfg.h>
#include  <bsp.h>
#include  <serial_bsp_stm3240x.h>
#include "serial_bsp_rs485.h"
#include "bsp_PWM.h"

Fixture_Status_Flag AppTask_Fixture_Status;
uint16_t Counter_PWM1,Counter_PWM2;

//�ⲿ��������
extern I2C_HandleTypeDef  I2cHandle;  //ʵ����I2cHandle
extern uint8_t RFID_Buf[40];
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim3;

//CPU_INT08U  SF_Version[16]={"MarsCard20170327"};
void PrintStringsToScreen(CPU_INT16S *Ptr_x,CPU_INT08U *ptr,CPU_INT08U size);
void SetOutput(void);
/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  APP_TASK_EQ_0_ITERATION_NBR              16u
#define  APP_TASK_EQ_1_ITERATION_NBR              18u


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

                                                                /* --------------- APPLICATION GLOBALS ---------------- */
static  OS_TCB       AppTaskStartTCB;
static  CPU_STK      AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

                                                                /* --------------- SEMAPHORE TASK TEST --------------- */

static  OS_TCB       AppTaskSerDataHandlingTCB;
static  CPU_STK      AppTaskSerDataHandlingStk[APP_CFG_TASK_SER_STK_SIZE];
///ͨ�ſ�������
OS_TCB   AppTaskSerDataHandlingTCB; 

static  OS_TCB       AppTaskRFIDCardHandlingTCB;
static  CPU_STK      AppTaskRFIDCardHandlingStk[APP_CFG_TASK_RFID_STK_SIZE];

static  OS_TCB       AppTaskPacientCartHandlingTCB;
static  CPU_STK      AppTaskPacientCartHandlingStk[APP_CFG_TASK_CART_STK_SIZE];

#if (OS_CFG_SEM_EN > 0u)
static  OS_SEM       AppTraceSem;
#endif


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskStart (void  *p_arg);
static  void  AppTaskCreate(void);
static  void  AppObjCreate (void);

static  void  AppTaskSerDataHandling     (void  *p_arg);
static  void  AppTaskRFIDCardHandling    (void  *p_arg);
static  void  AppTaskPacientCartHandling (void  *p_arg);

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*
* Notes       : 1) STM32F4xx HAL library initialization:
*                      a) Configures the Flash prefetch, intruction and data caches.
*                      b) Configures the Systick to generate an interrupt. However, the function ,
*                         HAL_InitTick(), that initializes the Systick has been overwritten since Micrium's
*                         RTOS has its own Systick initialization and it is recommended to initialize the
*                         Systick after multitasking has started.
*
*********************************************************************************************************
*/

int main(void)
{
    OS_ERR   err;
    
#if (CPU_CFG_NAME_EN == DEF_ENABLED)
    CPU_ERR  cpu_err;
#endif
	
    HAL_Init();                                                 /* See Note 1.                                          */
    Mem_Init();                                                 /* Initialize Memory Managment Module                   */
    Math_Init();                                                /* Initialize Mathematical Module                       */
	
#if (CPU_CFG_NAME_EN == DEF_ENABLED)
    CPU_NameSet((CPU_CHAR *)"STM32F407IG",
                (CPU_ERR  *)&cpu_err);
#endif

    BSP_IntDisAll();                                            /* Disable all Interrupts.                              */
    
    OSInit(&err);                                               /* Init uC/OS-III.                                      */
    App_OS_SetAllHooks();

    OSTaskCreate(&AppTaskStartTCB,                              /* Create the start task                                */
                  "App Task Start",
                  AppTaskStart,
                  0u,
                  APP_CFG_TASK_START_PRIO,
                  &AppTaskStartStk[0u],
                  AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 10u],
                  APP_CFG_TASK_START_STK_SIZE,
                  0u,
                  0u,
                  0u,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 &err);

    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */

    while (DEF_ON) {                                            /* Should Never Get Here.                               */
        ;
    }
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    OS_ERR      err;
	
   (void)p_arg;
   

    BSP_Init();                                                 /* Initialize BSP functions                             */
    CPU_Init();                                                 /* Initialize the uC/CPU services                       */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

    AppObjCreate();                                             /* Create Applicaiton kernel objects                    */
    AppTaskCreate();                                            /* Create Application tasks                             */
     
    BSP_Ser_Init(115200);
    BSP_RS232_Init(115200);
    BSP_RS485Port_Init(115200); 
    IIC_Configuration();
    BSP_Timer5_CH1_Inital();
    BSP_Timer3_CH2_Inital();
    
    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
        OSTimeDlyHMSM(24u, 0u, 0u, 0u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
    }
}

/*
*********************************************************************************************************
*                                          AppTaskCreate()
*
* Description : Create Application Tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  AppTaskCreate (void)
{
    OS_ERR  os_err;

    //��������һ�����ڴ���ͨ��
    //�봥����ͨ��
    OSTaskCreate(&AppTaskSerDataHandlingTCB,
                 "SerDataHandling",
                  AppTaskSerDataHandling,
                  0,
                  APP_CFG_TASK_SER_PRIO,
                  &AppTaskSerDataHandlingStk[0],
                  AppTaskSerDataHandlingStk[APP_CFG_TASK_SER_STK_SIZE / 10u],
                  APP_CFG_TASK_SER_STK_SIZE,
                  0u,
                  0u,
                  0,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 &os_err);
    //���������������RFID��Ϣ�ɼ� 
    //RS485
    OSTaskCreate(&AppTaskRFIDCardHandlingTCB,
                 "RFIDCardHandling",
                  AppTaskRFIDCardHandling,
                  0,
                  APP_CFG_TASK_RFID_PRIO,
                 &AppTaskRFIDCardHandlingStk[0],
                  AppTaskRFIDCardHandlingStk[APP_CFG_TASK_RFID_STK_SIZE / 10u],
                  APP_CFG_TASK_RFID_STK_SIZE,
                  0u,
                  0u,
                  0,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 &os_err);

    //����������������PacientCart����
    OSTaskCreate(&AppTaskPacientCartHandlingTCB,
                 "PacientCartHandling",
                  AppTaskPacientCartHandling,
                  0,
                  APP_CFG_TASK_CART_PRIO,
                 &AppTaskPacientCartHandlingStk[0],
                  AppTaskPacientCartHandlingStk[APP_CFG_TASK_CART_STK_SIZE / 10u],
                  APP_CFG_TASK_CART_STK_SIZE,
                  0u,
                  0u,
                  0,
                 (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR ),
                 &os_err);
    
}


/*
*********************************************************************************************************
*                                          AppObjCreate()
*
* Description : Create Application Kernel Objects.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/
static  void  AppObjCreate (void)
{
    OS_ERR  os_err;
    
#if (OS_CFG_SEM_EN > 0u)
    OSSemCreate(&AppTraceSem,
                "Trace Lock",
                 1u,
                &os_err);
    
#endif

}

/*
*********************************************************************************************************
*                                          AppTaskSerDataHandling()
*
* Description : Test uC/OS-III objects.
*
* Argument(s) : p_arg is the argument passed to 'AppTaskSerDataHandling' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Caller(s)   : This is a task
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  AppTaskSerDataHandling (void  *p_arg)
{
    OS_ERR  os_err;
    CPU_TS   ts;
    CPU_INT08U   *Ptr_CoilData;
    CPU_INT16S   *Ptr_RegData;
    
    (void)p_arg;
    Ptr_CoilData=CoilData;
    Ptr_RegData=RegData;
    
    //IIC��ȡ
    HAL_I2C_Mem_Read_IT(&I2cHandle,0xA0,1,I2C_MEMADD_SIZE_16BIT,(uint8_t*)&RegData[1], 4); 
    PrintStringsToScreen(Ptr_RegData+NRegisAdd201,"hello word",16);

    while (DEF_TRUE) {
      
          OSTaskSemPend((OS_TICK )0,
                        (OS_OPT  )OS_OPT_PEND_BLOCKING,
                        (CPU_TS *)&ts,
                        (OS_ERR *)&os_err);
          
          
          if(os_err==OS_ERR_NONE){
               //��������
               checkComm0Modbus();   //modbusͨ��Э��
               SetOutput();
               BSP_OUT01=~BSP_OUT01;
               receCount=0;
          }
          
          OSTimeDlyHMSM(0u, 0u, 0u, 10u,
                        OS_OPT_TIME_HMSM_STRICT,
                        &os_err);
    }
}


/*
*********************************************************************************************************
*                                          AppTaskRFIDCardHandling()
*
* Description : Test uC/OS-III objects.
*
* Argument(s) : p_arg is the argument passed to 'AppTaskRFIDCardHandling' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Caller(s)   : This is a task
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  AppTaskRFIDCardHandling (void  *p_arg)
{
    OS_ERR       os_err;
    CPU_INT16S   *Ptr_RegData=RegData;
      
    while (DEF_TRUE) {
               PrintStringsToScreen(Ptr_RegData+NRegisAdd201,RFID_Buf,10);
               BSP_OUT03=~BSP_OUT03;
               OSTimeDlyHMSM(0u, 0u, 1u, 0u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &os_err);
    }
}


/*
*********************************************************************************************************
*                                          AppTaskPacientCartHandling()
*
* Description : Test uC/OS-III objects.
*
* Argument(s) : p_arg is the argument passed to 'AppTaskPacientCartHandling' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Caller(s)   : This is a task
*
* Note(s)     : none.
*********************************************************************************************************
*/ 
void  AppTaskPacientCartHandling (void  *p_arg)
{
      OS_ERR      err;
      CPU_INT16S   *Ptr_RegData=RegData;
      Counter_PWM1=0;
      Counter_PWM2=0;
      AppTask_Fixture_Status.Start_Cart =0;
      //С������ϵͳ
      while (DEF_TRUE) {
        
        BSP_OUT02=~BSP_OUT02;
        
        if(AppTask_Fixture_Status.Start_Cart == 1){
          AppTask_Fixture_Status.Start_Cart =0;
          Counter_PWM1=0;
          Counter_PWM2=0;
          AppTask_Fixture_Status.AutoMode =1;
        }
        
        
        if(AppTask_Fixture_Status.AutoMode == 1 && AppTask_Fixture_Status.Stop_Cart == 0){
          
              // pwm1
              if(Counter_PWM1<*(Ptr_RegData+SRegisAdd1)){Counter_PWM1+=100;}
              else { Counter_PWM1=*(Ptr_RegData+SRegisAdd1); }
              __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,Counter_PWM1);
              *(Ptr_RegData+NRegisAdd101)=Counter_PWM1;
              
              //pwm2
              if(Counter_PWM2<*(Ptr_RegData+SRegisAdd2)){Counter_PWM2+=100;}
              else{ Counter_PWM2 = *(Ptr_RegData+SRegisAdd2); }
              __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,Counter_PWM2);
              *(Ptr_RegData+NRegisAdd102)=Counter_PWM2;
              
        }else if(AppTask_Fixture_Status.Stop_Cart == 1){
              Counter_PWM1=0;
              Counter_PWM2=0;
              __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,Counter_PWM1);
              __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,Counter_PWM2);
              *(Ptr_RegData+NRegisAdd101)=Counter_PWM1;
              *(Ptr_RegData+NRegisAdd102)=Counter_PWM2;
              AppTask_Fixture_Status.AutoMode  =0;
              AppTask_Fixture_Status.Stop_Cart =0;
              
          
        }
        if((Counter_PWM1 == *(Ptr_RegData+SRegisAdd1)) && (Counter_PWM2 == *(Ptr_RegData+SRegisAdd2))){AppTask_Fixture_Status.AutoMode=0;}
       
        
        OSTimeDlyHMSM(0u, 0u, 0u, 50u,
                      OS_OPT_TIME_HMSM_STRICT,
                      &err);
      }
}

//��ʾ���������λ��ָ��,��ʾ�������ָ�룬��ʾλ��
void PrintStringsToScreen(CPU_INT16S *Ptr_x,CPU_INT08U *ptr,CPU_INT08U size)
{
        CPU_INT08U   Chcounter; 
        //��ӡ�汾��Ϣ NRegisAdd121
        for(Chcounter=0;Chcounter<size ;Chcounter+=2){
          //HMI������ ���ֽ���ʾ���ұߣ����ֽ���ʾ�����
          //��������ķ�ת�ߵ��ֽڣ�ʹ�����ݰ�����������������ʾ
          *(Ptr_x+Chcounter/2)=((CPU_INT16U)(*(ptr+1)<<8)|((CPU_INT16U)(*ptr)));
           ptr=ptr+2;                   
        }  
}

//�������Կؼ�
//�������
void SetOutput(void)
{
   CPU_INT08U   *Ptr_CoilData;
   Ptr_CoilData=CoilData;               //���� �����¼� ����
   CPU_INT16S   *Ptr_RegData=RegData;
   if(Coil.Add>0)          
   {      
     //��ť״̬ɨ��------------------------------------------------------------
    switch(Coil.Add)    
    {
        case NCoilAdd401:
        {
              if(*(Ptr_CoilData+NCoilAdd401) == 1)
              {
                 AppTask_Fixture_Status.Clear_RFIDBuf=1;
              }
//              else
//              {
//                 AppTask_Fixture_Status.Clear_RFIDBuf=0;
//                 BSP_OUT16=1;
//                 Clear_RFIDBuf();
//              }
        }break; 
        
        //ǿ�ƶ˿����
        case NCoilAdd402:
        {
              if(*(Ptr_CoilData+NCoilAdd402) == 1)
              {
                    if(*(Ptr_RegData+SRegisAdd1)<50000){ 
                         *(Ptr_RegData+SRegisAdd1)+=1;
                    }
                    else{*(Ptr_RegData+SRegisAdd1)=2500;}
                    AppTask_Fixture_Status.AutoMode =1;
              }
//              else
//              {
//                
//              }
        }break;   
        case NCoilAdd403:
        {
              if(*(Ptr_CoilData+NCoilAdd403) == 1)
              {
                if(*(Ptr_RegData+SRegisAdd1)>2500){
                        *(Ptr_RegData+SRegisAdd1)-=1;
                }
                else{ *(Ptr_RegData+SRegisAdd1)=2500;}
              }
               AppTask_Fixture_Status.AutoMode =1;
//              else
//              {
//                
//                
//              }
        }break; 
         case NCoilAdd404:
        {
              if(*(Ptr_CoilData+NCoilAdd404) == 1)
              {
                if(*(Ptr_RegData+SRegisAdd2)<50000){ *(Ptr_RegData+SRegisAdd2)+=1;}
                else{ *(Ptr_RegData+SRegisAdd2)=50000;}
              }
               AppTask_Fixture_Status.AutoMode =1;
//              else
//              {
//                
//              }
        }break; 
        case NCoilAdd405:
        {
              if(*(Ptr_CoilData+NCoilAdd405) == 1)
              {
                if(*(Ptr_RegData+SRegisAdd2)>2500){ *(Ptr_RegData+SRegisAdd2)-=1;}
                else{ *(Ptr_RegData+SRegisAdd2)=2500;}
              }
               AppTask_Fixture_Status.AutoMode =1;
//              else
//              {
//                
//                
//              }
        }break;
        case NCoilAdd406:
        {
              if(*(Ptr_CoilData+NCoilAdd406) == 1)
              {
                    AppTask_Fixture_Status.Start_Cart = 1;
                
              }
//              else
//              {
//                 
//                
//              }
        }break;
           case NCoilAdd407:
        {
              if(*(Ptr_CoilData+NCoilAdd407) == 1)
              {
                 AppTask_Fixture_Status.Stop_Cart =1;
              }
//              else
//              {
//                 BSP_OUT06=1; 
//                
//              }
        }break;
           case NCoilAdd508:
        {
          if(*(Ptr_CoilData+NCoilAdd508) == 1)
          {
    
          }
          else
          {
            
          }
        }break;
        case NCoilAdd509:
        {
          if(*(Ptr_CoilData+NCoilAdd509) == 1)
          {
           
          }
          else
          {
            
          }
        }break;
           //Counter_PWM1=*(Ptr_RegData+SRegisAdd1);
	  // Counter_PWM2=*(Ptr_RegData+SRegisAdd2);	
     }  
      Coil.Add=0;
      Coil.num=0;
  
   }
}

