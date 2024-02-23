.global _start

.section .data
string:
    .ascii "Hello, world!\0"

.section .text
_start:
    ; copy the string
    ldr r1, =string
    ldr r2, =string_copy
    mov r0, #16
copy_loop:
    ldrb r3, [r1], #1
    strb r3, [r2], #1
    subs r0, r0, #1
    bne copy_loop

    ; print the string
    ldr r0, =1
    ldr r1, =string_copy
    ldr r2, =16
    mov r7, #4
    svc 0

    ; exit
    mov r7, #1
    svc 0

.section .bss
string_copy:
    .skip 16