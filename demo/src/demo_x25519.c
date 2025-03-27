
#include "demo.h"
#include "test_func.h"

void demo_x25519_hw(unsigned int mode, unsigned int verb, INTF interface) {

#ifdef AXI
    unsigned int clk_index = 0;
    float clk_frequency;
    float set_clk_frequency = FREQ_X25519;
    Set_Clk_Freq(clk_index, &clk_frequency, &set_clk_frequency, (int) verb);
#endif

    seed_rng();

    // ---- KEY GEN ---- //
    unsigned char* pub_key_A;
    unsigned char* pri_key_A;
    unsigned int pub_len_A;
    unsigned int pri_len_A;

    if (mode == 25519)   x25519_genkeys_hw(&pri_key_A, &pub_key_A, &pri_len_A, &pub_len_A, interface);
    // else                x448_genkeys(&pri_key_A, &pub_key_A, &pri_len_A, &pub_len_A);

    if (verb >= 2) printf("\n pub_len: %d (bytes)", pub_len_A);
    if (verb >= 2) printf("\n pri_len: %d (bytes)", pri_len_A);

    if (verb >= 3) { printf("\n public key: ");   show_array(pub_key_A, pub_len_A, 32); }
    if (verb >= 3) { printf("\n private key: "); show_array(pri_key_A, pri_len_A, 32); }

    unsigned char* pub_key_B;
    unsigned char* pri_key_B;
    unsigned int pub_len_B;
    unsigned int pri_len_B;
 
    if (mode == 25519)   x25519_genkeys_hw(&pri_key_B, &pub_key_B, &pri_len_B, &pub_len_B, interface);
    // else                x448_genkeys(&pri_key_B, &pub_key_B, &pri_len_B, &pub_len_B);

    if (verb >= 2) printf("\n pub_len: %d (bytes)", pub_len_B);
    if (verb >= 2) printf("\n pri_len: %d (bytes)", pri_len_B);

    if (verb >= 3) { printf("\n public key: ");   show_array(pub_key_B, pub_len_B, 32); }
    if (verb >= 3) { printf("\n private key: "); show_array(pri_key_B, pri_len_B, 32); }

    // --- SHARED_SECRET --- //

    unsigned char* ss_A;
    unsigned int ss_len_A;
    if (mode == 25519)
        x25519_ss_gen_hw(&ss_A, &ss_len_A, pub_key_B, pub_len_B, pri_key_A, pri_len_A, interface); // A Side
    // else
        // x448_ss_gen(&ss_A, &ss_len_A, (const unsigned char*)pub_key_B, pub_len_B, (const unsigned char*)pri_key_A, pri_len_A); // A Side

    unsigned char* ss_B;
    unsigned int ss_len_B;
    if (mode == 25519)
        x25519_ss_gen_hw(&ss_B, &ss_len_B, pub_key_A, pub_len_A, pri_key_B, pri_len_B, interface); // B Side
    // else
        // x448_ss_gen(&ss_B, &ss_len_B, (const unsigned char*)pub_key_A, pub_len_A, (const unsigned char*)pri_key_B, pri_len_B); // B Side

    if (verb >= 2) printf("\n ss_len_A: %d (bytes)", ss_len_A);
    if (verb >= 3) { printf("\n ss_A: ");   show_array(ss_A, ss_len_A, 32); }

    if (verb >= 2) printf("\n ss_len_B: %d (bytes)", ss_len_B);
    if (verb >= 3) { printf("\n ss_B: ");   show_array(ss_B, ss_len_B, 32); }

    unsigned char s_mode[20];
    if (mode == 25519)  sprintf(s_mode, "%s", "X25519 KEM");
    // else                sprintf(s_mode, "%s", "X448 KEM");

    print_result_valid(s_mode, memcmp(ss_A, ss_B, ss_len_A));

#ifdef AXI
    set_clk_frequency = FREQ_TYPICAL;
    Set_Clk_Freq(clk_index, &clk_frequency, &set_clk_frequency, (int)verb);
#endif
}