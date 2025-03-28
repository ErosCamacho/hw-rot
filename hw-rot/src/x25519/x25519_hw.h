#ifndef X25519_H
#define X25519_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/intf.h"
#include "../common/conf.h"
#include "../common/extra_func.h"

//-- Elements Bit Sizes
#define X25519_BYTES          32
#define AXI_BYTES              8

//-- Control Operations
#define X25519_RST_OFF      0x00
#define X25519_RST_ON       0x01
#define X25519_INTF_RST     0x02
#define X25519_INTF_OPER    0x00
#define X25519_INTF_LOAD    0x04
#define X25519_INTF_READ    0x08

//-- I/O Addresses
#define X25519_SCALAR      0x0
#define X25519_POINT_IN    0x4
#define X25519_POINT_OUT   0x0

//-- Debug
#ifdef I2C
    #define X25519_WAIT_TIME    50
#else
    #define X25519_WAIT_TIME    5000
#endif
#define X25519_N_ITER       1000

//-- INTERFACE INIT/START & READ/WRITE
void x25519_init(INTF interface);
void x25519_start(INTF interface);
void x25519_write(unsigned long long address, unsigned long long size, void *data, unsigned long long reset, INTF interface);
void x25519_read(unsigned long long address, unsigned long long size, void *data, INTF interface);

//-- GENERATE PUBLIC KEY
void x25519_genkeys_hw(unsigned char **pri_key, unsigned char **pub_key, unsigned int *pri_len, unsigned int *pub_len, INTF interface);

//-- ECDH X25519 OPERATION
void x25519_ss_gen_hw(unsigned char **shared_secret, unsigned int *shared_secret_len, unsigned char *pub_key, unsigned int pub_len, unsigned char *pri_key, unsigned int pri_len, INTF interface);

#endif