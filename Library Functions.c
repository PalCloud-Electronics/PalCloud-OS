#include <stdint.h>

typedef struct memory_block {
    uint32_t size;
    uint32_t next;
} memory_block_t;

memory_block_t memory_blocks;
uint32_t memory_start;
uint32_t memory_end;

void init_memory_management(uint32_t start, uint32_t end) {
    memory_start = start;
    memory_end = end;

    memory_block_t *block = (memory_block_t *)start;
    block->size = 0;
    block->next = 0;
}

void *allocate_memory(uint32_t size) {
    if (memory_blocks.size == 0) {
        return NULL;
    }

    memory_block_t *block = (memory_block_t *)memory_blocks.size;
    memory_blocks.size = block->next;

    return (void *)(block + 1);
}

void deallocate_memory(void *memory) {
    if (memory == NULL) {
        return;
    }

    memory_block_t *block = (memory_block_t *)memory - 1;
    block->next = memory_blocks.size;
    memory_blocks.size = (uint32_t)block;
}

void *reallocate_memory(void *memory, uint32_t size) {
    if (memory == NULL) {
        return allocate_memory(size);
    }

    memory_block_t *block = (memory_block_t *)memory - 1;

    if (block->size >= size) {
        return memory;
    }

    void *new_memory = allocate_memory(size);

    if (new_memory == NULL) {
        return NULL;
    }

    uint32_t copy_size = size;

    if (copy_size > block->size) {
        copy_size = block->size;
    }

    memcpy(new_memory, memory, copy_size);
    deallocate_memory(memory);

    return new_memory;
}

int main() {
    init_memory_management(0x1000, 0x2000);

    int *array = allocate_memory(10 * sizeof(int));

    if (array == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        array[i] = i * i;
    }

    for (int i = 0; i < 10; i++) {
        printf("%d ", array[i]);
    }

    printf("\n");

    deallocate_memory(array);

    return 0;
}