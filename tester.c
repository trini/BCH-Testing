/*
 * The goal here is to take a known set of data, bits of error correction
 * and remainders and determine what the polynomial used in the calculation
 * used was, by brute force.
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <linux/bch.h>

/*
 * XXX: TODO: BIG WARNING
 *
 * In the SW only test we know how the phrase 'apply mask so that an erased
 * page is a valid codeword' works and what the values being used are.  Must
 * ensure we do that right for the HW equivalent.
 *
 * XXX: TODO: BIG WARNING
 */

#define ECCSIZE		512
#define ECCBYTES	13

extern const uint16_t data[];
extern const uint16_t ecc_data[];

static uint8_t generated_ecc_data[ECCBYTES];

int main (int argc, char **argv) {
	struct bch_control *bch = NULL;
	int i;


	/* Ensure our buffer is empty */
	memset(&generated_ecc_data, 0, sizeof(generated_ecc_data));

	bch = init_bch(CONFIG_BCH_CONST_M, CONFIG_BCH_CONST_T, 0);

	encode_bch(bch, data, ECCSIZE, generated_ecc_data);
	for (i = 0; i < ECCBYTES; i++)
		printf("0x%x ", generated_ecc_data[i]);
	printf("\n");

	return 0;
}
