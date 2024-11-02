cmake_minimum_required(VERSION 3.13)

# Initialize Pico SDK
include(pico_sdk_import.cmake)

project(DroneFlightController C CXX ASM)
pico_sdk_init()

# Your project settings
add_executable(drone_flight_controller
    src/communication/spi_driver.c
    # ... other source files ...
)

# Link Pico libraries
target_link_libraries(drone_flight_controller pico_stdlib hardware_spi) 