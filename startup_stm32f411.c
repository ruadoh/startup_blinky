extern int main(void);

// Symbols defined by the linker script
extern unsigned int _stack;
extern unsigned int _data;
extern unsigned int _idata;
extern unsigned int _edata;
extern unsigned int _bss;
extern unsigned int _ebss;

/**********/
#define RCC_BASE 0x40023800
#define RCC_CFGR_SWS (0xC << 2)  // Bits 3:2 SWS[1:0] System clock switch status

typedef struct {
    volatile unsigned int CR;
    volatile unsigned int PLLCFGR;
    volatile unsigned int CFGR;
    volatile unsigned int CIR;
    volatile unsigned int AHB1RSTR;
    volatile unsigned int AHB2RSTR;
    volatile unsigned int AHB3RSTR;
    unsigned int RESERVED0;
    volatile unsigned int APB1RSTR;
    volatile unsigned int APB2RSTR;
    unsigned int RESERVED1[2];
    volatile unsigned int AHB1ENR;
    volatile unsigned int AHB2ENR;
    volatile unsigned int AHB3ENR;
    unsigned int RESERVED2;
    volatile unsigned int APB1ENR;
    volatile unsigned int APB2ENR;
} RCC_TypeDef;

#define RCC ((RCC_TypeDef *)RCC_BASE)

uint32_t SystemCoreClock = 16000000; // Default to HSI clock (16 MHz)
const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
/********/

static void get_core_clock(void) {
    uint32_t tmp = RCC->CFGR & RCC_CFGR_SWS;
    switch(tmp) {
        case 0x00:
            // HSI oscillator used as system clock
            // Turn on PA5 (LED on)
            // GPIOA_ODR |= (1 << 5);
            SystemCoreClock = 16000000; // HSI is 16 MHz
            break;
        case 0x04:
            // HSE oscillator used as system clock
            break;
        case 0x08:
            // PLL used as system clock
            break;
        default:
            // Reserved, should not happen
            break;
    }

    // Get HCLK prescaler
    // tmp = (RCC->CFGR & 0x00000070) >> 4;
    tmp = AHBPrescTable[(RCC->CFGR & 0x000000F0) >> 4];
    SystemCoreClock >>= tmp; // Apply prescaler (equivalent to dividing by 2^tmp)
}

// Global (and static local) variables initialized to non-zero value
static void copy_data(void) {
    unsigned int *src_data_ptr = &_idata;
    unsigned int *dst_data_ptr = &_data;
    while (dst_data_ptr < &_edata) {
        *dst_data_ptr++ = *src_data_ptr++;
    }
}

// Global (and static local) variables initialized to zero
static void clear_bss(void) {
    unsigned int *bss_ptr = &_bss;
    while (bss_ptr < &_ebss) {
        *bss_ptr++ = 0;
    }
}

void isr_reset(void) {
    // core_init();
    copy_data();
    clear_bss();

    main();

    // We should never return from main, but just in case we do
    while(1);
}

void isr_hardfault(void) {
    while(1);
}

#define IVT_ARRAY_SIZE (90U)
typedef void (*isr_t)(void);
__attribute((used, section(".ivt")))
static const isr_t ivt[IVT_ARRAY_SIZE] =
{
    (isr_t)&_stack,
    isr_reset,
    0, // NMI
    isr_hardfault,

    // Incomplete vector table [demo purposes]
};