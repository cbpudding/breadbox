#include <stdint.h>

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
    uint32_t size;
    // CRC32C hash to verify resource integrity
    uint32_t hash;
    // Pointer to the resource data
    void *data;
} breadbox_resource_t;

typedef struct {
    // File signature. Should always be "BBR\0". If the bytes are backwards then
    // the file was created for the wrong architecture and no attempts should be
    // made to read the file any further. ~Alex
    uint32_t signature;
    // Supported platform
    breadbox_resource_platform_t platform;
    // Type of resource
    breadbox_resource_type_t type;
    // Size of the resource of bytes
    uint32_t size;
    // CRC32C hash to verify resource integrity
    uint32_t hash;
} breadbox_resource_header_t;

#endif