#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
// #include "hardware/dma.h"
#include "hardware/pio.h"
#include "blink.pio.h"
// #include "hardware/timer.h"
// #include "hardware/clocks.h"

// uint32_t dataDAC[60] = {
//     1048575,1006550,964525,922500,880475,838450,796425,754400,712375,670350,
//     628325,586300,544275,503275,461250,419225,377200,335175,293150,251125,
//     209100,209100,209100,209100,209100,209100,209100,209100,209100,209100,
//     209100,209100,209100,209100,209100,209100,209100,209100,209100,209100,
//     230625,271625,313650,355675,397700,439725,481750,523775,565800,607825,
//     649850,691875,733900,775925,816925,858950,900975,943000,985025,1027050

// };

#define datalen 50
uint32_t dataDAC[datalen] = { // 100 0 100
    // 1048575,995275,943000,890725,838450,786175,733900,680600,628325,576050,
    // 523775,471500,419225,366950,313650,261375,209100,156825,104550,52275,
    // 0,0,0,0,0,0,0,0,0,0,
    // 0,0,0,0,0,0,0,0,0,0,
    // 25625,77900,130175,183475,235750,288025,340300,392575,444850,497125,
    // 550425,602700,654975,707250,759525,811800,864075,917375,969650,1021925

    1048575,1037300,1027050,1016800,1006550,995275,985025,974775,964525,953250,
943000,932750,922500,912250,900975,890725,880475,870225,858950,848700,
838450,838450,838450,838450,838450,838450,838450,838450,838450,838450,
843575,853825,864075,875350,885600,895850,906100,917375,927625,937875,
948125,959400,969650,979900,990150,1000400,1011675,1021925,1032175,1042425

    // 1048575,1006550,964525,922500,880475,838450,796425,754400,712375,670350,
    // 628325,586300,544275,503275,461250,419225,377200,335175,293150,251125,
    // 209100,209100,209100,209100,209100,209100,209100,209100,209100,209100,
    // 209100,209100,209100,209100,209100,209100,209100,209100,209100,209100,
    // 230625,271625,313650,355675,397700,439725,481750,523775,565800,607825,
    // 649850,691875,733900,775925,816925,858950,900975,943000,985025,1027050


    // 1048575,1037300,1027050,1016800,1006550,995275,985025,974775,964525,953250,
    // 943000,932750,922500,912250,900975,890725,880475,870225,858950,848700,
    // 838450,838450,838450,838450,838450,838450,838450,838450,838450,838450,
    // 838450,838450,838450,838450,838450,838450,838450,838450,838450,838450,
    // 843575,853825,864075,875350,885600,895850,906100,917375,927625,937875,
    // 948125,959400,969650,979900,990150,1000400,1011675,1021925,1032175,1042425

};



#define DATA_BASE_PIN 2
#define CLK_PIN 12      
#define SD_PIN 13
#define SD_CLK 16
#define SD_LATCH 17
#define MCLK 18
#define MD 19
#define MLATCH 20
#define BUTTON_PIN 21

bool printing = false;
void blink_pin_forever(PIO pio, uint sm, uint offset) {

    blink_program_init(pio, sm, offset, DATA_BASE_PIN, CLK_PIN);
    pio_sm_set_enabled(pio, sm, true);
}

void reset_DAC(PIO pio, uint sm){
    pio_sm_put_blocking(pio, sm, -1);
}

void shift_register_write(){
    // printf("Writing to shift register\n");
    gpio_put(SD_LATCH, 1);
    for(int i = 0; i < 30; i++){
        gpio_put(SD_CLK, 0);

        gpio_put(SD_PIN, 1);
        gpio_put(SD_PIN + 1, 0);
        gpio_put(SD_PIN + 2, 0);

        gpio_put(SD_CLK, 1);
        i = i;
    }
    for(int i = 0; i < 290; i++){
        gpio_put(SD_CLK, 0);

        gpio_put(SD_PIN, 0);
        gpio_put(SD_PIN + 1, 0);
        gpio_put(SD_PIN + 2, 0);

        gpio_put(SD_CLK, 1);
        i = i;
    }
    gpio_put(SD_LATCH, 0);
    gpio_put(SD_CLK, 0);
    gpio_put(SD_LATCH, 1);
    // printf("Done writing to shift register\n");
}

void getButton(){
    if(gpio_get(BUTTON_PIN) == 0){
        sleep_ms(2);
        if(gpio_get(BUTTON_PIN) == 0)
            printing = true;
    }
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
    gpio_init(BUTTON_PIN);

    gpio_init(MCLK);
    gpio_init(MD);
    gpio_init(MLATCH);

    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);
    gpio_set_dir(SD_PIN, GPIO_OUT);
    gpio_set_dir(SD_PIN + 1, GPIO_OUT);
    gpio_set_dir(SD_PIN + 2, GPIO_OUT);
    gpio_set_dir(SD_CLK, GPIO_OUT);
    gpio_set_dir(SD_LATCH, GPIO_OUT);

    gpio_set_dir(MCLK, GPIO_OUT);
    gpio_set_dir(MD, GPIO_OUT);
    gpio_set_dir(MLATCH, GPIO_OUT);

    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
    // printf("HELLO WORLD\n");
    PIO pio = pio0;
    uint sm;
    uint offset;
    pio_claim_free_sm_and_add_program_for_gpio_range(&blink_program, &pio, &sm, &offset, DATA_BASE_PIN, 11, true);
    // printf("Loaded program at %d\n", offset);
    gpio_put(MLATCH, 1);
    blink_pin_forever(pio, sm, offset);
    reset_DAC(pio, sm);
    while(true){

        getButton();
        if(printing){
            printing = false;
            gpio_put(PICO_DEFAULT_LED_PIN, 1);
            shift_register_write();

            gpio_put(MD, 1);   

            gpio_put(MCLK, 0);
            gpio_put(MD, 1);
            gpio_put(MCLK, 1);
            
            for(int i = 1; i < 8; i++){
                gpio_put(MCLK, 0);
                gpio_put(MD, 0);
                gpio_put(MCLK, 1);
                i = i;
            }
            gpio_put(MD, 1);
            gpio_put(MLATCH, 0);
            sleep_us(1);
            gpio_put(MLATCH, 1);
            gpio_put(MD, 0);

            gpio_put(MLATCH, 0);
            sleep_us(1);
            gpio_put(MLATCH, 1);

            gpio_put(MD, 0);
            
           
            for(int i = 0; i < datalen; i++){
                pio_sm_put_blocking(pio, sm, dataDAC[i]);
            }
            sleep_ms(100);
            gpio_put(PICO_DEFAULT_LED_PIN, 0);
            
        }

        sleep_ms(50);
        
    }
    // while (true) {
    //     printf("Hello, world!\n");
    //     sleep_ms(1000);
    // }
    return 0;
}
