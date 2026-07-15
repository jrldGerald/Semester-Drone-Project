ENTRY(Reset_Handler)

MEMORY      /* This defines the physical boundaries of the hardware boxes */
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 64K
    SRAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

SECTIONS
{  /* <--- OPEN SECTIONS BLOCK HERE */

    /* Group all execution code and constants into FLASH */
    .text :
    {
        . = ALIGN(4);        
        KEEP(*(.isr_vector)) 
        *(.text)             
        *(.text*)            
        *(.rodata)           
        *(.rodata*)
        . = ALIGN(4);
    } > FLASH

    /* Landmark: Find where the initial values are sleeping in FLASH */
    _sidata = LOADADDR(.data);

    /* Allocate runtime space in RAM, but load the default values from FLASH */
    .data :
    {
        . = ALIGN(4);
        _sdata = .;          /* Landmark: Start of initialized variables in RAM */
        *(.data)             /* Grab all initialized variables */
        *(.data*)
        . = ALIGN(4);
        _edata = .;          /* Landmark: End of initialized variables in RAM */
    } > SRAM AT > FLASH
    
    /* Allocate space in RAM for variables that default to zero */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;          /* Landmark: Start of uninitialized memory in RAM */
        *(.bss)             /* Grab all uninitialized variables */
        *(.bss*)
        *(COMMON)           /* Grab any loose global variables */
        . = ALIGN(4);
        _ebss = .;          /* Landmark: End of uninitialized memory in RAM */
    } > SRAM
    
    /* Calculate the absolute top of the Stack (The very end of our 20K SRAM) */
    _estack = ORIGIN(SRAM) + LENGTH(SRAM);

}  /* <--- CLOSE SECTIONS BLOCK HERE AT THE VERY END */
