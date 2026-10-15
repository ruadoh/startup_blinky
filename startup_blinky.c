
#include <stdint.h>

#define RCC_BASE 0x40023800
#define RCC_AHB1ENR (*(volatile unsigned int *)(RCC_BASE + 0x30))
#define GPIOA_BASE 0x40020000
#define GPIOA_MODER (*(volatile unsigned int *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR (*(volatile unsigned int *)(GPIOA_BASE + 0x14))

// Function to test CRT0 initialization of global and static variables
int test_crt(void);

int main(void) {
    // test_crt();

    // Enable AHB1ENR clock access for GPIOA
    RCC_AHB1ENR |= (1 << 0);

    // Set PA5 as output
    GPIOA_MODER |= (1 << 10);  // Set bit 10 to 1
    GPIOA_MODER &= ~(1 << 11); // Set bit 11 to 0

    while (1) {
        // Toggle PA5
        GPIOA_ODR ^= (1 << 5);
        // Seems like CPU Freq set to 1MHz by default - from eye.
        for (volatile int i = 0; i < 1000000UL; i++); // Delay
    }

    return 0;
}

int global_variable = 10;
int uninit_global_variable;  // Expect 0 according to C standard.

/* Function to show why CRT0 is needed
    Without CRT0: unitialized variables contain garbage, when we expect them to be 0. 
    This is fixed with copy_data and clear_bss functions in startup.
    In linker script, we also needed to define data and bss sections, ensuring data sections are copied from flash to RAM.*/
int test_crt(void) {
    int local_variable = 20;
    static int static_local_variable = 30;
    static int uninit_static_local_variable;  // Expect 0 according to C standard.
    static int sum;  // Expect 0 according to C standard.
    sum = global_variable + local_variable + 
          static_local_variable + uninit_global_variable +
          uninit_static_local_variable;
    if (sum == 60) {
        while(1) { }
    } else {
        return 0;
    }
}