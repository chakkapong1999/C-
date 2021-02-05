#include "stm32l1xx.h"
#include "stdio.h"
#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_system.h"
#include "stm32l1xx_ll_utils.h"
#include "stm32l1xx_ll_rcc.h"
#include "stm32l1xx_ll_pwr.h"
#include "stm32l1xx_ll_gpio.h"
#include "stm32l1xx_ll_lcd.h"
#include "stm32l152_glass_lcd.h"

char disp_str[7];

void SystemClock_Config(void);
int main()
{
	SystemClock_Config();
	LCD_GLASS_Init();
	LL_GPIO_InitTypeDef GPIO_InitStruct;
	
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
	
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	//PA0
	GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	LL_GPIO_Init(GPIOA,&GPIO_InitStruct);
	//PC11
	GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	LL_GPIO_Init(GPIOC,&GPIO_InitStruct);
	//PC12
	GPIO_InitStruct.Pin = LL_GPIO_PIN_12;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	LL_GPIO_Init(GPIOC,&GPIO_InitStruct);
	//PA11
	GPIO_InitStruct.Pin = LL_GPIO_PIN_11;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	LL_GPIO_Init(GPIOA,&GPIO_InitStruct);
	//PA12
	GPIO_InitStruct.Pin = LL_GPIO_PIN_12;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	LL_GPIO_Init(GPIOA,&GPIO_InitStruct);
	//PA5
	GPIO_InitStruct.Pin = LL_GPIO_PIN_5;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	LL_GPIO_Init(GPIOA,&GPIO_InitStruct);
	//PB2
	GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	LL_GPIO_Init(GPIOB,&GPIO_InitStruct);
	while(1)
	{
			LCD_GLASS_DisplayString((uint8_t*)disp_str);
			if(LL_GPIO_IsInputPinSet(GPIOA,LL_GPIO_PIN_0))
			{
				LL_GPIO_TogglePin(GPIOA,LL_GPIO_PIN_5);
				LL_GPIO_TogglePin(GPIOB,LL_GPIO_PIN_2);
				LL_GPIO_TogglePin(GPIOA,LL_GPIO_PIN_11);
				LL_GPIO_TogglePin(GPIOA,LL_GPIO_PIN_12);
			}
			if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_11)==0)
				{
					if(LL_GPIO_IsInputPinSet(GPIOA,LL_GPIO_PIN_12)==0)
					{
					sprintf(disp_str,"SW 2-P");
					LCD_GLASS_DisplayString((uint8_t*)disp_str);
					LL_mDelay(10000);
					}
					if(LL_GPIO_IsInputPinSet(GPIOA,LL_GPIO_PIN_11)==1)
					{
					sprintf(disp_str,"SW 4-P");
					LCD_GLASS_DisplayString((uint8_t*)disp_str);
					LL_mDelay(10000);
					}
				}
				
			if(LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_12)==0)
				{
					if(LL_GPIO_IsInputPinSet(GPIOA,LL_GPIO_PIN_12)==0)
					{
					sprintf(disp_str,"SW 1-P");
					LCD_GLASS_DisplayString((uint8_t*)disp_str);
					LL_mDelay(10000);
					}
					if(LL_GPIO_IsInputPinSet(GPIOA,LL_GPIO_PIN_11)==1)
					{
					sprintf(disp_str,"SW 3-P");
					LCD_GLASS_DisplayString((uint8_t*)disp_str);
					LL_mDelay(10000);
					}
				}
				else
				{
					sprintf(disp_str,"SW:OFF");
					LCD_GLASS_DisplayString((uint8_t*)disp_str);
				}
		}
		
	}
/* ==============   BOARD SPECIFIC CONFIGURATION CODE BEGIN    ============== */
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 32000000
  *            HCLK(Hz)                       = 32000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLLMUL                         = 6
  *            PLLDIV                         = 3
  *            Flash Latency(WS)              = 1
  * @retval None
  */
void SystemClock_Config(void)
{
  /* Enable ACC64 access and set FLASH latency */ 
  LL_FLASH_Enable64bitAccess();; 
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

  /* Set Voltage scale1 as MCU will run at 32MHz */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  
  /* Poll VOSF bit of in PWR_CSR. Wait until it is reset to 0 */
  while (LL_PWR_IsActiveFlag_VOSF() != 0)
  {
  };
  
  /* Enable HSI if not already activated*/
  if (LL_RCC_HSI_IsReady() == 0)
  {
    /* HSI configuration and activation */
    LL_RCC_HSI_Enable();
    while(LL_RCC_HSI_IsReady() != 1)
    {
    };
  }
  
	
  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLL_MUL_6, LL_RCC_PLL_DIV_3);

  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };
  
  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };
  
  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 32MHz                             */
  /* This frequency can be calculated through LL RCC macro                          */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ (HSI_VALUE, LL_RCC_PLL_MUL_6, LL_RCC_PLL_DIV_3); */
  LL_Init1msTick(32000000);
  
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(32000000);
}
