/*
 * The goal here is to take a known set of data, bits of error correction
 * and remainders and determine what the polynomial used in the calculation
 * used was, by brute force.
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <linux/bch.h>

#include "misc.h"

/*
 * XXX: TODO: BIG WARNING
 *
 * In the SW only test we know how the phrase 'apply mask so that an erased
 * page is a valid codeword' works and what the values being used are.  Must
 * ensure we do that right for the HW equivalent.
 *
 * In SW matching we take data, run algo, take results and ^ with the matching
 * part of running our algo over erased (ff) page.  We need to do the same in
 * our cycling.
 *
 * XXX: TODO: BIG WARNING
 */

int main (int argc, char **argv) {
	struct bch_control *bch = NULL;
	int i;
	unsigned int guessed_prim_poly;
	unsigned char erased_page[ECCSIZE];
	uint8_t generated_ecc_data[ECCBYTES];
	uint8_t generated_ecc_mask[ECCBYTES];

	if (argc != 2) {
		printf("Usage: %s prim_poly\n", argv[0]);
		exit(1);
	}

	guessed_prim_poly = strtoul(argv[1], NULL, 10);
	if (!guessed_prim_poly) {
		printf("Usage: %s prim_poly\n", argv[0]);
		exit(1);
	}

	memset(erased_page, 0xff, ECCSIZE);

	/* Ensure our buffer are sane */
	memset(&generated_ecc_data, 0, sizeof(generated_ecc_data));
	memset(&generated_ecc_mask, 0, sizeof(generated_ecc_mask));

	bch = init_bch(CONFIG_BCH_CONST_M, CONFIG_BCH_CONST_T,
			guessed_prim_poly);

	/* Get the erased page mask */
	encode_bch(bch, erased_page, ECCSIZE, generated_ecc_mask);
	for (i = 0; i < ECCBYTES; i++)
		generated_ecc_mask[i] ^= 0xff;

	/* Encode the data */
	encode_bch(bch, data, ECCSIZE, generated_ecc_data);

	/* Apply the mask */
	for (i = 0; i < ECCBYTES; i++)
		generated_ecc_data[i] ^= generated_ecc_mask[i];
	if (memcmp(generated_ecc_data, ecc_data, ECCBYTES) == 0) {
		printf("Match with %d\n", guessed_prim_poly);
		exit(0);
	}

	return 1;
}
