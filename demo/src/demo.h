
#ifndef DEMO_H
#define DEMO_H

#ifdef HWROT_INST
#include <hw-rot.h>
#else
#include "../../hw-rot.h"
#endif // HWROT_INST

#include "test_func.h"

void demo_eddsa_hw(unsigned int mode, unsigned int verb, INTF interface);
void demo_x25519_hw(unsigned int mode, unsigned int verb, INTF interface);
void demo_aes_hw(unsigned int bits, unsigned int verb, INTF interface);
void demo_sha2_hw(unsigned int verb, INTF interface);
void demo_sha3_hw(unsigned int verb, INTF interface);
void demo_trng_hw(unsigned int bits, unsigned verb, INTF interface);

// test - speed
void test_aes_hw(unsigned char mode[4], unsigned int bits, unsigned int n_test, unsigned int verb, time_result* tr_en, time_result* tr_de, INTF interface);
void test_sha3_hw(unsigned int sel, unsigned int n_test, time_result* tr, unsigned int verb, INTF interface);
void test_sha2_hw(unsigned int sel, unsigned int n_test, time_result* tr, unsigned int verb, INTF interface);
void test_eddsa_hw(unsigned int mode, unsigned int n_test, unsigned int verb, time_result* tr_kg, time_result* tr_si, time_result* tr_ve, INTF interface);
void test_x25519_hw(unsigned int mode, unsigned int n_test, unsigned int verb, time_result* tr_kg, time_result* tr_ss, INTF interface);
void test_trng_hw(unsigned int mode, unsigned int bits, unsigned int n_test, time_result* tr, unsigned int verb, INTF interface);

#endif