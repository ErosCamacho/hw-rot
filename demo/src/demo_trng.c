
#include "demo.h"
#include "test_func.h"

void demo_trng_hw(unsigned int bits, unsigned verb, INTF interface) {

    unsigned int bytes = (int)(bits / 8);
    unsigned char* random; 
    random = malloc(bytes);

    unsigned char buf[20]; sprintf(buf, "%d bits", bits); 
    memset(random, 0, bytes);

    trng_hw(random, bytes, interface);

    if (verb >= 1) {
        printf("\n TRNG Random %d bits: ", bits);  show_array(random, bytes, 32);
    }

    print_result_double_valid("TRNG", buf, test_random(random, bytes));
    /*
    memset(random, 0, bytes);

    ctr_drbg(random, bytes);

    if (verb >= 1) {
        printf("\n CTR-DRBG Random %d bits: ", bits);  show_array(random, bytes, 32);
    }

    print_result_double_valid("CTR-DRBG", buf, test_random(random, bytes));

    memset(random, 0, bytes);

    hash_drbg(random, bytes);

    if (verb >= 1) {
        printf("\n HASH-DRBG Random %d bits: ", bits);  show_array(random, bytes, 32);
    }

    print_result_double_valid("HASH-DRBG", buf, test_random(random, bytes));
    */
}