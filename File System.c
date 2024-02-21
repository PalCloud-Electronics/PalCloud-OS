#include <stdbool.h>
#include <string.h>

#define FILE_SYSTEM_SIZE 0x100000
#define FILE_NAME_MAX_LENGTH 64
#define FILE_CONTENT_MAX_LENGTH 1024

typedef struct {
    char name[FILE_NAME_MAX_LENGTH];
    uint32_t start_address;
    uint32_t length;
} File;

typedef struct {
    File files[100];
    uint32_t file_count;
} FileSystem;

FileSystem file_system;

bool file_system_init(void)
{
    file_system.file_count = 0;
    return true;
}

bool file_system_add_file(const char *name, const uint8_t *content, uint32_t length)
{
    if (file_system.file_count >= 100)
    {
        return false;
    }

    File *file = &file_system.files[file_system.file_count];
    strncpy(file->name, name, FILE_NAME_MAX_LENGTH);
    file->start_address = FILE_SYSTEM_SIZE + file_system.file_count * FILE_CONTENT_MAX_LENGTH;
    file->length = length;

    for (uint32_t i = 0; i < length; i++)
    {
        *((uint8_t *)file->start_address + i) = content[i];
    }

    file_system.file_count++;

    return true;
}

const File *file_system_find_file(const char *name)
{
    for (uint32_t i = 0; i < file_system.file_count; i++)
    {
        if (strcmp(file_system.files[i].name, name) == 0)
        {
            return &file_system.files[i];
        }
    }

    return NULL;
}