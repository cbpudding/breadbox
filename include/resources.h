#ifndef RESOURCES_H
#define RESOURCES_H

typedef enum {
    // We don't know what type this is supposed to be and it's up to the game to
    // figure it out. ~Alex
    BBRES_UNKNOWN
} breadbox_resource_type_t;

// Assuming each field is only 32 bits, this structure should only be 16 bytes.
// Maybe 20 if we add a signature like "BBR\0"... ~Alex
typedef struct {
    // Type of resource
    breadbox_resource_type_t type;
    // Size of the resource in bytes
    int size;
    // CRC32C Hash to verify resource integrity
    int hash;
    // Pointer or offset from the start of the file to the resource data
    void *data;
} breadbox_resource_t;

#endif