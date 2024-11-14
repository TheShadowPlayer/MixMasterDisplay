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
    printf("\n\n\n\nSTARTTTT\n\n\n\n");
    // Init
    stdio_init_all();
    sleep_ms(10000);
    printf("Init Application\n");

    // Initialise the Wi-Fi chip
    if (cyw43_arch_init())
    {
        printf("Wi-Fi init failed\n");
        return -1;
    }

    if (DEV_Module_Init() != 0)
    {
        return -1;
    }

    // Setup Display
    printf("Setup Display\n");

    LCD_1IN28_Init(HORIZONTAL);
    LCD_1IN28_Clear(WHITE);

    DEV_SET_PWM(100);

    LCD_1IN28_Clear(BLUE);

    printf("SettingCallback stuff gpio: %d Event: %d\n", DEV_I2C_INT, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL);

    DEV_IRQ_SET(DEV_I2C_INT, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &Touch_INT_callback);

    LCD_1IN28_Clear(GREEN);

    // Example to turn on the Pico W LED
    
    XY.mode = 0;
    /* TP Init */
    if(Touch_1IN28_init(XY.mode) == true)
        printf("OK!\r\n");
    else
        printf("NO!\r\n");

    printf("Starting Processing\n");
    while (true)
    {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        printf("Gesture %d\n", XY.Gesture);
        sleep_ms(25);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(25);
    }
}
