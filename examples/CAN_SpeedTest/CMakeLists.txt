add_executable(mcp2515-CAN_SpeedTest-example
CAN_SpeedTest.cpp
	../../mcp2515.cpp
)

# Pull in pico_stdlib which pulls in commonly used features
target_link_libraries(mcp2515-CAN_SpeedTest-example pico_stdlib hardware_spi)

# enable stdio_usb
pico_enable_stdio_usb(mcp2515-CAN_SpeedTest-example 1) 
pico_enable_stdio_uart(mcp2515-CAN_SpeedTest-example 0) 

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(mcp2515-CAN_SpeedTest-example)
