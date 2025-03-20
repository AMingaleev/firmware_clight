#include "adc-driver.h"

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_adc.h"
#include "stm32f1xx_hal_dma.h"

#define SPEED_ADC     (ADC1)
#define SPEED_GPIO    (GPIOA)
#define SPEED_PIN     (GPIO_PIN_0)

#define ADC_MAX_VALUE           (4090)
#define ADC_MIN_VALUE           (1980)
#define PERIOD_MAX              (100)
#define PERIOD_MIN              (10)

#define DMA_CHANNEL_COUNT   (1)

static DMA_HandleTypeDef dma;
static ADC_HandleTypeDef adc;
static ADC_ChannelConfTypeDef adc_config;
static uint32_t adc_value = 0;     // 
//static uint32_t speed_value = 0;   // 
static uint8_t  adc_flag = 0;

static void ADC_Init(void);
static void DMA_HalfCallback(DMA_HandleTypeDef *hdma);
static void DMA_CpltCallback(DMA_HandleTypeDef *hdma);

static void DMA_HalfCallback(DMA_HandleTypeDef *hdma){
  __NOP();
}
static void DMA_CpltCallback(DMA_HandleTypeDef *hdma){
  __NOP();
}

static void ADC_Init(void){	
    __HAL_RCC_GPIOA_CLK_ENABLE();   // GPIOA
    __HAL_RCC_ADC1_CLK_ENABLE();    //  
    __HAL_RCC_DMA1_CLK_ENABLE();    // DMA

// GPIO PA0
    GPIO_InitTypeDef gpio;
    gpio.Pin    = SPEED_PIN;
    gpio.Mode   = GPIO_MODE_ANALOG;
    gpio.Pull   = GPIO_NOPULL;
    gpio.Speed  = GPIO_SPEED_LOW;
    HAL_GPIO_Init(SPEED_GPIO, &gpio);

// DMA
  dma.Instance                  = DMA1_Channel1;
  dma.Init.Direction            = DMA_PERIPH_TO_MEMORY;
  dma.Init.MemDataAlignment     = DMA_MDATAALIGN_HALFWORD;
  dma.Init.MemInc               = DMA_MINC_ENABLE;
  dma.Init.Mode                 = DMA_NORMAL;
  dma.Init.PeriphDataAlignment  = DMA_PDATAALIGN_HALFWORD;
  dma.Init.PeriphInc            = DMA_PINC_DISABLE;
  dma.Init.Priority             = DMA_PRIORITY_LOW;
  dma.XferCpltCallback          = &DMA_CpltCallback;
  dma.XferHalfCpltCallback      = &DMA_HalfCallback;
  HAL_DMA_Init(&dma);

// 
  adc.Instance = ADC1;
  adc.Init.ContinuousConvMode     = ENABLE;
  adc.Init.DataAlign              = ADC_DATAALIGN_RIGHT;
  adc.Init.DiscontinuousConvMode  = DISABLE;
  adc.Init.ExternalTrigConv       = ADC_SOFTWARE_START;
  adc.Init.NbrOfConversion        = 1;
  adc.Init.ScanConvMode           = ADC_SCAN_ENABLE;
  adc.DMA_Handle                  = &dma;
  HAL_ADC_Init(&adc);

  adc_config.Channel = ADC_CHANNEL_0;
  adc_config.Rank = ADC_REGULAR_RANK_1;
  adc_config.SamplingTime = ADC_SAMPLETIME_41CYCLES_5;
  HAL_ADC_ConfigChannel(&adc, &adc_config);

  HAL_ADCEx_Calibration_Start(&adc);

  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);


  HAL_ADC_Start_DMA(&adc, &adc_value, DMA_CHANNEL_COUNT);
}

uint32_t get_ADC_Value(void){
  return adc_value;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
__NOP();
}
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc){
  if (adc.Instance == ADC1) {
    HAL_ADC_Stop_DMA(&adc);
    adc_flag = 1;
  }
}

void DMA1_Channel1_IRQHandler(void){
  HAL_DMA_IRQHandler(&dma);
}

uint8_t is_FlagAdc(void){
  return adc_flag;
}

void Speed_AdcStart(void){
  HAL_ADC_Start_DMA(&adc, &adc_value, DMA_CHANNEL_COUNT);
}

void ADC_Setup(void){
  ADC_Init();
}

uint32_t get_Frequence(void){
    uint32_t x, y, z;
    x =  (adc_value - ADC_MIN_VALUE) * 100;
    y =  (ADC_MAX_VALUE - ADC_MIN_VALUE);
    z = x / y;
    return z;
}

#if LIGHTMOTION_K
uint32_t get_Brightness(void){
    uint32_t x, y, z;
    x =  (adc_value - ADC_MIN_VALUE) * 500;
    y =  (ADC_MAX_VALUE - ADC_MIN_VALUE);
    z = x / y;
    return z;
}
#endif



