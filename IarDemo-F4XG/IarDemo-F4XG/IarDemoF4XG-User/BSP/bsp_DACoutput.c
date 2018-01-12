#include "bsp_DACoutput.h"

DAC_HandleTypeDef    DacHandle;

void HAL_DAC_MspInit(DAC_HandleTypeDef* hdac)
{
    GPIO_InitTypeDef          GPIO_InitStruct;
    static DMA_HandleTypeDef  hdma_dac1;
    
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* DAC Periph clock enable */
    __HAL_RCC_DAC_CLK_ENABLE();
    /* Enable GPIO clock ****************************************/
    DACx_CHANNEL1_GPIO_CLK_ENABLE();
   /* DMA1 clock enable */
    DMAx_CLK_ENABLE();
    
    /*##-2- Configure peripheral GPIO ##########################################*/ 
    /* DAC Channel1 GPIO pin configuration */
    GPIO_InitStruct.Pin = DACx_CHANNEL1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DACx_CHANNEL1_GPIO_PORT, &GPIO_InitStruct);
    
    /*##-3- Configure the DMA streams ##########################################*/
    /* Set the parameters to be configured for Channel1*/
    hdma_dac1.Instance = DACx_DMA_STREAM1;
    
    hdma_dac1.Init.Channel  = DACx_DMA_CHANNEL1;
    hdma_dac1.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_dac1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_dac1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_dac1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_dac1.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_dac1.Init.Mode = DMA_CIRCULAR;
    hdma_dac1.Init.Priority = DMA_PRIORITY_HIGH;
    hdma_dac1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;         
    hdma_dac1.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    hdma_dac1.Init.MemBurst = DMA_MBURST_SINGLE;
    hdma_dac1.Init.PeriphBurst = DMA_PBURST_SINGLE; 

    HAL_DMA_Init(&hdma_dac1);  
  
}