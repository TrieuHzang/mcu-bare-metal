ENTRY(Reset_Handler)

/*
 * STM32F103C8T6 memory map
 *
 * FLASH : 64 KB starting at 0x08000000
 * RAM   : 20 KB starting at 0x20000000
 */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 64K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

/*
 * Initial Main Stack Pointer.
 *
 * RAM starts at 0x20000000
 * RAM size is 20 KB = 0x5000
 *
 * Therefore:
 * _estack = 0x20005000
 */
_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    /*
     * Vector table.
     *
     * Must be placed at the beginning of Flash.
     */
    .isr_vector :
    {
        . = ALIGN(4);

        KEEP(*(.isr_vector))

        . = ALIGN(4);
    } > FLASH

    /*
     * Program instructions and read-only data.
     */
    .text :
    {
        . = ALIGN(4);

        *(.text*)
        *(.rodata*)

        . = ALIGN(4);
    } > FLASH

    /*
     * Initialized global/static variables.
     *
     * Runtime location: RAM
     * Initial values:   FLASH
     */
    .data :
    {
        . = ALIGN(4);

        _sdata = .;

        *(.data*)

        . = ALIGN(4);

        _edata = .;
    } > RAM AT > FLASH

    /*
     * Address in Flash where the initial
     * .data values are stored.
     */
    _sidata = LOADADDR(.data);

    /*
     * Zero-initialized global/static variables.
     */
    .bss (NOLOAD) :
    {
        . = ALIGN(4);

        _sbss = .;

        *(.bss*)
        *(COMMON)

        . = ALIGN(4);

        _ebss = .;
    } > RAM
}
