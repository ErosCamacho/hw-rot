
#include "../hw-rot.h"

void main() {
	
	// --- Open Interface --- //
	INTF RoT;
	open_INTF(&RoT, INTF_ADDRESS, INTF_LENGTH);

#ifdef AXI
	// --- Loading Bitstream --- //
	load_bitstream(BITSTREAM_AXI);
#endif

	// PUF_KeyGen
	unsigned char key[32]; for (int i = 0; i < 32; i++) key[i] = i;
	puf_gen_enrollment(&RoT, 15, 256);
	unsigned char HelpData[32 * 15];
	puf_HelpDataGen(&RoT, 0, 15, key, HelpData);
	unsigned char key2[32];
	puf_RetryKey(&RoT, 15, key2, HelpData);

	if (!memcmp(key, key2,  32)) printf("PUF Key Gen: SUCCESS");
	else printf("PUF Key Gen: FAIL");

	// Test MAC
	unsigned char msg[] = "FPL 2025";
	unsigned char mac[128];
	
	memset(mac, 0, 128);
	hmac_sha2_256_hw(key, msg, mac, RoT); printf("\n");  for (int i = 0; i < 128; i++) printf("%02x", mac[i]);
	memset(mac, 0, 128);
	hmac_sha2_384_hw(key, msg, mac, RoT); printf("\n");  for (int i = 0; i < 128; i++) printf("%02x", mac[i]);
	memset(mac, 0, 128);
	hmac_sha2_512_hw(key, msg, mac, RoT); printf("\n");  for (int i = 0; i < 128; i++) printf("%02x", mac[i]);
	memset(mac, 0, 128);
	hmac_sha2_512_256_hw(key, msg, mac, RoT); printf("\n");  for (int i = 0; i < 128; i++) printf("%02x", mac[i]);
	memset(mac, 0, 128);
	hmac_sha3_256_hw(key, msg, mac, RoT); printf("\n");  for (int i = 0; i < 128; i++) printf("%02x", mac[i]);
	memset(mac, 0, 128);
	hmac_sha3_512_hw(key, msg, mac, RoT); printf("\n");  for (int i = 0; i < 128; i++) printf("%02x", mac[i]);


	memset(mac, 0, 128);
	kmac128_hw(key, msg, 128, mac, RoT); printf("\n");  for (int i = 0; i < 128; i++) printf("%02x", mac[i]);
	memset(mac, 0, 128);
	kmac256_hw(key, msg, 128, mac, RoT); printf("\n");  for (int i = 0; i < 128; i++) printf("%02x", mac[i]);

	// X.509 
	unsigned char sim_x509[256]; for (int i = 0; i < 256; i++) sim_x509[i] = i;

	unsigned char* pub_key;
	unsigned char* pri_key;
	unsigned int pub_len;
	unsigned int pri_len;
	eddsa25519_genkeys_hw(&pri_key, &pub_key, &pri_len, &pub_len, RoT);

	unsigned char* sig;
	unsigned int sig_len;
	eddsa25519_sign_hw(sim_x509, strlen(sim_x509), pri_key, pri_len, pub_key, pub_len, &sig, &sig_len, RoT);

	unsigned int result;
	eddsa25519_verify_hw(sim_x509, strlen(sim_x509), pub_key, pub_len, sig, sig_len, &result, RoT);

	if (!result) printf("X.509: SUCCESS");
	else printf("X.509: FAIL");

	// Diffie-Hellman
	seed_rng();

	// ---- KEY GEN ---- //
	// A Side
	unsigned char* pub_key_A;
	unsigned char* pri_key_A;
	unsigned int pub_len_A;
	unsigned int pri_len_A;

	x25519_genkeys_hw(&pri_key_A, &pub_key_A, &pri_len_A, &pub_len_A, RoT);

	// B Side
	unsigned char* pub_key_B;
	unsigned char* pri_key_B;
	unsigned int pub_len_B;
	unsigned int pri_len_B;

	x25519_genkeys_hw(&pri_key_B, &pub_key_B, &pri_len_B, &pub_len_B, RoT);

	// Share secret
	unsigned char* ss_A;
	unsigned int ss_len_A;
	x25519_ss_gen_hw(&ss_A, &ss_len_A, pub_key_B, pub_len_B, pri_key_A, pri_len_A, RoT); // A Side

	unsigned char* ss_B;
	unsigned int ss_len_B;
	x25519_ss_gen_hw(&ss_B, &ss_len_B, pub_key_A, pub_len_A, pri_key_B, pri_len_B, RoT); // B Side

	// Cipher A -> B
	unsigned char ctx[32];
	unsigned int len_ctx;
	aes_128_ecb_encrypt_hw(ss_A, ctx, len_ctx, msg, sizeof(msg), RoT);
	unsigned char msg2[32];
	unsigned int len_msg;
	aes_128_ecb_decrypt_hw(ss_B, ctx, len_ctx, msg2, len_msg, RoT);

	if (memcmp(msg, msg2, len_msg)) printf("DH: SUCCESS");
	else printf("DH: FAIL");







}