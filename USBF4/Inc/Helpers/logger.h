/*
 * logger.h
 *
 *  Created on: Mar 28, 2021
 *      Author: Avinash J
 */

#ifndef HELPERS_LOGGER_H_
#define HELPERS_LOGGER_H_

#include <stdint.h>

typedef enum
{
	LOG_LEVEL_ERROR,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG
}LogLvl;

extern LogLvl system_log_level;

void log_error(char const * const format, ...);
void log_info(char const * const format, ...);
void log_debug(char const * const format, ...);
void log_debug_array(char const * const label, void const *array, uint16_t const len);


#endif /* HELPERS_LOGGER_H_ */
