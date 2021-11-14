#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

#include "mcp2515.h"

// using SPI0 at 10MHz
#define SPI_PORT spi0
#define SPI_BAUD 10 * 1000000

#define SPI_SCK 2
#define SPI_TX  3
#define SPI_RX  4
#define SPI_CS  5

int main(int argc, char **argv) {
    // turn on board LED
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    stdio_init_all();

    // delay before printf (otherwise it likely won't print)
    sleep_ms(500);

    // SETUP

    MCP2515 mcp2515(SPI_PORT, SPI_BAUD, SPI_SCK, SPI_TX, SPI_RX, SPI_CS);

    struct can_frame canMsg;

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
