Simple startup code for Cortex-M4, following YouTuve video:

https://www.youtube.com/watch?v=MhOba73z-dQ&list=LL&index=8&t=332s

# Compile
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -std=c11 -g -O0 -c startup_blinky.c -o startup_blinky.o
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -std=c11 -g -O0 -c startup_stm32f411.c -o startup_stm32f411.o

# Link
arm-none-eabi-gcc -T stm32f411.ld -nolibc -nostartfiles -Wl,--verbose startup_blinky.o startup_stm32f411.o -o startup_blinky.elf

arm-none-eabi-objdump -D startup_blinky.elf | less

arm-none-eabi-size startup_blinky.elf

# Notes
1. I didn't experience hardfault with test_crt as specfied in video (from reading variables stored in flash - verified they were stored in flash). Some difference between m0 and m4?