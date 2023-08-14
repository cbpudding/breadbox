#include "breadbox.h"

void breadbox_debug(const char *format, ...) {
    va_list args;
    va_start(args, format);
    breadbox_log(BBLOG_GAME, BBLOG_DEBUG, format, args);
    va_end(args);
}

void breadbox_debug_internal(breadbox_log_source_t source, const char *format, ...) {
    va_list args;
    va_start(args, format);
    breadbox_log(source, BBLOG_DEBUG, format, args);
    va_end(args);
}

void breadbox_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    breadbox_log(BBLOG_GAME, BBLOG_ERROR, format, args);
    va_end(args);
}

void breadbox_error_internal(breadbox_log_source_t source, const char *format, ...) {
    va_list args;
    va_start(args, format);
    breadbox_log(source, BBLOG_ERROR, format, args);
    va_end(args);
}

void breadbox_info(const char *format, ...) {
    va_list args;
    va_start(args, format);
    breadbox_log(BBLOG_GAME, BBLOG_INFO, format, args);
    va_end(args);
}

void breadbox_info_internal(breadbox_log_source_t source, const char *format, ...) {
    va_list args;
    va_start(args, format);
    breadbox_log(source, BBLOG_INFO, format, args);
    va_end(args);
}

void breadbox_warning(const char *format, ...) {
    va_list args;
    va_start(args, format);
    breadbox_log(BBLOG_GAME, BBLOG_WARNING, format, args);
    va_end(args);
}

void breadbox_warning_internal(breadbox_log_source_t source, const char *format, ...) {
    va_list args;
    va_start(args, format);
    breadbox_log(source, BBLOG_WARNING, format, args);
    va_end(args);
}
