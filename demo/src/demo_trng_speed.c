 
#include "demo.h"
#include "test_func.h"

void test_trng_hw(unsigned int mode, unsigned int bits, unsigned int n_test, time_result* tr, unsigned int verb, INTF interface)
{
    unsigned int bytes = (int)(bits / 8);
    unsigned char* random_trng; random_trng = malloc(bytes);
    unsigned char* random_ctr;  random_ctr = malloc(bytes);
    unsigned char* random_hmac;  random_hmac = malloc(bytes);

    uint64_t start_t, stop_t;

    //-- Initialize to avoid 1st measure error
    start_t = timeInMicroseconds();
    stop_t = timeInMicroseconds();

    tr->time_mean_value = 0;
    tr->time_max_value = 0;
    tr->time_min_value = 0;
    tr->val_result = 0;

    uint64_t time_hw = 0;
    uint64_t time_total_hw = 0;

    /*
    if (mode == 0)        printf("\n\n -- Test TRNG %d bits --", bits);
    else if (mode == 1)   printf("\n\n -- Test CTR-DRBG %d bits --", bits);
    else if (mode == 2)   printf("\n\n -- Test HASH-DRBG %d bits --", bits);
    */

    for (unsigned int test = 1; test <= n_test; test++) {

        if (mode == 0) {
            start_t = timeInMicroseconds();
            trng_hw(random_trng, bytes, interface); // from crypto_api_sw.h
            stop_t = timeInMicroseconds(); if (verb >= 1) printf("\n SW: ET: %.3f s \t %.3f ms \t %d us", (stop_t - start_t) / 1000000.0, (stop_t - start_t) / 1000.0, (unsigned int)(stop_t - start_t));
            if (!test_random(random_trng, bytes)) tr->val_result++;
            if (verb >= 2) show_array(random_trng, bytes, 32);
        }
        else if (mode == 1) {
        /*
            start_t = timeInMicroseconds();
            ctr_drbg(random_ctr, bytes); // from crypto_api_sw.h
            stop_t = timeInMicroseconds(); if (verb >= 1) printf("\n SW: ET: %.3f s \t %.3f ms \t %d us", (stop_t - start_t) / 1000000.0, (stop_t - start_t) / 1000.0, (unsigned int)(stop_t - start_t));
            if (!test_random(random_ctr, bytes)) tr->val_result++;
            if (verb >= 2) show_array(random_ctr, bytes, 32);
        */
        }
        else if (mode == 2) {
        /*    
            start_t = timeInMicroseconds();
            hash_drbg(random_hmac, bytes); // from crypto_api_sw.h
            stop_t = timeInMicroseconds(); if (verb >= 1) printf("\n SW: ET: %.3f s \t %.3f ms \t %d us", (stop_t - start_t) / 1000000.0, (stop_t - start_t) / 1000.0, (unsigned int)(stop_t - start_t));
            if (!test_random(random_hmac, bytes)) tr->val_result++;
            if (verb >= 2) show_array(random_hmac, bytes, 32);
        */
        }

        time_hw = stop_t - start_t;
        time_total_hw += time_hw;

        if (test == 1)                               tr->time_min_value = time_hw;
        else if (tr->time_min_value > time_hw)    tr->time_min_value = time_hw;

        if (tr->time_max_value < time_hw)         tr->time_max_value = time_hw;


    }

    tr->time_mean_value = (uint64_t)(time_total_hw / n_test);

    free(random_trng);
    free(random_ctr);
    free(random_hmac);
}
