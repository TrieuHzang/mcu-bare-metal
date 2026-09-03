ENTRY(Reset_Handler)

/*
 * STM32F103C8T6 memory map
 *
 * FLASH: 64 KB
 * RAM:   20 KB
 */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 64K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

/*
 * Initial Main Stack Pointer.
 *
 * RAM:
 * 0x20000000 + 20 KB
 * = 0x20000000 + 0x5000
 * = 0x20005000
 */
_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    /*
     * Cortex-M vector table.
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
     * Program code and read-only data.
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
     * Runtime address: RAM
     * Initial values:  FLASH
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
     * Flash address containing the initial
     * values for .data.
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
