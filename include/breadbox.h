// I'd like to think that stdarg.h and stdint.h are portable... ~Alex
#include <stdarg.h>
#include <stdint.h>

#include <cglm/cglm.h>

#ifndef BREADBOX_H
#define BREADBOX_H

#define BREADBOX_TICKRATE 20

typedef struct {
    void **data;
    int limit;
    int size;
} breadbox_list_t;

typedef struct {
    // The aspect ratio of the camera
    float aspect;
    // The far plane of the camera's frustum
    float far;
    // The field of view of the camera
    float fov;
    // The camera's calculated matrix
    mat4 matrix;
    // The near plane of the camera's frustum
    float near;
    // The position of the camera
    vec3 position;
} breadbox_camera_t;

typedef struct {
    // The ambient color of the material
    vec4 ambient;
    // The diffuse color of the material
    vec4 diffuse;
    // The specular color of the material
    vec4 specular;
} breadbox_material_t;

// Here because it relies on breadbox_vertex_t
typedef struct {
    // Vertices
    vec3 *a;
    vec3 *b;
    vec3 *c;
    // Normals
    vec3 *na;
    vec3 *nb;
    vec3 *nc;
} breadbox_face_t;

// Here because it relies on breadbox_list_t
typedef struct {
    // List of unique faces in the geometry
    breadbox_list_t faces;
    // List of unique normals
    breadbox_list_t normals;
    // List of unique points in space
    breadbox_list_t vertices;
} breadbox_geometry_t;

typedef struct {
    // The geometry of the prop
    breadbox_geometry_t *geometry;
    // The material of the prop
    breadbox_material_t *material;
    // The model matrix for transforming/rotating/scaling
    mat4 matrix;
} breadbox_prop_t;

typedef struct {
    // Diffuse color of the light
    vec4 diffuse;
    // Position of the light
    vec4 position;
    // Specular color of the light
    vec4 specular;
} breadbox_light_t;

typedef enum {
    BBLOG_DEBUG,
    BBLOG_ERROR,
    BBLOG_INFO,
    BBLOG_WARNING
} breadbox_log_level_t;

typedef enum {
    BBLOG_ENGINE,
    BBLOG_GAME,
    BBLOG_SYSTEM
} breadbox_log_source_t;

typedef enum {
    // Tell the engine that an axis has been updated
    BBMSG_AXIS0,
    BBMSG_AXIS1,
    BBMSG_AXIS2,
    BBMSG_AXIS3,
    BBMSG_AXIS4,
    BBMSG_AXIS5,
    BBMSG_AXIS6,
    BBMSG_AXIS7,
    BBMSG_AXIS8,
    BBMSG_AXIS9,
    BBMSG_AXIS10,
    BBMSG_AXIS11,
    BBMSG_AXIS12,
    BBMSG_AXIS13,
    BBMSG_AXIS14,
    BBMSG_AXIS15,
    BBMSG_AXIS16,
    BBMSG_AXIS17,
    BBMSG_AXIS18,
    BBMSG_AXIS19,
    BBMSG_AXIS20,
    BBMSG_AXIS21,
    BBMSG_AXIS22,
    BBMSG_AXIS23,
    BBMSG_AXIS24,
    BBMSG_AXIS25,
    BBMSG_AXIS26,
    BBMSG_AXIS27,
    BBMSG_AXIS28,
    BBMSG_AXIS29,
    BBMSG_AXIS30,
    BBMSG_AXIS31,
    // Tell the engine that a tick has passed
    BBMSG_TICK,
    // Tell the engine that a trigger has been fired
    BBMSG_TRIGGER0,
    BBMSG_TRIGGER1,
    BBMSG_TRIGGER2,
    BBMSG_TRIGGER3,
    BBMSG_TRIGGER4,
    BBMSG_TRIGGER5,
    BBMSG_TRIGGER6,
    BBMSG_TRIGGER7,
    BBMSG_TRIGGER8,
    BBMSG_TRIGGER9,
    BBMSG_TRIGGER10,
    BBMSG_TRIGGER11,
    BBMSG_TRIGGER12,
    BBMSG_TRIGGER13,
    BBMSG_TRIGGER14,
    BBMSG_TRIGGER15,
    BBMSG_TRIGGER16,
    BBMSG_TRIGGER17,
    BBMSG_TRIGGER18,
    BBMSG_TRIGGER19,
    BBMSG_TRIGGER20,
    BBMSG_TRIGGER21,
    BBMSG_TRIGGER22,
    BBMSG_TRIGGER23,
    BBMSG_TRIGGER24,
    BBMSG_TRIGGER25,
    BBMSG_TRIGGER26,
    BBMSG_TRIGGER27,
    BBMSG_TRIGGER28,
    BBMSG_TRIGGER29,
    BBMSG_TRIGGER30,
    BBMSG_TRIGGER31
} breadbox_message_t;

typedef struct {
    // The ambient light color
    vec4 ambient;
    // List of all the lights(breadbox_light_t) in the game
    breadbox_list_t lights;
    // List of all the props(breadbox_prop_t) in the game
    breadbox_list_t props;
    // The view matrix
    mat4 *view;
} breadbox_model_t;

typedef struct {
    // Configurable input axes
    float axes[32];
    // The height of the viewport
    int height;
    // Determine which events fire the update function
    // There's probably a better way to declare a 128-bit integer... ~Alex
    uint32_t mask[4];
    // Current game tick
    int tick;
    // The width of the viewport
    int width;
} breadbox_subscriptions_t;

typedef struct {
    breadbox_model_t model;
    breadbox_subscriptions_t subscriptions;
} breadbox_t;

// Recalculate the camera matrix based on its properties
void breadbox_camera_update(breadbox_camera_t *camera);

// Free resources before the game closes
void breadbox_cleanup(breadbox_t *engine);

// Converts a color from 32-bit RGBA into a vector
void breadbox_color_convert(vec4 victim, uint32_t color);

// Prints a debug message to the log
void breadbox_debug(const char *format, ...);

// Same as above for internal use only!
void breadbox_debug_internal(breadbox_log_source_t source, const char *format, ...);

// Prints an error message to the log
void breadbox_error(const char *format, ...);

// Same as above for internal use only!
void breadbox_error_internal(breadbox_log_source_t source, const char *format, ...);

// Frees a geometry object
void breadbox_geometry_free(breadbox_geometry_t *geometry);

// Initializes a geometry object
void breadbox_geometry_init(breadbox_geometry_t *geometry);

// Prints an informational message to the log
void breadbox_info(const char *format, ...);

// I think you get the idea at this point. ~Alex
void breadbox_info_internal(breadbox_log_source_t source, const char *format, ...);

// Initializes the game, not the engine.
void breadbox_init(breadbox_t *engine);

// Appends a node to the list
int breadbox_list_append(breadbox_list_t *list, void *data);

// Frees a list and all of its nodes
void breadbox_list_free(breadbox_list_t *list);

// Initializes a brand new list
void breadbox_list_init(breadbox_list_t *list);

// Removes a node from the list
void breadbox_list_remove(breadbox_list_t *list, int i);

// Prints a message to the log. It's recommended to use the debug, error, info,
// and warning functions to preserve your remaining sanity. ~Alex
void breadbox_log(breadbox_log_source_t source, breadbox_log_level_t level, const char *format, va_list args);

// Frees the model from memory
void breadbox_model_free(breadbox_model_t *model);

// Initializes the model
void breadbox_model_init(breadbox_model_t *model);

// This is the main way the engine is notified of events by the system. ~Alex
void breadbox_publish(breadbox_t *engine, breadbox_message_t msg);

// This function is used by the game itself to let the platform know that we're
// done and everything can be shut down. ~Alex
void breadbox_quit(void);

// Subscribes to a specific message type so the update function is fired when
// the event is received.
void breadbox_subscribe(breadbox_subscriptions_t *subs, breadbox_message_t type);

// Initializes engine subscriptions
void breadbox_subscription_init(breadbox_subscriptions_t *subs);

// This function is how we tell the game what's going on. This should only
// be called by the subscription module or messages might not be routed
// properly. Unless this is called from the subscription service, you should
// probably be using breadbox_publish instead. ~Alex
void breadbox_update(breadbox_t *engine, breadbox_message_t msg);

// Prints a warning message to the log
void breadbox_warning(const char *format, ...);

// The internal version of breadbox_warning
void breadbox_warning_internal(breadbox_log_source_t source, const char *format, ...);

#endif
