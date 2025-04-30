#include <stdio.h>
#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"

TaskHandle_t  xTaskHandle1 = NULL;
TaskHandle_t  xTaskHandle2 = NULL;

void vTask1_handler(void *params);
void vTask2_handler(void *params);

//used for semi-hosting -> debugging setting needed. - openocd.
extern void initialise_monitor_handles();

int main(void)
{
	initialise_monitor_handles();

	printf("This is hello world example code.");
    // 1. Reset the RCC clock configuration to the default reset state
    // => HSI ON, PLL OFF, HSE OFF, System clock = HSI (16 MHz), cpu_clock = 16 MHz.
    RCC_DeInit();  

    // 2. update the SystemCoreClock variable
    // => SystemCoreClock = 16,000,000; // Set SystemCoreClock to 16 MHz (HSI)
    SystemCoreClockUpdate();     

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
    	printf("Hello-world: From task1\n");
    }
}

void vTask2_handler(void *params)
{
    while(1)
    {
    	printf("Hello-world: From task2\n");
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
