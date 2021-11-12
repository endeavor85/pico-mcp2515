#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

#include "mcp2515.h"

// using SPI0 at 10MHz
#define SPI_PORT spi0
// #define SPI_BAUD 10 * 1000000
// using SPI0 at 1MHz
#define SPI_BAUD 1000 * 1000

#define SPI_SCK 2
#define SPI_TX  3
#define SPI_RX  4
#define SPI_CS  5

int main(int argc, char **argv) {
    stdio_init_all();

    // turn on board LED
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    // delay before printf (otherwise it likely won't print)
    sleep_ms(500);

    struct can_frame canMsg;
    
    gpio_set_function(SPI_RX, GPIO_FUNC_SPI);
    gpio_set_function(SPI_SCK, GPIO_FUNC_SPI);
    gpio_set_function(SPI_TX, GPIO_FUNC_SPI);
    // Make the SPI pins available to picotool
    bi_decl(bi_3pins_with_func(SPI_RX, SPI_TX, SPI_SCK, GPIO_FUNC_SPI));

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(SPI_CS);
    gpio_set_dir(SPI_CS, GPIO_OUT);
    gpio_put(SPI_CS, 1);
    // Make the CS pin available to picotool
    bi_decl(bi_1pin_with_name(SPI_CS, "SPI CS"));

    // SETUP

    MCP2515 mcp2515(SPI_PORT, SPI_BAUD, SPI_CS);
    mcp2515.reset();
    mcp2515.setBitrate(CAN_125KBPS, MCP_8MHZ);
    mcp2515.setNormalMode();
    
    printf("------- CAN Read ----------\n");
    printf("ID  DLC   DATA\n");

    // LOOP

    while(true) {
        if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
            printf("%0x %0x ", canMsg.can_id, canMsg.can_dlc); // print ID and DLC
            
            for (int i = 0; i < canMsg.can_dlc; i++)  {  // print the data
                printf("%0x ", canMsg.data[i]);
            }

            printf("\n");
        }
    }
}
