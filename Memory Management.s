; Memory management system

section .data
; Memory block structure
memory_blocksize dd 0
_block_nextd 0
; Memory management system data
memorystart dd 0memory_end dd 0memory_blocks dd 0

; Error codes
ERROR_OUT_OF_MEMORY db "Out of memory", 0
ERROR_INVALID_ADDRESS db "Invalid address", 0

section .text

; Initialize memory management system
init_memory_management:
    ; Set memory start and end addresses
    mov eax, memory_start
    mov ebx, memory_end

    ; Initialize memory blocks
    mov ecx, 0
    mov edi, memory_blocks

init_memory_blocks:
    ; Check if memory block is within memory range
    cmp eax, ebx
    jge end_init_memory_blocks

    ; Set memory block size
    mov [edi], eax

    ; Set memory block next pointer
    mov eax, [edi]
    add eax, memory_block_size
    mov [edi + memory_block_next], eax

    ; Move to next memory block
    add edi, memory_block_size

    ; Move to next memory block
    jmp init_memory_blocks

end_init_memory_blocks:
    ; Return
    ret

; Allocate memory
allocate_memory:
    ; Check if memory blocks are available
    cmp dword [memory_blocks], 0
    jz error_out_of_memory

    ; Get first memory block
    mov edi, [memory_blocks]

    ; Allocate memory
    mov eax, [edi]
    mov [edi], 0

    ; Update memory blocks
    mov [memory_blocks], [edi + memory_block_next]

    ; Return allocated memory address
    ret

error_out_of_memory:
    ; Print out of memory error message
    mov eax, SYS_write
    mov ebx, STDOUT
    lea ecx, [ERROR_OUT_OF_MEMORY]
    mov edx, len_ERROR_OUT_OF_MEMORY
    int 0x80

    ; Return error code
    mov eax, -1
    ret

; Deallocate memory
deallocate_memory:
    ; Check if memory block is within memory range
    cmp eax, memory_start
    jl error_invalid_address
    cmp eax, memory_end
    jge error_invalid_address

    ; Find memory block
    mov edi, memory_blocks

find_memory_block:
    ; Check if memory block is found
    cmp dword [edi], 0
    jz error_invalid_address

    ; Check if memory block is the one to deallocate
    cmp eax, [edi]
    jz deallocate_memory_block

    ; Move to next memory block
    mov edi, [edi + memory_block_next]

    ; Continue finding memory block
    jmp find_memory_block

deallocate_memory_block:
    ; Deallocate memory block
    mov [edi], eax

    ; Update memory blocks
    mov ebx, [edi + memory_block_next]
    mov [edi + memory_block_next], 0
    mov [edi], ebx

    ; Return
    ret

error_invalid_address:
    ; Print invalid address error message
    mov eax, SYS_write
    mov ebx, STDOUT
    lea ecx, [ERROR_INVALID_ADDRESS]
    mov edx, len_ERROR_INVALID_ADDRESS
    int 0x80

    ; Return error code
    mov eax, -1
    ret

; Reallocate memory
reallocate_memory:
    ; Check if memory block is within memory range
    cmp eax, memory_start
    jl error_invalid_address
    cmp eax, memory_end
    jge error_invalid_address

    ; Find memory block
    mov edi, memory_blocks

find_memory_block_reallocate:
    ; Check if memory block is found
    cmp dword [edi], 0
    jz error_invalid_address

    ; Check if memory block is the one to reallocate
    cmp eax, [edi]
    jz reallocate_memory_block

    ; Move to next memory block
    mov edi, [edi + memory_block_next]

    ; Continue finding memory block
    jmp find_memory_block_reallocate

reallocate_memory_block:
    ; Check if memory block is large enough
    mov ecx, [edi + memory_block_size]
    cmp ecx, ebx
    jge end_reallocate_memory

    ; Reallocate memory block
    mov ecx, [edi]
    add ecx, ebx
    mov [edi + memory_block_size], ebx

    ; Update memory blocks
    mov edi, memory_blocks
    mov esi, [edi]

find_memory_block_split:
    ; Check if memory block is found
    cmp dword [edi], 0
    jz error_invalid_address

    ; Check if memory block is the one to split
    cmp esi, [edi]
    jz split_memory_block

    ; Move to next memory block
    mov edi, [edi + memory_block_next]

    ; Continue finding memory block
    jmp find_memory_block_split

split_memory_block: