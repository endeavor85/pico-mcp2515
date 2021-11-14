#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "pico/time.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"

#include "mcp2515.h"

// using SPI0 at 10MHz
#define SPI_PORT spi0
#define SPI_BAUD 10 * 1000000

#define SPI_SCK 18
#define SPI_TX  19
#define SPI_RX  16
#define SPI_CS  17

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

    can_frame canMsg;

    mcp2515.reset();
    mcp2515.setBitrate(CAN_125KBPS, MCP_8MHZ);
    mcp2515.setNormalMode();
    
    printf("------- CAN Speedtest ----------\n");

    // LOOP

    while(true) {

        // set end time tp 1000 ms from now
        const uint32_t endTime = to_ms_since_boot(make_timeout_time_ms(1000));
        uint16_t cntr = 0;

        // count good messages until endTime is reached
        while (to_ms_since_boot(get_absolute_time()) < endTime) {
            if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
                cntr++;
            }
        }
        
        // print result
        printf("%6d msg/sec\n", cntr);
    }
}
