
/******************************** Include Files *******************************/

#include "puf_hw.h"

/******************************** Test parameters *****************************/

#define N_CMPS 1024		// -c Number of comparisons
#define E_CMPS 0        //    Number of discarded comparisons
#define E_tests 100		// -m Number of test for enrollment

#define PE 0 // PUF/Entropy Source
#define BG 1 // -g Binary/Gray counters
#define LH 0 // -l Lower/Higher Bits



/******************************************************************************/
/******************************************************************************/

void puf_gen_enrollment(INTF* puf, unsigned int RC, unsigned int K_bits) {

	int puf_ref[MN_CMPS];
	int puf_selm[MN_CMPS];
	int puf_eref[MN_CMPS];
	int cr;

	int op;
	unsigned int puf_output[MN_REGS];
	int puf_selr[(int)ceil((MN_CMPS / 32.))];

	// Reset Challenges Mask
	op = 0;
	write_INTF(&puf, &op, CHLADDC, sizeof(op));

	unsigned int PUF_CONFIG; 
	PUF_CONFIG = (PE << 5) + (BG << 4) + (LH << 2);
		
	HammDist HDintra;

	FILE* CM_ptr;	// PUF Challenges Mask
	FILE* RO_ptr;	// PUF Reference Output

	char ChallengesMask_file[100];
	char ReferenceOutput_file[100];

	strcpy(ChallengesMask_file, "PUF");
	strcat(ChallengesMask_file, "_CM.txt");
	strcpy(ReferenceOutput_file, "PUF");
	strcat(ReferenceOutput_file, "_RO.txt");

	CM_ptr = fopen(ChallengesMask_file, "w");
	RO_ptr = fopen(ReferenceOutput_file, "w");

	// Perform the enrollment process 	    
	HDintra = PUF_enrollment(&puf, 0, 1, PUF_CONFIG, E_tests, N_CMPS, E_CMPS, puf_ref, puf_selm);

	// Get PUF reference output for N_CMPS-E_CMPS
	cr = 0;
	for (int c = 0; c < N_CMPS; c++) {
		if (puf_selm[c] == 0) {
			puf_eref[cr] = puf_ref[c];
			cr++;
		}
	}

	chl_btor(puf_selm, N_CMPS, puf_selr);

	// Write Challenges Selection Mask
	PUF_writeChallegesMask(&puf, puf_selr, ceil(MN_CMPS / 32.), 0);

	op = RCHL;
	write_INTF(&puf, &op, CHLADDC, sizeof(op));

	HDintra = PUF_HD(&puf, 1, PUF_CONFIG, puf_eref, puf_selm, E_tests, N_CMPS, E_CMPS);

	// Write ChallengesMask_file
	for (int c = 0; c < N_CMPS; c++) {
		fprintf(CM_ptr, "%1X", puf_selm[c]);
	}
	fclose(CM_ptr);

	// Write ReferenceOutput_file
	for (int c = 0; c < (N_CMPS - E_CMPS); c++) {
		fprintf(RO_ptr, "%1X", puf_eref[c]);
	}
	fclose(RO_ptr);

}

void puf_HelpDataGen(INTF* puf, unsigned int gen_enroll, unsigned int RC, char* key, char* HelpData) {

	int puf_eref[MN_CMPS];
	int puf_selm[MN_CMPS];
	char puf_refb[MN_BITS];
	int puf_selr[(int)ceil((MN_CMPS / 32.))];

	unsigned int op;
	// Reset Challenges Mask
	op = 0;
	write_INTF(&puf, &op, CHLADDC, sizeof(op));

	unsigned int PUF_CONFIG;
	PUF_CONFIG = (PE << 5) + (BG << 4) + (LH << 2);

	unsigned int K_bits = sizeof(key) * 8;

	char keyb[ML_KEY];
	char Ext_Keyb[MN_BITS];
	char Help_Datb[MN_BITS];

	// --- generate enrollment
	if (gen_enroll) puf_gen_enrollment(puf, 15, sizeof(key));

	// --- read enrollment
	FILE* CM_ptr;	// PUF Challenges Mask

	char ChallengesMask_file[100];

	strcpy(ChallengesMask_file, "PUF");
	strcat(ChallengesMask_file, "_CM.txt");

	CM_ptr = fopen(ChallengesMask_file, "w");

	// read enrollment files
	char  bufer[MN_CMPS + 1];

	//  read Challenge Mask from files
	fgets(bufer, N_CMPS + 1, CM_ptr);
	for (int c = 0; c < N_CMPS; c++) sscanf(&bufer[c], "%1X", &puf_selm[c]);

	chl_btor(puf_selm, N_CMPS, puf_selr);

	// Write Challenges Selection Mask
	PUF_writeChallegesMask(&puf, puf_selr, ceil(MN_CMPS / 32.), 0);

	op = RCHL;
	write_INTF(&puf, &op, CHLADDC, sizeof(op));

	// retry PUF output
	PUF_HD(&puf, 1, PUF_CONFIG, puf_eref, puf_selm, 1, N_CMPS, E_CMPS);

	puf_ctob(puf_eref, N_CMPS - E_CMPS, puf_refb);
	puf_ctob(key, K_bits, keyb);

	// Calculate Extended Key
	int kb = 0;
	for (int i = 0; i < K_bits; i++) {
		for (int j = 0; j < RC; j++) {
			kb = i * RC + j;
			Ext_Keyb[kb] = keyb[i];
		}
	}

	//   Generate Helper Data
	for (int b = 0; b < K_bits * RC; b++) {
		Help_Datb[b] = Ext_Keyb[b] ^ puf_refb[b];
	}

	// Convert binary to hexadecimal
	puf_btoh(Help_Datb, K_bits * RC, HelpData);
}

void puf_RetryKey(INTF* puf, unsigned int RC, char* key, char* HelpData) {

	// ------------------------- //
	// ---- READ PUF OUTPUT ---- //
	// ------------------------- //
	// --------------------------------------------------------------------------//
	int puf_eref[MN_CMPS];
	int puf_selm[MN_CMPS];
	char puf_refb[MN_BITS];
	int puf_selr[(int)ceil((MN_CMPS / 32.))];
	unsigned int op;
	// Reset Challenges Mask
	op = 0;
	write_INTF(&puf, &op, CHLADDC, sizeof(op));

	unsigned int PUF_CONFIG;
	PUF_CONFIG = (PE << 5) + (BG << 4) + (LH << 2);

	// --- read enrollment
	FILE* CM_ptr;	// PUF Challenges Mask

	char ChallengesMask_file[100];

	strcpy(ChallengesMask_file, "PUF");
	strcat(ChallengesMask_file, "_CM.txt");

	CM_ptr = fopen(ChallengesMask_file, "w");

	// read enrollment files
	char  bufer[MN_CMPS + 1];

	//  read Challenge Mask from files
	fgets(bufer, N_CMPS + 1, CM_ptr);
	for (int c = 0; c < N_CMPS; c++) sscanf(&bufer[c], "%1X", &puf_selm[c]);

	chl_btor(puf_selm, N_CMPS, puf_selr);

	// retry PUF output
	// Write Challenges Selection Mask
	PUF_writeChallegesMask(&puf, puf_selr, ceil(MN_CMPS / 32.), 0);

	op = RCHL;
	write_INTF(&puf, &op, CHLADDC, sizeof(op));

	// Generate PUF response
	PUF_applyChallenges(&puf, PUF_CONFIG, N_CMPS - E_CMPS, 0);

	//Read PUF results
	unsigned int puf_output[MN_REGS];
	PUF_readOutput(&puf, N_CMPS - E_CMPS, puf_output, 0);

	// Get PUF comparisons
	int  puf_outc[MN_CMPS];
	unsigned int N_regs = ceil((N_CMPS - E_CMPS) / (Dbw / 4.));
	puf_rtoc(puf_output, N_regs, puf_outc);

	// Get PUF bits  
	char  puf_outb[MN_CMPS * 8];
	puf_ctob(puf_outc, N_CMPS - E_CMPS, puf_outb);


	// --------------------------------------------------------------------------//
	// ------------------------- //
	// ----		RETRY KEY	---- //
	// ------------------------- //
	// --------------------------------------------------------------------------//

	char Ext_Rkeyb[MN_BITS];
	char Rec_keyb[MN_BITS];
	char Help_Datb[MN_BITS];
	puf_ctob(HelpData, sizeof(HelpData), Help_Datb);

	unsigned int HD_bits = sizeof(HelpData)*8;
	// XOR with Helper Data
	for (int b = 0; b < HD_bits; b++) {
		Ext_Rkeyb[b] = Help_Datb[b] ^ puf_outb[b];
	}

	// Apply Error Correction Code 
	puf_ecc(Ext_Rkeyb, HD_bits, RC, Rec_keyb);

	// Convert binary to hexadecimal
	puf_btoh(Rec_keyb, HD_bits/RC, key);

}