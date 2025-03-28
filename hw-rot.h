
#ifndef HW_ROT_H_INCLUDED
#define HW_ROT_H_INCLUDED

#include <stdlib.h>

#include "hw-rot/src/common/intf.h"
#include "hw-rot/src/sha3/sha3_shake_hw.h"
#include "hw-rot/src/sha2/sha2_hw.h"
#include "hw-rot/src/eddsa/eddsa_hw.h"
#include "hw-rot/src/x25519/x25519_hw.h"
#include "hw-rot/src/trng/trng_hw.h"
#include "hw-rot/src/aes/aes_hw.h"
#include "hw-rot/src/mac/mac_hw.h"
#include "hw-rot/src/puf/puf_hw.h"

//-- SHA-3 / SHAKE
#define sha3_512_hw			        sha3_512_hw_func
#define sha3_256_hw			        sha3_256_hw_func
#define shake_128_hw		        shake128_hw_func
#define shake_256_hw		        shake256_hw_func

#define sha_256_hw			        sha_256_hw_func
#define sha_384_hw			        sha_384_hw_func
#define sha_512_hw			        sha_512_hw_func
#define sha_512_256_hw		        sha_512_256_hw_func

//-- EdDSA25519
#define eddsa25519_genkeys_hw       eddsa25519_genkeys_hw
#define eddsa25519_sign_hw          eddsa25519_sign_hw
#define eddsa25519_verify_hw        eddsa25519_verify_hw

//-- X25519
#define x25519_genkeys_hw           x25519_genkeys_hw
#define x25519_ss_gen_hw            x25519_ss_gen_hw

//-- TRNG
#define trng_hw        			    trng_hw

//-- AES-128/192/256-ECB
#define aes_128_ecb_encrypt_hw      aes_128_ecb_encrypt_hw
#define aes_128_ecb_decrypt_hw      aes_128_ecb_decrypt_hw
#define aes_192_ecb_encrypt_hw      aes_192_ecb_encrypt_hw
#define aes_192_ecb_decrypt_hw      aes_192_ecb_decrypt_hw
#define aes_256_ecb_encrypt_hw      aes_256_ecb_encrypt_hw
#define aes_256_ecb_decrypt_hw      aes_256_ecb_decrypt_hw

//-- AES-128/192/256-CBC
#define aes_128_cbc_encrypt_hw      aes_128_cbc_encrypt_hw
#define aes_128_cbc_decrypt_hw      aes_128_cbc_decrypt_hw
#define aes_192_cbc_encrypt_hw      aes_192_cbc_encrypt_hw
#define aes_192_cbc_decrypt_hw      aes_192_cbc_decrypt_hw
#define aes_256_cbc_encrypt_hw      aes_256_cbc_encrypt_hw
#define aes_256_cbc_decrypt_hw      aes_256_cbc_decrypt_hw

//-- AES-128/192/256-CMAC
#define aes_128_cmac_hw             aes_128_cmac_hw
#define aes_192_cmac_hw             aes_192_cmac_hw
#define aes_256_cmac_hw             aes_256_cmac_hw

//-- AES-128/192/256-CCM-8
#define aes_128_ccm_8_encrypt_hw    aes_128_ccm_8_encrypt_hw
#define aes_128_ccm_8_decrypt_hw    aes_128_ccm_8_decrypt_hw
#define aes_192_ccm_8_encrypt_hw    aes_192_ccm_8_encrypt_hw
#define aes_192_ccm_8_decrypt_hw    aes_192_ccm_8_decrypt_hw
#define aes_256_ccm_8_encrypt_hw    aes_256_ccm_8_encrypt_hw
#define aes_256_ccm_8_decrypt_hw    aes_256_ccm_8_decrypt_hw

//-- AES-128/192/256-GCM
#define aes_128_gcm_encrypt_hw      aes_128_gcm_encrypt_hw
#define aes_128_gcm_decrypt_hw      aes_128_gcm_decrypt_hw
#define aes_192_gcm_encrypt_hw      aes_192_gcm_encrypt_hw
#define aes_192_gcm_decrypt_hw      aes_192_gcm_decrypt_hw
#define aes_256_gcm_encrypt_hw      aes_256_gcm_encrypt_hw
#define aes_256_gcm_decrypt_hw      aes_256_gcm_decrypt_hw

//-- MAC
#define hmac_sha2_256_hw            hmac_sha2_256
#define hmac_sha2_384_hw            hmac_sha2_384
#define hmac_sha2_512_hw            hmac_sha2_512
#define hmac_sha2_512_256_hw        hmac_sha2_512_256
#define hmac_sha3_256_hw            hmac_sha3_256
#define hmac_sha3_512_hw            hmac_sha3_512
#define kmac128_hw                  kmac128
#define kmac256_hw                  kmac256


//-- INTERFACE
#ifdef I2C
    #define INTF_ADDRESS            0x1A            //-- I2C_DEVICE_ADDRESS
    #define INTF_LENGTH		        0x40
#elif AXI
    // ------- MS2XL_BASEADDR ------- //
    #define INTF_LENGTH		        0x40

    #ifdef PYNQZ2
        #define INTF_ADDRESS		0x43C00000      //-- MS2XL_BASEADDR
    #elif ZCU104
        #define INTF_ADDRESS        0x00A0000000    //-- MS2XL_BASEADDR
    #else
        #define INTF_ADDRESS        0x0000000000
    #endif

    // ------- BITSTREAM_FILE ------- //
    #ifdef PYNQZ2
        #define BITSTREAM_AXI       "../hw-rot/bit/PYNQZ2_HW_ROT_1.0.bit"
    #elif ZCU104
        #define BITSTREAM_AXI       "../hw-rot/bit/ZCU104_HW_ROT_1.0.bit"
    #endif

    /* ------- FREQUENCIES DEFINITION ------- */
    #ifdef PYNQZ2
        #define FREQ_TYPICAL       100.0
        #define FREQ_SHA2          100.0
        #define FREQ_SHA3          100.0
        #define FREQ_EDDSA          60.0
        #define FREQ_X25519         80.0
        #define FREQ_MLKEM         100.0
        #define FREQ_AES           100.0
    #elif ZCU104
        #define FREQ_TYPICAL       450.0
        #define FREQ_SHA2          375.0
        #define FREQ_SHA3          450.0
        #define FREQ_EDDSA         320.0
        #define FREQ_X25519        320.0
        #define FREQ_MLKEM         320.0
        #define FREQ_AES           375.0
    #endif
#endif

#endif 