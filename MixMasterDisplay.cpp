#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
extern "C"
{
#include "LCD_1in28.h"
#include "Touch_1in28.h"
}

Touch_1IN28_XY XY;

UBYTE flag = 0;

uint DISPLAYTIME = 1000;

void Touch_INT_callback(uint gpio, uint32_t events)
{
  if (XY.mode == 0)
  {
    XY.Gesture = DEV_I2C_Read_Byte(address, 0x01);
    flag = TOUCH_IRQ;
  }
  else
  {
    flag = TOUCH_IRQ;
    XY = Touch_1IN28_Get_Point();
  }
}

int main()
{
  /**
   *Initial Device Setup
   */
  stdio_init_all();

  // Initialise the Wi-Fi chip
  if (cyw43_arch_init())
  {
    printf("Wi-Fi init failed\n");
    return -1;
  }

  // Init Dev Lib from Display
  if (DEV_Module_Init() != 0)
  {
    return -1;
  }

  /**
   * Setup of the Display
   */
  // Set orientation
  LCD_1IN28_Init(HORIZONTAL);

  // Set background Color
  LCD_1IN28_Clear(SH_BACKGROUND);

  // Set PWM for Background
  DEV_SET_PWM(100);

  // Register Callback
  DEV_IRQ_SET(DEV_I2C_INT, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &Touch_INT_callback);

  // Example to turn on the Pico W LED

  XY.mode = 0;
  /* TP Init */
  if (Touch_1IN28_init(XY.mode) == true)
    printf("OK!\r\n");
  else
    printf("NO!\r\n");

  printf("Starting Processing\n");
  while (true)
  {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    LCD_1IN28_Clear(SH_BACKGROUND);
    sleep_ms(DISPLAYTIME);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    LCD_1IN28_Clear(SH_PRIMARY);
    sleep_ms(DISPLAYTIME);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    LCD_1IN28_Clear(SH_DARK);
    sleep_ms(DISPLAYTIME);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    LCD_1IN28_Clear(SH_YELLOW);
    sleep_ms(DISPLAYTIME);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    LCD_1IN28_Clear(SH_S_BACKGROUND);
    sleep_ms(DISPLAYTIME);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    LCD_1IN28_Clear(SH_GREEN);
    // printf("Gesture %d\n", XY.Gesture);
    sleep_ms(DISPLAYTIME);
  }
}
