
#include "demo.h"
#include "test_func.h"


static void demo_sha2_perf_hw(unsigned int sel, unsigned char* input, unsigned int len_input, unsigned char* md, INTF interface) {

    if (sel == 0)           sha_256_hw(input, len_input, md, interface);
    else if (sel == 1)      sha_384_hw(input, len_input, md, interface);
    else if (sel == 2)      sha_512_hw(input, len_input, md, interface);
    else if (sel == 3)      sha_512_256_hw(input, len_input, md, interface);
    // else if (sel == 4)      sha_224(input, len_input, md);
    // else if (sel == 5)      sha_512_224(input, len_input, md);

}


void test_sha2_hw(unsigned int sel, unsigned int n_test, time_result* tr, unsigned int verb, INTF interface) {

#ifdef AXI
    unsigned int clk_index = 0;
    float clk_frequency;
    float set_clk_frequency = FREQ_SHA2;
    Set_Clk_Freq(clk_index, &clk_frequency, &set_clk_frequency, (int)verb);
#endif

    srand(time(NULL));   // Initialization, should only be called once.

    uint64_t start_t, stop_t;

    //-- Initialize to avoid 1st measure error
    start_t = timeInMicroseconds();
    stop_t = timeInMicroseconds();

    tr->time_mean_value = 0;
    tr->time_max_value = 0;
    tr->time_min_value = 0;
    tr->val_result = 0xFFFFFFFF;

    uint64_t time_hw = 0;
    uint64_t time_total_hw = 0;

    /*
    if (sel == 0)        printf("\n\n -- Test SHA-256 --");
    else if (sel == 1)   printf("\n\n -- Test SHA-384 --");
    else if (sel == 2)   printf("\n\n -- Test SHA-512 --");
    else if (sel == 3)   printf("\n\n -- Test SHA-512/256 --");
    else if (sel == 4)   printf("\n\n -- Test SHA-224 --");
    else if (sel == 5)   printf("\n\n -- Test SHA-512/224 --");
    */

    int buf_len = 1000;

    unsigned char md[64];
    unsigned char buf[buf_len];

    // buf = malloc(1024);
    // md  = malloc(256);
    // md1 = malloc(256);

    for (unsigned int test = 1; test <= n_test; test++) {
        // int r = rand() % buf_len;// 100000;
        int r = 64;

        for (int i = 0; i < r; i++)
        {
            buf[i] = rand();
        }

        // ctr_drbg(buf, r);
        // trng_hw(buf, r, interface);

        memset(md, 0, 64);

        if (verb >= 2) printf("\n test: %d \t bytes: %d", test, r);

        start_t = timeInMicroseconds();
        demo_sha2_perf_hw(sel, buf, r, md, interface);
        stop_t = timeInMicroseconds(); if (verb >= 2) printf("\n SW: ET: %.3f s \t %.3f ms \t %d us", (stop_t - start_t) / 1000000.0, (stop_t - start_t) / 1000.0, (unsigned int)(stop_t - start_t));

        if (verb >= 1) show_array(md, 64, 32);

        time_hw = stop_t - start_t;
        time_total_hw += time_hw;

        if (test == 1)                                  tr->time_min_value = time_hw;
        else if (tr->time_min_value > time_hw)       tr->time_min_value = time_hw;
        if (tr->time_max_value < time_hw)            tr->time_max_value = time_hw;

    }

    tr->time_mean_value = (uint64_t)(time_total_hw / n_test);

    // free(buf);
    // free(md);
    // free(md1);

#ifdef AXI
    set_clk_frequency = FREQ_TYPICAL;
    Set_Clk_Freq(clk_index, &clk_frequency, &set_clk_frequency, (int)verb);
#endif

}