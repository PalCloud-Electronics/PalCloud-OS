#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Physical hard drive structure
typedef struct {
    // File handle for the image file
    FILE *image;

    // Size of the image file
    size_t size;

    // Current position in the image file
    size_t position;

    // Buffer for reading and writing data
    void *buffer;

    // Size of the buffer
    size_t buffer_size;
} physical_hard_drive_t;

// Path to the image file
char image_path[256];

// Virtual hard drive structure
typedef struct {
    // File handle for the image file
    FILE *image;

    // Size of the image file
    size_t size;

    // Current position in the image file
    size_t position;

    // Buffer for reading and writing data
    void *buffer;

    // Size of the buffer
    size_t buffer_size;

    // Physical hard drive
    physical_hard_drive_t *physical;
} virtual_hard_drive_t;

// Initialize the physical hard drive
void physical_hard_drive_init(physical_hard_drive_t *phd, const char *image_path) {
    // Open the image file
    phd->image = fopen(image_path, "r+");

    // Get the size of the image file
    fseek(phd->image, 0, SEEK_END);
    phd->size = ftell(phd->image);
    rewind(phd->image);

    // Allocate a buffer for reading and writing data
    phd->buffer_size = 4096;
    phd->buffer = malloc(phd->buffer_size);

    // Set the current position to the beginning of the file
    phd->position = 0;
}

// Read data from the physical hard drive
void physical_hard_drive_read(physical_hard_drive_t *phd, void *buffer, size_t size) {
    // Read the data from the image file into the buffer
    while (size > 0) {
        // Determine how much data to read from the image file
        size_t to_read = size < phd->buffer_size ? size : phd->buffer_size;

        // Read the data from the image file
        fseek(phd->image, phd->position, SEEK_SET);
        fread(phd->buffer, to_read, 1, phd->image);

        // Copy the data from the buffer to the destination
        memcpy(buffer, phd->buffer, to_read);

        // Update the current position
        phd->position += to_read;

        // Advance the destination pointer
        buffer = (char *)buffer + to_read;

        // Subtract the amount of data read from the size
        size -= to_read;
    }
}

// Write data to the physical hard drive
void physical_hard_drive_write(physical_hard_drive_t *phd, const void *buffer, size_t size) {
    // Write the data from the buffer to the image file
    while (size > 0) {
        // Determine how much data to write to the image file
        size_t to_write = size < phd->buffer_size ? size : phd->buffer_size;

        // Copy the data from the buffer to the hard drive buffer
        memcpy(phd->buffer, buffer, to_write);

        // Write the data to the image file
        fseek(phd->image, phd->position, SEEK_SET);
        fwrite(phd->buffer, to_write, 1, phd->image);

        // Update the current position
        phd->position += to_write;

        // Advance the source pointer
        buffer = (char *)buffer + to_write;

        // Subtract the amount of data written from the size
        size -= to_write;
    }
}

// Get the size of the physical hard drive
size_t physical_hard_drive_size(physical_hard_drive_t *vhd) {
    return vhd->size;
}

// Seek to a specific position in the physical hard drive
void physical(physiacl_hard_drive_t *vhd, size_t position) {
    // Seek to the specified position in the physical hard drive
    physical_hard_drive_seek(&vhd->physical, position);

    // Update the current position
    vhd->position = position;
}

// Close the physical hard drive
void physical_hard_drive_close(physical_hard_drive_t *vhd) {
    // Free the buffer
    free(vhd->buffer);

    // Close the image file
    fclose(vhd->image);
}

// Initialize the virtual hard drive
void virtual_hard_drive_init(virtual_hard_drive_t *vhd, const char *image_path, physical_hard_drive_t *physical) {
    // Initialize the virtual hard drive with the same parameters as the physical hard drive
    physical_hard_drive_init(&vhd->physical, image_path);

    // Set the image file and size to the same as the physical hard drive
    vhd->image = vhd->physical.image;
    vhd->size = physical_hard_drive_size(&vhd->physical);

    // Set the current position to the beginning of the file
    vhd->position = 0;

    // Allocate a buffer for reading and writing data
    vhd->buffer_size = 4096;
    vhd->buffer = malloc(vhd->buffer_size);
}

// Read data from the virtual hard drive
void virtual_hard_drive_read(virtual_hard_drive_t *vhd, void *buffer, size_t size) {
    // If the read would go beyond the end of the file, adjust the size
    if (vhd->position + size > vhd->size) {
        size = vhd->size - vhd->position;
    }

    // Read the data from the physical hard drive
    physical_hard_drive_read(&vhd->physical, vhd->buffer, size);

    // Copy the data from the buffer to the destination
    memcpy(buffer, vhd->buffer, size);

    // Update the current position
    vhd->position += size;
}

// Write data to the virtual hard drive
void virtual_hard_drive_write(virtual_hard_drive_t *vhd, const void *buffer, size_t size) {
    // If the write would go beyond the end of the file, adjust the size
    if (vhd->position + size > vhd->size) {
        vhd->size = vhd->position + size;
    }

    // Write the data to the physical hard drive
    physical_hard_drive_write(&vhd->physical, buffer, size);

    // Update the current position
    vhd->position += size;
}

// Get the size of the virtual hard drive
size_t virtual_hard_drive_size(virtual_hard_drive_t *vhd) {
    return vhd->size;
}

// Seek to a specific position in the virtual hard drive
void virtual_hard_drive_seek(virtual_hard_drive_t *vhd, size_t position) {
    // Seek to the specified position in the physical hard drive
    physical_hard_drive_seek(&vhd->physical, position);

    // Update the current position
    vhd->position = position;
}

// Close the virtual hard drive
void virtual_hard_drive_close(virtual_hard_drive_t *vhd) {
    // Free the buffer
    free(vhd->buffer);

    // Close the image file
    fclose(vhd->image);
}