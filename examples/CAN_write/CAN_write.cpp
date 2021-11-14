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

    struct can_frame canMsg1;
    struct can_frame canMsg2;

    canMsg1.can_id  = 0x0F6;
    canMsg1.can_dlc = 8;
    canMsg1.data[0] = 0x8E;
    canMsg1.data[1] = 0x87;
    canMsg1.data[2] = 0x32;
    canMsg1.data[3] = 0xFA;
    canMsg1.data[4] = 0x26;
    canMsg1.data[5] = 0x8E;
    canMsg1.data[6] = 0xBE;
    canMsg1.data[7] = 0x86;

    canMsg2.can_id  = 0x036;
    canMsg2.can_dlc = 8;
    canMsg2.data[0] = 0x0E;
    canMsg2.data[1] = 0x00;
    canMsg2.data[2] = 0x00;
    canMsg2.data[3] = 0x08;
    canMsg2.data[4] = 0x01;
    canMsg2.data[5] = 0x00;
    canMsg2.data[6] = 0x00;
    canMsg2.data[7] = 0xA0;
    
    mcp2515.reset();
    mcp2515.setBitrate(CAN_125KBPS, MCP_8MHZ);
    mcp2515.setNormalMode();
    
    printf("Example: Write to CAN\n");

    // LOOP

    while(true) {
      uint8_t status = mcp2515.getStatus();
      printf("Status: 0x%02x\n", status);

      mcp2515.sendMessage(&canMsg1);
      mcp2515.sendMessage(&canMsg2);

      printf("Messages sent\n");
      
      sleep_ms(100);
    }
}
