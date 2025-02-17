/*
 * convert.h
 *
 *  Created on: Feb 17, 2025
 *      Author: paulb
 */

#ifndef INC_TASKS_HEADERS_DECODE_H_
#define INC_TASKS_HEADERS_DECODE_H_

float decode_float16_to_float32(uint16_t float16_value);
uint16_t decode_uint16_t( uint8_t* addr );

#endif /* INC_TASKS_HEADERS_DECODE_H_ */
