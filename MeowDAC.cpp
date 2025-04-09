#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
// #include "hardware/dma.h"
#include "hardware/pio.h"
#include "blink.pio.h"
// #include "hardware/timer.h"
// #include "hardware/clocks.h"

uint32_t dataDAC[60] = {
    1048575,1008600,969650,929675,890725,851775,811800,772850,733900,693925,
    654975,616025,576050,537100,497125,458175,419225,379250,340300,301350,
    261375,261375,261375,261375,261375,261375,261375,261375,261375,261375,
    261375,261375,261375,261375,261375,261375,261375,261375,261375,261375,
    280850,320825,359775,399750,438700,477650,517625,556575,595525,635500,
    674450,713400,753375,792325,832300,871250,910200,950175,989125,1028075

};


#define DATA_BASE_PIN 2
#define CLK_PIN 12      
#define SD_PIN 13
#define SD_CLK 16
#define SD_LATCH 17
#define MCLK 18
#define MD 19
#define MLATCH 20

void blink_pin_forever(PIO pio, uint sm, uint offset) {

    blink_program_init(pio, sm, offset, DATA_BASE_PIN, CLK_PIN);
    pio_sm_set_enabled(pio, sm, true);
}

void reset_DAC(PIO pio, uint sm){
    pio_sm_put_blocking(pio, sm, -1);
}

void shift_register_write(){

}

void getButton(){
    
}
int main()
{
    stdio_init_all();

    // setup_default_uart();
    
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    set_sys_clock_khz(100000, true);
    gpio_init(SD_PIN);
    gpio_init(SD_PIN + 1);
    gpio_init(SD_PIN + 2);
    gpio_init(SD_CLK);
    gpio_init(SD_LATCH);

    gpio_init(MCLK);
    gpio_init(MD);
    gpio_init(MLATCH);

    gpio_set_dir(SD_PIN, GPIO_OUT);
    gpio_set_dir(SD_PIN + 1, GPIO_OUT);
    gpio_set_dir(SD_PIN + 2, GPIO_OUT);
    gpio_set_dir(SD_CLK, GPIO_OUT);
    gpio_set_dir(SD_LATCH, GPIO_OUT);

    gpio_set_dir(MCLK, GPIO_OUT);
    gpio_set_dir(MD, GPIO_OUT);
    gpio_set_dir(MLATCH, GPIO_OUT);

    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    // printf("HELLO WORLD\n");
    PIO pio = pio0;
    uint sm;
    uint offset;
    pio_claim_free_sm_and_add_program_for_gpio_range(&blink_program, &pio, &sm, &offset, DATA_BASE_PIN, 11, true);
    // printf("Loaded program at %d\n", offset);

    blink_pin_forever(pio, sm, offset);
    reset_DAC(pio, sm);
    sleep_ms(1000);
    while(true){
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        sleep_ms(50);
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
        for(int i = 0; i < 60; i++){
            pio_sm_put_blocking(pio, sm, dataDAC[i]);
        }
        sleep_ms(50);
    }
    // while (true) {
    //     printf("Hello, world!\n");
    //     sleep_ms(1000);
    // }
    return 0;
}
