#include <stdint.h>

// ============================================================================
// 1. IMPORT LINKER LANDMARKS
// ============================================================================
// These symbols are defined in our linker.ld file. We use 'extern' because 
// their physical addresses are resolved during the final linking phase.
extern uint32_t _sidata; // Start of .data section in Flash (source)
extern uint32_t _sdata;  // Start of .data section in RAM (destination)
extern uint32_t _edata;  // End of .data section in RAM
extern uint32_t _sbss;   // Start of .bss section in RAM
extern uint32_t _ebss;   // End of .bss section in RAM
extern uint32_t _estack; // End of stack (top of RAM from our linker)

// Forward declaration of the main application entry point
extern int main();

// Forward declaration of the Reset Handler
void Reset_Handler(void);

// ============================================================================
// 2. THE RESET HANDLER (THE IGNITION FUNCTION)
// ============================================================================
// This is the very first function the STM32 executes when power is applied.
void Reset_Handler(void) {
    // A. Copy the .data section from Flash to RAM
    uint32_t *src = &_sidata;
    uint32_t *dest = &_sdata;

    while (dest < &_edata) {
        *dest++ = *src++;
    }

    // B. Clear the .bss section in RAM to 0 (prevents random garbage data)
    uint32_t *bss_dest = &_sbss;
    while (bss_dest < &_ebss) {
        *bss_dest++ = 0;
    }

    // C. Jump to our actual C++ main application!
    main();

    // If main ever exits (it shouldn't in bare metal), trap the CPU here
    while (1);
}

// ============================================================================
// 3. THE VECTOR TABLE
// ============================================================================
// We define a function pointer type for exception and interrupt handlers.
typedef void (*ISR_Handler)(void);

// We place this array in the ".isr_vector" section. Our linker script is
// hardcoded to place this section at address 0x08000000 (start of Flash).
__attribute__((section(".isr_vector")))
const ISR_Handler Vector_Table[] = {
    (ISR_Handler)&_estack,  // 1. Initial Stack Pointer (MSP)
    Reset_Handler,          // 2. The Reset Vector (Points to our ignition code)
    0,                      // 3. NMI Handler (Placeholder)
    0,                      // 4. HardFault Handler (Placeholder)
};
