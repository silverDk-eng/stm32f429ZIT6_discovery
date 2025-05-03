#include <stdio.h>
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t  xTaskHandle1 = NULL;
TaskHandle_t  xTaskHandle2 = NULL;

void vTask1_handler(void *params);
void vTask2_handler(void *params);
static void prvSetupHardware(void);

#ifdef USE_SEMIHOSTING
//used for semi-hosting -> debugging setting needed. - openocd.
extern void initialise_monitor_handles();
#endif

void printmsg(char *msg);
char usr_msg[200];

int main(void)
{
#ifdef USE_SEMIHOSTING
	initialise_monitor_handles();
	printf("This is hello world example code.");
#endif

    // 1. Reset the RCC clock configuration to the default reset state
    // => HSI ON, PLL OFF, HSE OFF, System clock = HSI (16 MHz), cpu_clock = 16 MHz.
    RCC_DeInit();  

    // 2. update the SystemCoreClock variable
    // => SystemCoreClock = 16,000,000; // Set SystemCoreClock to 16 MHz (HSI)
    SystemCoreClockUpdate();     

    prvSetupHardware();

    sprintf(usr_msg, "hello world application start");
    printmsg(usr_msg);
    //3. lets create 2 tasks, task-1 and task-2
    xTaskCreate(vTask1_handler, "Task-1", configMINIMAL_STACK_SIZE, NULL, 2, &xTaskHandle1);
    xTaskCreate(vTask2_handler, "Task-2", configMINIMAL_STACK_SIZE, NULL, 2, &xTaskHandle2);    


    //4. Start the scheduler
    vTaskStartScheduler();
    for(;;);
}

void vTask1_handler(void *params)
{
    while(1)
    {
        sprintf(usr_msg, "hello-world:From Task-1\n");
    	printmsg(usr_msg);
    }
}

void vTask2_handler(void *params)
{
    while(1)
    {
        sprintf(usr_msg, "hello-world:From Task-2\n");
    	printmsg(usr_msg);
    }
}

static void prvSetupHardware(void)
{
	GPIO_InitTypeDef gpio_uart_pins = {0};
	USART_InitTypeDef uart1_init = {0};

	//1. Enable the UART Peripheral Clock
	// AHB1 + APB2 - UART1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//PIN PA2 is UART_TX, PA3 is UART2 RX.
	//2. Alternate function - PA2, PA3.
	//

	gpio_uart_pins.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	gpio_uart_pins.GPIO_Mode = GPIO_Mode_AF;
	gpio_uart_pins.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &gpio_uart_pins);

//	void GPIO_PinAFConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_PinSource, uint8_t GPIO_AF)
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

//	void USART_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct);

	uart1_init.USART_BaudRate = 115200;
	uart1_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	uart1_init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	uart1_init.USART_StopBits = USART_StopBits_1;
	uart1_init.USART_Parity = USART_Parity_No;
	uart1_init.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART1, &uart1_init);
	USART_Cmd(USART1, ENABLE);
}

void printmsg(char *msg)
{
//	void USART_SendData(USART_TypeDef* USARTx, uint16_t Data);
	for(uint32_t i = 0; i < strlen(msg); i++)
	{

//		FlagStatus USART_GetFlagStatus(USART_TypeDef* USARTx, uint16_t USART_FLAG)
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET);
		USART_SendData(USART1, msg[i]);

	}

}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif 
