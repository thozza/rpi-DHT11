/*
 * dht11.c - Simple C library implementation for DHT11 sensor using libbcm2835
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

#include <bcm2835.h>
#include <stdio.h>
#include <stdint.h>
#include <dht11.h>

#define DEBUG 0

const char* DHT11_result2string(int result)
{
	switch (result) {
		case DHT11_SUCCESS:
			return "Success";
		case DHT11_ERROR:
			return "Error";
		case DHT11_CHECKSUM:
			return "Checksum mismatch";
		case DHT11_TIMEOUT:
			return "Timeout";
		default:
			return "Unknown result";
	}
}

int DHT11_read_data(int pin, struct DHT11_data *data)
{
	int i;
	int loops;
	uint64_t *raw_data = (uint64_t *)data;
#if(DEBUG)
	int val_change = 0;
	int loops_cnt[83] = {};
#endif

	if (pin < 0 || data == NULL) {
		return DHT11_ERROR;
	}

	// send start signal
	bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(pin, LOW);
	bcm2835_delay(18);

	bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_INPT);

	// receive 3bit of start sequence + 40bit of data
	for (i = 0; i < 83; i++) {
		loops = 0;
		while ((i & 0x01) ? !bcm2835_gpio_lev(pin) : bcm2835_gpio_lev(pin)) {
			++loops;
			if (loops > 1000) {
				return DHT11_TIMEOUT;
			}
		}
#if(DEBUG)
		loops_cnt[val_change++] = loops;
#endif	
		if ((i > 2) && !(i & 0x01)) {
			*raw_data <<= 1;

			if (loops > 150)
				*raw_data |= 0x01;
		}
	}

#if(DEBUG)
	for (i = 0; i < val_change; i++)
	{
		if (i == 3)
			printf("\n");
		printf("%d\tchange -> loops=%d", i, loops_cnt[i]);
		if ((i > 2) && !(i & 0x01)) {
			if (loops_cnt[i] > 150)
				printf(" (got 1)");
			else
				printf(" (got 0)");
		}
		printf("\n");
	}

	printf("%#.2x %#.2x %#.2x %#.2x %#.2x\n", data->hum_int, data->hum_dec, data->temp_int, data->temp_dec, data->checksum);
#endif
	// check the checksum
	if (data->hum_int + data->hum_dec + data->temp_int + data->temp_dec != data->checksum)
		return DHT11_CHECKSUM;

	return DHT11_SUCCESS;
}
