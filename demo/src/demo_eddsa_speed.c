 
#include "demo.h"
#include "test_func.h"

void test_eddsa_hw(unsigned int mode, unsigned int n_test, unsigned int verb, time_result* tr_kg, time_result* tr_si, time_result* tr_ve, INTF interface)
{

#ifdef AXI
	unsigned int clk_index = 0;
	float clk_frequency;
	float set_clk_frequency = FREQ_EDDSA;
    Set_Clk_Freq(clk_index, &clk_frequency, &set_clk_frequency, (int) verb);
#endif

    seed_rng();

    uint64_t start_t, stop_t;

    //-- Initialize to avoid 1st measure error
    start_t = timeInMicroseconds();
    stop_t = timeInMicroseconds();

    tr_kg->time_mean_value = 0;
    tr_kg->time_max_value = 0;
    tr_kg->time_min_value = 0;
    tr_kg->val_result = 0;

    tr_si->time_mean_value = 0;
    tr_si->time_max_value = 0;
    tr_si->time_min_value = 0;
    tr_si->val_result = 0;

    tr_ve->time_mean_value = 0;
    tr_ve->time_max_value = 0;
    tr_ve->time_min_value = 0;
    tr_ve->val_result = 0;

    uint64_t time_hw = 0;
    uint64_t time_total_kg_hw = 0;
    uint64_t time_total_ve_hw = 0;
    uint64_t time_total_si_hw = 0;

    unsigned char* pub_key;
    unsigned char* pri_key;
    unsigned int pub_len;
    unsigned int pri_len;
    unsigned char msg[] = "Hello, this is the HW RoT";
    unsigned char* sig;
    unsigned int sig_len;
    unsigned int result = 1;

    /*
    if (mode == 25519)          printf("\n\n -- Test EdDSA-25519 --");
    if (mode == 448)            printf("\n\n -- Test EdDSA-448 --");
    */

    for (int test = 1; test <= n_test; test++) {

        if (verb >= 1) printf("\n test: %d", test);

        result = 1;
        
        // ---- EDDSA ---- //
        if (mode == 25519)
        {

            // -----------------
            // keygen_sw
            start_t = timeInMicroseconds();
            eddsa25519_genkeys_hw(&pri_key, &pub_key, &pri_len, &pub_len, interface); 
            stop_t = timeInMicroseconds(); if (verb >= 1) printf("\n SW GEN KEYS: ET: %.3f s \t %.3f ms \t %d us", (stop_t - start_t) / 1000000.0, (stop_t - start_t) / 1000.0, (unsigned int)(stop_t - start_t));

            time_hw = stop_t - start_t;
            time_total_kg_hw += time_hw;

            if (test == 1)										tr_kg->time_min_value = time_hw;
            else if (tr_kg->time_min_value > time_hw)		tr_kg->time_min_value = time_hw;
            if (tr_kg->time_max_value < time_hw)				tr_kg->time_max_value = time_hw;

            if (verb >= 2)
                printf("\n pub_len: %d (bytes)", pub_len);
            if (verb >= 2)
                printf("\n pri_len: %d (bytes)", pri_len);

            if (verb >= 3)
            {
                printf("\n public key: ");
                show_array(pub_key, pub_len, 32);
            }
            if (verb >= 3)
            {
                printf("\n private key: ");
                show_array(pri_key, pri_len, 32);
            }

            // sign_hw
            start_t = timeInMicroseconds();
            eddsa25519_sign_hw(msg, strlen(msg), pri_key, pri_len, pub_key, pub_len, &sig, &sig_len, interface);
            stop_t = timeInMicroseconds(); if (verb >= 1) printf("\n SW SIGN: ET: %.3f s \t %.3f ms \t %d us", (stop_t - start_t) / 1000000.0, (stop_t - start_t) / 1000.0, (unsigned int)(stop_t - start_t));

            time_hw = stop_t - start_t;
            time_total_si_hw += time_hw;

            if (test == 1)										tr_si->time_min_value = time_hw;
            else if (tr_si->time_min_value > time_hw)		tr_si->time_min_value = time_hw;
            if (tr_si->time_max_value < time_hw)				tr_si->time_max_value = time_hw;

            if (verb >= 3)
            {
                printf("\n signature: ");
                show_array(sig, sig_len, 32);
            }

            // dec_hw

            start_t = timeInMicroseconds();
            eddsa25519_verify_hw(msg, strlen(msg), pub_key, pub_len, sig, sig_len, &result, interface);
            stop_t = timeInMicroseconds(); if (verb >= 1) printf("\n SW VERIFY: ET: %.3f s \t %.3f ms \t %d us", (stop_t - start_t) / 1000000.0, (stop_t - start_t) / 1000.0, (unsigned int)(stop_t - start_t));

            time_hw = stop_t - start_t;
            time_total_ve_hw += time_hw;

            if (test == 1)										tr_ve->time_min_value = time_hw;
            else if (tr_ve->time_min_value > time_hw)		tr_ve->time_min_value = time_hw;
            if (tr_ve->time_max_value < time_hw)				tr_ve->time_max_value = time_hw;

            if (result) tr_ve->val_result++;

        }

        // ---- EDDSA ---- //
        if (mode == 448)
        {
            /*
            // -----------------
            // keygen_sw
            start_t = timeInMicroseconds();
            eddsa448_genkeys(&pri_key, &pub_key, &pri_len, &pub_len); // from crypto_api_sw.h
            stop_t = timeInMicroseconds(); if (verb >= 1) printf("\n SW GEN KEYS: ET: %.3f s \t %.3f ms \t %d us", (stop_t - start_t) / 1000000.0, (stop_t - start_t) / 1000.0, (unsigned int)(stop_t - start_t));

            time_hw = stop_t - start_t;
            time_total_kg_hw += time_hw;

            if (test == 1)										tr_kg->time_min_value = time_hw;
            else if (tr_kg->time_min_value > time_hw)		tr_kg->time_min_value = time_hw;
            if (tr_kg->time_max_value < time_hw)				tr_kg->time_max_value = time_hw;

            if (verb >= 2)
                printf("\n pub_len: %d (bytes)", pub_len);
            if (verb >= 2)
                printf("\n pri_len: %d (bytes)", pri_len);

            if (verb >= 3)
            {
                printf("\n public key: ");
                show_array(pub_key, pub_len, 32);
            }
            if (verb >= 3)
            {
                printf("\n private key: ");
                show_array(pri_key, pri_len, 32);
            }

            // sign_hw
            start_t = timeInMicroseconds();
            eddsa448_sign(msg, strlen(msg), (const unsigned char*)pri_key, pri_len, &sig, &sig_len); // from crypto_api_sw.h
            stop_t = timeInMicroseconds(); if (verb >= 1) printf("\n SW SIGN: ET: %.3f s \t %.3f ms \t %d us", (stop_t - start_t) / 1000000.0, (stop_t - start_t) / 1000.0, (unsigned int)(stop_t - start_t));

            time_hw = stop_t - start_t;
            time_total_si_hw += time_hw;

            if (test == 1)										tr_si->time_min_value = time_hw;
            else if (tr_si->time_min_value > time_hw)		tr_si->time_min_value = time_hw;
            if (tr_si->time_max_value < time_hw)				tr_si->time_max_value = time_hw;

            if (verb >= 3)
            {
                printf("\n signature: ");
                show_array(sig, sig_len, 32);
            }

            // dec_hw

            start_t = timeInMicroseconds();
            eddsa448_verify(msg, strlen(msg), (const unsigned char*)pub_key, pub_len, (const unsigned char*)sig, sig_len, &result);
            stop_t = timeInMicroseconds(); if (verb >= 1) printf("\n SW VERIFY: ET: %.3f s \t %.3f ms \t %d us", (stop_t - start_t) / 1000000.0, (stop_t - start_t) / 1000.0, (unsigned int)(stop_t - start_t));

            time_hw = stop_t - start_t;
            time_total_ve_hw += time_hw;

            if (test == 1)										tr_ve->time_min_value = time_hw;
            else if (tr_ve->time_min_value > time_hw)		tr_ve->time_min_value = time_hw;
            if (tr_ve->time_max_value < time_hw)				tr_ve->time_max_value = time_hw;

            if (!result) tr_ve->val_result++;
        */
        }

    }

    tr_kg->time_mean_value = (uint64_t)(time_total_kg_hw / n_test);
    tr_si->time_mean_value = (uint64_t)(time_total_si_hw / n_test);
    tr_ve->time_mean_value = (uint64_t)(time_total_ve_hw / n_test);
    
#ifdef AXI
    set_clk_frequency = FREQ_TYPICAL;
    Set_Clk_Freq(clk_index, &clk_frequency, &set_clk_frequency, (int)verb);
#endif
}