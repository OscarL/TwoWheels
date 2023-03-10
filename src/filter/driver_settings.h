// CRIPPLED VERSION (just what I need for the filter) of:

/* driver_settings - implements the driver settings API
**
** Initial version by Axel Dörfler, axeld@pinc-software.de
** This file may be used under the terms of the OpenBeOS License.
*/

#ifndef _DRIVER_SETTINGS_H
#define _DRIVER_SETTINGS_H

#include <OS.h>

typedef struct driver_parameter {
	char* name;
	int	value_count;
	char** values;
	int parameter_count;
	struct driver_parameter* parameters;
} driver_parameter;

typedef struct driver_settings {
	int parameter_count;
	struct driver_parameter* parameters;
} driver_settings;

#ifdef __cplusplus
extern "C" {
#endif

extern void*	load_driver_settings_from_path(const char *path);
extern status_t	unload_driver_settings(void* handle);

extern const char* get_driver_parameter(void *handle, const char* key, const char* unknownValue, const char* noargValue);
extern bool	get_driver_boolean_parameter(void* handle, const char* key, bool unknownValue, bool noargValue);

#ifdef __cplusplus
}
#endif

#endif	// _DRIVER_SETTINGS_H
