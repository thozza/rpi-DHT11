/*
 * dht11.h - Simple C library header for DHT11 sensor using libbcm2835
 * Copyright 2013 Tomas Hozza <thozza@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 * Tomas Hozza <thozza@gmail.com>
 *
 * Data sheet: http://www.micro4you.com/files/sensor/DHT11.pdf
 */

#include <stdint.h>

#ifndef DHT11_C__H
#define DHT11_C__H

#define DHT11_SUCCESS	0
#define DHT11_ERROR		1
#define DHT11_CHECKSUM	2
#define DHT11_TIMEOUT	3

struct DHT11_data
{
	uint8_t checksum;
	uint8_t temp_dec;
	uint8_t temp_int;
	uint8_t hum_dec;
	uint8_t hum_int;
};

int DHT11_read_data(int pin, struct DHT11_data *data);
const char* DHT11_result2string(int result);

#endif /* DHT11_C__H */
