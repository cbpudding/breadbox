#ifndef RESOURCES_H
#define RESOURCES_H

typedef enum {
    // Whatever the data is, it's not tied to a specific platform! ~Alex
    BBPLAT_NEUTRAL,
    // Data with this descriptor can only be used with the unix-glx target ~Alex
    BBPLAT_UNIX_GLX
} breadbox_resource_platform_t;

typedef enum {
    // We don't know what type this is supposed to be and it's up to the game to
    // figure it out. ~Alex
    BBRES_UNKNOWN
} breadbox_resource_type_t;

typedef struct {
    // Type of resource
    breadbox_resource_type_t type;
    // Size of the resource in bytes
    int size;
    // CRC32C hash to verify resource integrity
    int hash;
    // Pointer to the resource data
    void *data;
} breadbox_resource_t;

typedef struct {
    // File signature. Should always be "BBR\0". Maybe we could use this value
    // to detect endianness or maybe even a version number sometime in the
    // future? If "BBR\0" is 0x42425200 on little endian architectures, then we
    // will read it as 0x00524242 if the file's endianness isn't correct! ~Alex
    char signature[4];
    // Supported platform
    breadbox_resource_platform_t platform;
    // Type of resource
    breadbox_resource_type_t type;
    // Size of the resource of bytes
    int size;
    // CRC32C hash to verify resource integrity
    int hash;
} breadbox_resource_header_t;

#endif