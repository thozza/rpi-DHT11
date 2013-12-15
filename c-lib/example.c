/*
 * example.c - Simple example application using libbcm2835 and dht11 lib
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
 */

#include <bcm2835.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <dht11.h>

#define PIN 4	// use GPIO 4 pin

int main(int argc, char **argv)
{
	int res;
	struct DHT11_data data = {};

	if (!bcm2835_init()) {
		perror("Initialization of bcm2835 library failed!\n");
		return EXIT_FAILURE;
	}

	res = DHT11_read_data(PIN, &data);

	printf("Result: %s\n", DHT11_result2string(res));
	if (res == DHT11_SUCCESS)
		printf("Hum_int=%d Hum_dec=%d Tmp_int=%d Tmp_dec=%d CHsum=%d\n", data.hum_int, data.hum_dec, data.temp_int, data.temp_dec, data.checksum);

	if (!bcm2835_close()) {
		perror("Failed to close bcm2835 library.\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
