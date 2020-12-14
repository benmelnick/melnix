    global loader                   ; the entry symbol for ELF

    extern main                     ; entry symbol for C code

    MAGIC_NUMBER equ 0x1BADB002     ; define the magic number constant
    FLAGS        equ 0x0            ; multiboot flags
    CHECKSUM     equ -MAGIC_NUMBER  ; calculate the checksum
                                    ; (magic number + checksum + flags should equal 0)

    KSTACK_SIZE  equ 0x1000         ; stack size of 4096 bytes

    section .text:                  ; start of the text (code) section
    align 4                         ; the code must be 4 byte aligned
        dd MAGIC_NUMBER             ; write the magic number to the machine code,
        dd FLAGS                    ; the flags,
        dd CHECKSUM                 ; and the checksum

    loader:                         ; the loader label (defined as entry point in linker script)
    
    setup_kstack:                   ; point esp to the start of the stack (end of memory area)
        mov esp, kernel_stack + KSTACK_SIZE
        
    call_main:
        call main
    
    .loop:
        jmp .loop                   ; loop forever

    section .bss
    align 4
    kernel_stack:
        resb KSTACK_SIZE            ; reserve KSTACK_SIZE bytes of memory for the stack