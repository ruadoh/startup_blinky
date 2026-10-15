MEMORY
{
    flash (rx) : ORIGIN = 0x08000000, LENGTH = 512K
    ram (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

_eram = ORIGIN(ram) + LENGTH(ram);
/* Stack starts at the end of ram and grows downwards */
_stack = _eram;

SECTIONS
{
    .text : ALIGN(4)
    {
        /* Placing IVT at start of flash MEMORY
           KEEP ensures compiler doesn't optimise out. */
        KEEP(*(.ivt))

        /* Place rest of code (.text) after */
        *(.text*)

        /* Create end of text symbol for convenience. */
        . = ALIGN(4);
        _etext = .;
    } > flash

    /* Initial (constant) values for data is placed in flash after the program code */
    _idata = LOADADDR(.data);

    .data : ALIGN(4)
    {
        _data = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > ram AT > flash  /* AT keyword tells linker to copy initial data values from flash to RAM? */

    .bss : ALIGN(4)
    {
        _bss = .;
        *(.bss*)
        . = ALIGN(4);
        _ebss = .;
        PROVIDE(_end = .);
    } > ram
}